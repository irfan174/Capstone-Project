/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 4
 * LCD Enable pin to digital pin 5
 * LCD D4 pin to digital pin 12
 * LCD D5 pin to digital pin 13
 * LCD D6 pin to digital pin 14
 * LCD D7 pin to digital pin 16
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 */

// include the library code:

#include <ESP8266WiFi.h>
#include "LCD.h"
#include "LiquidCrystal_I2C.h"


LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);
#include "esplcdweb.h"
String command;
// initialize the library with the numbers of the interface pins

const char* ssid     = "Mi Phone"; // Your WiFi ssid
const char* password = "12345678"; // Your Wifi password;
// size of buffer that stores a line of text for the LCD + null terminator
#define LCD_BUF_SZ   17

char lcd_buf_1[LCD_BUF_SZ] = {0};// buffer to save LCD line 1 text to
char lcd_buf_2[LCD_BUF_SZ] = {0};// buffer to save LCD line 2 text to
char charBuf[60];
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
   lcd.setBacklightPin(3,POSITIVE);
   lcd.setBacklight(HIGH);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

}
// get the two strings for the LCD from the incoming HTTP GET request
boolean GetLcdText(char *line1, char *line2, int len)
{
  boolean got_text = false;    // text received flag
  char *str_begin;             // pointer to start of text
  char *str_end;               // pointer to end of text
  int str_len = 0;
  int txt_index = 0;
  char *current_line;

  current_line = line1;

  // get pointer to the beginning of the text
  str_begin = strstr(charBuf, "&L1=");

  for (int j = 0; j < 2; j++) { // do for 2 lines of text
    if (str_begin != NULL) {
      str_begin = strstr(str_begin, "=");  // skip to the =
      str_begin += 1;                      // skip over the =
      str_end = strstr(str_begin, "&");

    
      if (str_end != NULL) {
        str_end[0] = 0;  // terminate the string
        str_len = strlen(str_begin);
        // copy the string to the buffer and replace %20 with space ' '
        for (int i = 0; i < str_len; i++) {
          if (str_begin[i] != '%') {
            if (str_begin[i] == 0) {
              // end of string
              break;
            }
            else {
              current_line[txt_index++] = str_begin[i];
              if (txt_index >= (len - 1)) {
                // keep the output string within bounds
                break;
              }
            }
          }
          else {
            // replace %20 with a space
            if ((str_begin[i + 1] == '2') && (str_begin[i + 2] == '0')) {
              current_line[txt_index++] = ' ';
              i += 2;
              if (txt_index >= (len - 1)) {
                // keep the output string within bounds
                break;
              }
            }
          }
          } // end for i loop
        // terminate the string
        current_line[txt_index] = 0;
        if (j == 0) {
          // got first line of text, now get second line
          str_begin = strstr(&str_end[1], "L2=");

          current_line = line2;
          txt_index = 0;
        }
        got_text = true;
      }
    }
  } // end for j loop

  return got_text;
}


void loop() {

  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String command1 = client.readStringUntil('/');
  command = client.readStringUntil('/');
  Serial.println(command);
if (command.indexOf("t") != -1) {
  command.toCharArray(charBuf, 70) ;

   // print the received text to the LCD if found
                        if (GetLcdText(lcd_buf_1, lcd_buf_2, LCD_BUF_SZ)) {
                          // lcd_buf_1 and lcd_buf_2 now contain the text from
                          // the web page
                          // write the received text to the LCD
                          lcd.clear();
                          lcd.setCursor(0, 0);
                          lcd.print(lcd_buf_1);
                          lcd.setCursor(0, 1);
                          lcd.print(lcd_buf_2);
                          
                        }
}
else {  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += file1;  
  client.flush();

  // Send the response to the client
while(s.length()>2000)
{
  String dummy = s.substring(0,2000);
  client.print(dummy);
  s.replace(dummy," ");
}
  
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is destroyed
}
}
