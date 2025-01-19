#ifndef DEBUG_H
#define DEBUG_H

// debugging
#define DEBUG 0     // choose to debug or not; 1 is debugging 0 is not

#if DEBUG == 1
    #define debug(x)      Serial.print(x)
    #define debugln(x)    Serial.println(x)
    #define debugBegin(x) Serial.begin(x)
#else
    #define debug(x)
    #define debugln(x)
    #define debugBegin(x)
#endif

#endif