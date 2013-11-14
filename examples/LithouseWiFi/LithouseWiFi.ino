/*
 Lithouse WiFi client 
 
 This sketch connects to api.lithouse.co through LithouseClient.
 It expects "on" or "off" command on incoming "LED" channel. 
 And, uploads normalized analog readings to "FSR" channel.  
 
 Circuit:
 * Described in getting started with Arduino tutorial in lithouse.co.
 
 created 11 Nov 2013
 by Shah Hossain
 
 */

#include <SPI.h>
#include <WiFi.h>
#include <Lithouse.h>

char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_NETWORK_PASSWORD";
WiFiClient client;
int status = WL_IDLE_STATUS;

//Copy deviceKey from Lithouse portal
char deviceKey [] = "YOUR_DEVICE_KEY";
//Incoming channel
char ledChannel [] = "LED";
//Outgoing channel
char fsrChannel [] = "FSR";

LithouseClient litClient ( client, deviceKey );
//Maximum channel number for simultaneous read or write
const int MAX_CHANNEL_COUNT = 1;
LithouseRecord records [MAX_CHANNEL_COUNT]; 

int LED_OUT_PIN = 5;
int FSR_IN_PIN = 0;
int fsrPressure = 0;
int fsrReading;

//Maximum length of a channel name or data string
const int MAX_LENGTH = 10;
char dataBuffer [MAX_LENGTH];
char channelBuffer [MAX_LENGTH];

void setup() {
  pinMode(LED_OUT_PIN, OUTPUT);
  Serial.begin ( 9600 );
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("connecting...to lithouse");  
}

void loop() {
  downloadLedState ( );
  uploadFSRState ( );
  
  delay ( 1000 );        
}

void uploadFSRState ( ) {
  fsrReading = analogRead ( FSR_IN_PIN );
  Serial.print ( "FSR reading = " );
  Serial.println ( fsrReading );
  
  //Normalizing FSR reading 
  int currentPressure = (fsrReading > 800) ? 80 : 0;
  
  //Only upload reading if there was a change  
  if ( currentPressure != fsrPressure ) {
    fsrPressure = currentPressure;
    itoa ( fsrPressure, dataBuffer, 10 );
    records[0].updateRecord ( fsrChannel, dataBuffer );
    
    litClient.send ( records, 1 );   
  }
}

void downloadLedState ( ) {
  if ( litClient.receive ( records, MAX_CHANNEL_COUNT ) == 1 ) {
    
    records[0].getData ( dataBuffer );
    records[0].getChannel ( channelBuffer );
    
    if ( 0 == stricmp ( channelBuffer, ledChannel ) 
        && 0 == stricmp ( dataBuffer, "on" ) ) {
      
      Serial.println ( "turn led on" );
      digitalWrite(LED_OUT_PIN, HIGH);
    } else {
      
      Serial.println ( "turn led off" );
      digitalWrite ( LED_OUT_PIN , LOW );    
    }
  }
}

int stricmp(const char *s1, const char *s2) 
{ 
  while(tolower((unsigned char) *s1) == tolower((unsigned char) *s2)) { 
    if(*s1++ == '\0') 
      return 0; 
    s2++; 
  } 
  
  return (unsigned char) *s1 < (unsigned char) *s2 ? -1 : 1; 
} 

