/*
 PIR motion sensor connected to Lithouse. 
 
 This sketch monitors an input pin which is connected to a PIR sensor. 
 Lithouse cloud is notified, if there is a LOW to HIGH transition on that pin.   
 
 Circuit:
 * Described in http://www.instructables.com/id/Triggering-IFTTT-from-Arduino-using-Lithouse/
 
 created 11 Nov 2013
 by Shah Hossain
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Lithouse.h>

byte mac[] = { 0x00, 0x13, 0x22, 0xEF, 0x16, 0x7E };
EthernetClient client;


//Copy deviceKey from Lithouse portal
char deviceKey [] = "YOUR_DEVICE_KEY";
char pirChannel [] = "PIR";

LithouseClient litClient ( client, deviceKey );
//Maximum channel number for simultaneous read or write
const int MAX_CHANNEL_COUNT = 1;
LithouseRecord records [MAX_CHANNEL_COUNT]; 


int ledPin = 7;                // output pin for the LED
int inputPin = 2;               // input pin for the PIR sensor
int pirState = LOW;             // we start assuming that no motion is detected
int pirValue = 0;               // reading of the PIR sensor

void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
 
  Serial.begin(9600);
  
  //connecting to lithouse
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.println("connecting...to ethernet");  
  // Start the Ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while ( 1 ) {
      // Hang on failure  
    }    
  }
  // Give the Ethernet shield a second to initialize
  delay ( 1000 );
  Serial.println("connecting...to lithouse"); 
}
 
void loop(){
  pirValue = digitalRead(inputPin);      // read input value
  
  if (pirValue == HIGH) {                // check if the input is HIGH, which means motion is detected
    digitalWrite(ledPin, HIGH);          // turn LED ON
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      pirState = HIGH;
      
      // send to lithouse
      records[0].updateRecord ( pirChannel, "detected" );
      litClient.send ( records, 1 );
    }
  } else {
    digitalWrite(ledPin, LOW);           // turn LED OFF
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }
}


