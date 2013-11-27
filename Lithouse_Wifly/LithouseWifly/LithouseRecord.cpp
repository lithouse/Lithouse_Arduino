#include <LithouseRecord.h>
#include <SPI.h>

LithouseRecord::LithouseRecord ( ) { }


LithouseRecord::LithouseRecord ( const char* channel, const char* data ) {
	//TODO: consider overflow
	strcpy ( _channel, channel );
	strcpy ( _data, data );
}

void LithouseRecord::updateRecord ( const char* channel, const char*  data ) {
	//TODO: consider overflow
	strcpy ( _channel, channel );
	strcpy ( _data, data );
}

size_t LithouseRecord::printTo ( Print& print ) const {
	int len = print.print ( "{\"" );
	len += print.print ( Constants::CHANNEL );
	len += print.print ( "\":\"" );
	len += print.print ( _channel );
	len += print.print ( "\",\"" );

	len += print.print ( Constants::DATA );
	len += print.print ( "\":\"" );
	len += print.print ( _data );
	
	len += print.print ( "\"}" );
	return len;
}

//TODO: Robust overflow check
int LithouseRecord::concatRecord ( char* buffer, int MAX_SIZE ) {
	int recordLength = 30 + strlen ( _channel ) + strlen ( _data );
	
	if (recordLength + strlen(buffer) >= MAX_SIZE) {
		Serial.println("ERROR: reocrd is too long. Please shorten the channel name or data.");
		return Constants::ERROR_OVERFLOW;
	}

	strcat ( buffer, "{\"" );
	strcat ( buffer, Constants::CHANNEL );
	strcat ( buffer, "\":\"" );
	strcat ( buffer, _channel );
	strcat ( buffer, "\",\"" );

	strcat ( buffer, Constants::DATA );
	strcat ( buffer, "\":\"" );
	strcat ( buffer, _data );
	
	strcat ( buffer, "\"}" );	

	return Constants::SUCCESS;
}

//TODO: consider overflow
void LithouseRecord::getData ( char* data ) {
	strcpy ( data, _data );
}

//TODO: consider overflow
void LithouseRecord::getChannel ( char* channel ) {
	strcpy ( channel, _channel );
}