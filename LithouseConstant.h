#ifndef _LITHOUSE_CONSTANT_H_
#define _LITHOUSE_CONSTANT_H_

class Constants
{
public:
   static const char* DATA;
   static const char* CHANNEL;
   static const char* TIMESTAMP;
   static const char* RECORDS;
   //Maximum length of a channel name or data string
   static const int MAX_VALUE_LENGTH = 10;

   static const int ERROR_OVERFLOW = -101;   
   static const int ERROR_NETWORK_FAILURE = -102;

   static const int SUCCESS = 1001;
};


#endif