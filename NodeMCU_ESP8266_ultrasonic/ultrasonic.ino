int trigPin = 11;
int echoPin = 10;
int led = 9;
int ldr = A0;



void setup() {
  Serial.begin(115200); 
   pinMode(led, OUTPUT);
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldr, INPUT);
  // put your setup code here, to run once:
        

}

void loop() {
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
