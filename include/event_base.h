#ifndef _EVENT_BASE_H
#define _EVENT_BASE_H

#include <event2/event.h>

class EventBase {
 public:
  EventBase() {};
  void Init();
  event_base *eb;
  
  __attribute__((unused)) static void log_CB(int sev, const char* msg);
  __attribute__((unused)) static void fatal_CB(int err);
};

#endif
