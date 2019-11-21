#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "LCD.h"
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);
int led = 2; // the pin the LED is connected to
int trigPin = 14;
int echoPin = 12;
int led2 = 15;
int ldr = A0;
const char* ssid     = "Mi Phone"; // Your WiFi ssid
const char* password = "12345678"; // Your Wifi password;
const char* Commands_Reply;                 // The command variable that is sent to the client

const char * host = "192.168.43.145";          // IP Client
//const char *baseStation = "192.168.43.112"; //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.


WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(led2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldr, INPUT);
  pinMode(led, OUTPUT);                     // Declare the LED as an output
  Serial.begin(115200);                     // initialize serial:
  delay(10);

  Serial.println("");
  Serial.println("Server-------------------------------");
  Serial.print("Configuring access point");
  WiFi.begin(ssid, password);

  // Waiting to connect to wifi
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
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
    Serial.println("-------------------------------------");
    Serial.println("");


   lcd.begin(20,4); 
   lcd.setBacklightPin(3,POSITIVE);
   lcd.setBacklight(HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Check if a client has connected
  
  //powerSaving();
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("Server-------------------------------");
  Serial.println("New client");
  Serial.print("From client = ");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request -------------------------------------
   String req = client.readStringUntil('\r');
   Serial.println(req);
   client.flush();

   //Command LED -------------------------------------------------------------
    if (req.indexOf("LED_Blink") != -1){
      Commands_Reply = "LED Status: Blinking";
      Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      digitalWrite(led, HIGH);
      delay(400);
      digitalWrite(led, LOW);
      delay(400);
      digitalWrite(led, HIGH);
      delay(400);
      digitalWrite(led, LOW);
      delay(400);
      digitalWrite(led, HIGH);
      delay(400);
      digitalWrite(led, LOW);
      delay(500);

      //sendToBasestation();
      //send ip to database--------------------------
      //car Ip-------------------




      





      //display work--------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Accident Occured");
      lcd.setCursor(0,1);
      lcd.print("On Lamppost 1");
      delay(8000);
      lcd.clear();
    }
   else if (req.indexOf("LED_On") != -1){
      Commands_Reply = "LED Status : On";
      Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      digitalWrite(led, HIGH);
      delay(500);
      //sendToBasestation();
     



      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Health Emmergency");
      lcd.setCursor(0,1);
      lcd.print("Ambulence Stucked");
      delay(6000);
      digitalWrite(led, LOW);
       delay(8000);
      lcd.clear();
    }
   /*else if (req.indexOf("LED_Off") != -1){
    Commands_Reply = "LED Status : Off";
    Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      digitalWrite(led, LOW);

      //sendToBasestation();





      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(Commands_Reply);
      delay(1500);
      lcd.clear();
    }*/
   else {
     Serial.println("invalid request");
     client.stop();
     return;
    }

   client.flush();
   Serial.println("Client disonnected");
   Serial.println("-------------------------------------");
   Serial.println("");
}
void powerSaving()
{
   int ldrValue = analogRead(ldr);
  Serial.print("LDR value = ");
  Serial.println( ldrValue);
  delay(10000);
  
 if(( ldrValue >=660 && ldrValue <=700 ))
    {
        
        long duration, distance;
        digitalWrite(trigPin,HIGH);
        delayMicroseconds(1000);
        digitalWrite(trigPin, LOW);
        duration=pulseIn(echoPin, HIGH);
        distance =(duration/2)/29.1;
        //Serial.print(distance);
        //Serial.println("CM");
        //delay(10);
      Serial.println("Low Brightness");
      analogWrite(led, 1);
      Serial.println("LED light DIMMING");
      if((distance<=100)) 
      {
         analogWrite(led, 255);
         Serial.println("Object Detected");
         Serial.println("LED light ON");
         delay(30);
    
  
     }
      else if(distance>100)
      {
         analogWrite(led, 2);
          }
    }
  else
  {
    analogWrite(led, 0);
    Serial.println("High Brightness");
    Serial.println("LED light OFF");
    long duration, distance;
        digitalWrite(trigPin,HIGH);
        delayMicroseconds(1000);
        digitalWrite(trigPin, LOW);
        duration=pulseIn(echoPin, HIGH);
        distance =(duration/2)/29.1;
    if((distance<=100)) 
      {
         analogWrite(led, 0);
         Serial.println("Object Detected");
         Serial.println("LED light OFF");
         delay(30);
  }
  
 
}
  

}
