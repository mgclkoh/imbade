#include <Arduino.h> //stdarg.h, stdio.h
#include "MyStringf.h"

String MyStringf(char* fmt, ...) {

  char buf[MY_STRINGF_BUF] = {0,};
  va_list ap; //arg_ptr

  va_start(ap, fmt); 
  vsnprintf(buf,sizeof(buf),fmt,ap);
  va_end(ap);

  return String(buf);
}