/*
 Lithouse Ethernet client 
 
 This sketch connects to api.lithouse.co through LithouseClient.
 It expects "on" or "off" command on incoming "LED" channel. 
 And, uploads normalized analog readings to "FSR" channel.  
 
 Circuit:
 * Described in getting started with Arduino tutorial in lithouse.co.
 
 created 11 Nov 2013
 by Shah Hossain
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <Lithouse.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x00, 0x13, 0x22, 0xEF, 0x16, 0x7E };
EthernetClient client;

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

