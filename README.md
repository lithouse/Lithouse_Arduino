Lithouse_Arduino
================

Lithouse_Arduino is a library for connecting with Lithouse cloud. It includes examples for coomunicating over both Ethernet and WiFi. 
You may find more details in http://lithouse.co.

##Normal Usage
As a first step, you need to create a `LithouseClient` object in your sketch with the `deviceKey` from http://lithouse.co.

```c
EthernetClient client; // change this to WiFiClient, if you are using WiFi shield
LithouseClient litClient ( client, deviceKey );
```

You may send one or more records to Lithouse with a single api call. First, update the channel names and data string of each record. Then pass the records array to `LithouseClient`. 

```c
const int MAX_CHANNEL_COUNT = 2;
LithouseRecord records [MAX_CHANNEL_COUNT]; 

records[0].updateRecord ( channelNames, dataBuffer );   
litClient.send ( records, 1 ); 
```

Similarly, you may receive one or more records from Lithouse:

```c
litClient.receive ( records, MAX_CHANNEL_COUNT );
```

##Error Codes

* `ERROR_OVERFLOW = -101`, the supplied ` LithouseRecord` does not contain enough space for incoming records. Or, the channel name or data buffer is longer than 10 characters.    
* `ERROR_NETWORK_FAILURE = -102`, the client is unable to connect to api.lithouse.co.

The library will also print out debugging information on the serial monitor in case of an error.

