#ifndef _DESIRE_UTIL_H
#define _DESIRE_UTIL_H

#include <event2/event.h>

#define CHARBUF_SIZE 1024

struct DesireState {
  
  // libEvent event_base
  event_base *eb;
  
  // file handle & descriptor for DE serial port
  FILE *de_serial;
};

#endif

