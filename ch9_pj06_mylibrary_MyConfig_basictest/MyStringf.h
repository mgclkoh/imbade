#ifndef MY_STRINGF_H
#define MY_STRINGF_H

#ifndef MY_STRINGF_BUF

#if defined(__AVR__) 
#define MY_STRINGF_BUF 128
#else 
#define MY_STRINGF_BUF 512
#endif 

#endif 

String MyStringf(char* fmt, ...);

#endif 