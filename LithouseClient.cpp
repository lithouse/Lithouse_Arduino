#include <LithouseClient.h>
#include <SPI.h>

const char* LithouseClient::API_ENDPOINT = "api.lithouse.co";
const char* LithouseClient::API_URI = "/v1/records HTTP/1.0";
const char* LithouseClient::USER_AGENT = "User-Agent: Arduino-lib/1.0";
const char* LithouseClient::CONTENT_TYPE = "Content-Type: application/json";

LithouseClient::LithouseClient(Client& client, const char* deviceKey)
: _client(client)
{
	strcpy ( _deviceKey, deviceKey );
}

int LithouseClient::send(LithouseRecord records[], int recordCount) {
	if (recordCount < 1) {
		return recordCount;
	}

	if (_client.connect(API_ENDPOINT, API_PORT)) {
		//Serial.println("Writing to Lithouse");

		int result = createRequestBody(records, recordCount);
		if (result < 0) {
			return result;
		}
		
		_client.print ( "POST " );
		_client.println ( API_URI );
		_client.println ( CONTENT_TYPE );
		_client.print ( "deviceKey: " );
		_client.println ( _deviceKey );
		_client.println ( USER_AGENT );
		_client.println ( "Connection: close" );
		_client.print ( "Content-Length: " );
		_client.println ( strlen ( _requestBodyBuffer ) );
		_client.println ( );
		
		_client.println(_requestBodyBuffer);

	}
	else {
		Serial.println("ERROR: Connection failed");
		return Constants::ERROR_NETWORK_FAILURE;
	}

	delay(API_CALL_DELAY);
	_client.stop();

	return recordCount;
}

int LithouseClient::receive(LithouseRecord records[], int MAX_RECORD_COUNT) {
	if (_client.connect(API_ENDPOINT, API_PORT)) {
		//Serial.println("Reading from Lithouse");
		_client.print("GET ");
		_client.println ( API_URI );
		_client.print("deviceKey: ");
		_client.println ( _deviceKey );
		_client.println(USER_AGENT);
		_client.println();
	}
	else {
		Serial.println("ERROR: Connection failed");
		return Constants::ERROR_NETWORK_FAILURE;
	}

	int result;
	while (_client.connected()) {
		result = readLine();
		if (result < 0) {
			return result;
		}

	}
	//Serial.println(_requestBodyBuffer);

	_client.stop();
	delay(API_CALL_DELAY);

	return parseResponseBody(records, MAX_RECORD_COUNT);
}

int LithouseClient::readLine() {
	char character;
	int currentLength = 0;

	while (_client.connected() && (character = _client.read()) != '\n') {
		if (character != '\r' && character != -1) {
			if ((currentLength + 1) == MAX_REQUEST_LINE_LENGTH) {
				Serial.println("ERROR: response is too long.");
				return Constants::ERROR_OVERFLOW;
			}
			_requestBodyBuffer[currentLength++] = character;
		}
	}

	_requestBodyBuffer[currentLength] = 0;
	return currentLength;
}

//void LithouseClient::printRecrods ( LithouseRecord records [], int recordCount ) {
//	if ( recordCount < 1 ) return; 
//	_client.print ( "{\"" );
//	_client.print ( Constants::RECORDS );
//	_client.print ( "\":[" );
//	_client.print ( records [ 0 ] );
//
//	for ( int i=1; i < recordCount; i++ ) {
//		_client.print ( "," );
//		_client.print ( records [ i ] );
//	}
//	_client.println ( "]}" );
//}

int LithouseClient::createRequestBody(LithouseRecord records[], int recordCount) {
	strcpy(_requestBodyBuffer, "");
	if (recordCount < 1) return Constants::SUCCESS;
	strcat(_requestBodyBuffer, "{\"");
	strcat(_requestBodyBuffer, Constants::RECORDS);
	strcat(_requestBodyBuffer, "\":[");

	int result = records[0].concatRecord(_requestBodyBuffer, MAX_REQUEST_LINE_LENGTH);
	if (result != Constants::SUCCESS) {
		return result;
	}

	for (int i = 1; i < recordCount; i++) {
		strcat(_requestBodyBuffer, ",");
		int result = records[i].concatRecord(_requestBodyBuffer, MAX_REQUEST_LINE_LENGTH);

		if (result != Constants::SUCCESS) {
			return result;
		}
	}
	strcat(_requestBodyBuffer, "]}");

	//Serial.println(_requestBodyBuffer);

	return Constants::SUCCESS;
}

int LithouseClient::parseResponseBody(LithouseRecord records[], int MAX_RECORD_COUNT) {
	int count = 0;
	char data[Constants::MAX_VALUE_LENGTH], channel[Constants::MAX_VALUE_LENGTH];
	char* ptr = strstr(_requestBodyBuffer, Constants::CHANNEL);

	while (ptr != NULL) {
		if (count == MAX_RECORD_COUNT) {
			Serial.println("ERROR: Increase the size of LithouseRecord array.");
			return Constants::ERROR_OVERFLOW;
		}

		if ((ptr = extractNextJSONValue(ptr, channel)) == NULL) break;
		if ((ptr = strstr(ptr, Constants::DATA)) == NULL) break;
		if ((ptr = extractNextJSONValue(ptr, data)) == NULL) break;

		records[count++].updateRecord(channel, data);
		ptr = strstr(ptr, Constants::CHANNEL);
	}

	return count;
}

char* LithouseClient::extractNextJSONValue(const char* jsonString, char* value) {
	char* ptr = strstr(jsonString, ":\"");
	if (ptr == NULL) return ptr;

	ptr += 2;
	int i;
	for (i = 0; ptr[i] && ptr[i] != '"'; i++) {
		value[i] = ptr[i];
	}
	value[i] = 0;
	ptr += i;

	return ptr;
}