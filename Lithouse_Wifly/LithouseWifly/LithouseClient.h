#ifndef _LITHOUSECLIENT_H_
#define _LITHOUSECLIENT_H_

#include <LithouseConstant.h>
#include <LithouseRecord.h>
#include <WiFly.h>

class LithouseClient
{
public:
	LithouseClient(WiFlyClient& client, const char* deviceKey);
	
	int receive(LithouseRecord records[], int MAX_RECORD_COUNT);
	int send ( LithouseRecord records [], int recordCount );

private:
	int readLine ( );
	int createRequestBody(LithouseRecord records[], int recordCount);
	int parseResponseBody(LithouseRecord records[], int MAX_RECORD_COUNT);
	char* extractNextJSONValue ( const char* jsonString, char* value );

	WiFlyClient& _client;

	static const int MAX_DEVICE_KEY_LENGTH = 40;
	//Maximum length of a HTTP request line
	static const int MAX_REQUEST_LINE_LENGTH = 300;
	static const int API_PORT = 80;
	//Delay between individual read and write calls
	static const int API_CALL_DELAY = 4000;

	static const char* API_ENDPOINT;
	static const char* API_URI;
	static const char* USER_AGENT;
	static const char* CONTENT_TYPE;
	char _deviceKey [MAX_DEVICE_KEY_LENGTH];
	char _requestBodyBuffer [MAX_REQUEST_LINE_LENGTH];
};


#endif