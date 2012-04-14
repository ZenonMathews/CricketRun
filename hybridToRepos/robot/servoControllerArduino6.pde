#include <Servo.h> 
 
Servo myservo1;      // create servo object to control a servo  
Servo myservo2;
int pos = 0;         // variable to store the servo position 
char * incomingByte; // for incoming serial data
int ledPin = 13;

void setup() 
{ 
  Serial.begin(57600);
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(10);
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
} 
 
 
void loop() 
{ 
  
 
    int readByte = Serial.read(); // read the incoming byte:
 
  if (readByte == 'W') {   //go front
   
               myservo1.write(30);
               myservo2.write(150);
               digitalWrite(ledPin, HIGH);   // sets the LED on
               delay(50);
    	 }

  if (readByte == 'S') {   // go back
               myservo1.write(150);
               myservo2.write(30);
               digitalWrite(ledPin, HIGH);   // sets the LED on
               delay(50);
     	 }



    if (readByte == 'A') {   // go left
               myservo1.write(30);
               myservo2.write(30);
               digitalWrite(ledPin, HIGH);   // sets the LED on
               delay(50);
         }


    if (readByte == 'D') {   // go right
               myservo1.write(150);
               myservo2.write(150);
               digitalWrite(ledPin, HIGH);   // sets the LED on
               delay(50);
         }

 
    else {
          digitalWrite(ledPin, LOW);   // sets the LED on
          myservo1.write(90);
          myservo2.write(90);   
  }
 }

 
