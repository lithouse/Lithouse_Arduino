#ifndef _LITHOUSE_RECORD_H_
#define _LITHOUSE_RECORD_H_

#include <Printable.h>
#include <Stream.h>
#include <LithouseConstant.h>


class LithouseRecord : public Printable {
public:
	LithouseRecord ( );
	LithouseRecord ( const char* channel, const char*  data );
	void getChannel ( char* channel );
	void getData ( char* data );
	//void getTimestamp ( char* timestamp );

	void updateRecord ( const char* channel, const char*  data );
	int concatRecord(char* buffer, int MAX_SIZE);
	virtual size_t printTo ( Print& print) const;
private:
	
	char _channel [Constants::MAX_VALUE_LENGTH];
	char _data [Constants::MAX_VALUE_LENGTH];
	//char _timestamp [MAX_VALUE_LENGTH];
};

#endif