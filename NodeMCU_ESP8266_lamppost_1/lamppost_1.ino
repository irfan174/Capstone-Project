#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

int led = 5; // the pin the LED is connected to
int trigPin = 14;
int echoPin = 12;
int led2 = 15;
int ldr = A0;

const char* ssid     = "Mi Phone"; // Your WiFi ssid
const char* password = "12345678"; // Your Wifi password;
const char* Commands_Reply;                 // The command variable that is sent to the client

const char * host = "192.168.43.85";          // IP Client
const char * host1 = "192.168.43.145";          // IP Server
const char *baseStation = "192.168.43.112"; //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.


const int httpPort = 80;
const char* Commands;

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
      Commands_Reply = "LED Status : Blinking";
      Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      Serial.println(host);
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
      delay(1000);

      Serial.println("Client-------------------------------");
      Serial.print("Send Command = ");
      Commands="LED_Blinking";
      Serial.println(Commands);
      send_commands();
      sendToBasestation();

      
    }
   else if (req.indexOf("LED_On") != -1){
      Commands_Reply = "LED Status : On";
      Serial.print("Server send = ");
      Serial.println(Commands_Reply);
      client.print(String("GET ") + Commands_Reply + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      Serial.println(host);
      digitalWrite(led, HIGH);
      delay(1000);

      Serial.println("Client-------------------------------");
      Serial.print("Send Command = ");
      Commands="LED_On";
      Serial.println(Commands);
      send_commands();
      delay(5000);
      digitalWrite(led, LOW);
  
    }
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
void send_commands(){
  Serial.println("Sending command...");
  //Serial.println("Don't press the button for now...");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(host1);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  if (!client.connect(host1, httpPort)) {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URI for the request  
  Serial.print("Requesting URL : ");
  Serial.println(Commands);

  // This will send the request to the server
  client.print(String("GET ") + Commands + " HTTP/1.1\r\n" + "Host: " + host1 + "\r\n" + "Connection: Close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {      
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  Serial.print("Server Reply = "); 
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  /*Serial.println("Now you can press the button ...");
  Serial.println("-------------------------------------");
  Serial.println("");*/
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
      if((distance<=50)) 
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
    if((distance<=50)) 
      {
         analogWrite(led, 0);
         Serial.println("Object Detected");
         Serial.println("LED light OFF");
         delay(30);
  }
  
 
}
  

}




void sendToBasestation()
{
      HTTPClient http;    //Declare object of class HTTPClient
 
  String postData, carIp;
  
  carIp = String(host);   //String to interger conversion
  //String IPaddress =  WiFi.localIP().toString();
  //Post Data
  postData = "ldrvalue=" + carIp;
 
  http.begin("http://192.168.43.112/Nodemcu_db_record_view_new/InsertDB.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
  
  

  //Serial.println("LDR Value=" + ldrvalue);
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  Serial.println("Ip Address = " + carIp);
  
  http.end();  //Close connection
  
}
