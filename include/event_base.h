#ifndef _EVENT_BASE_H
#define _EVENT_BASE_H

#include <event2/event.h>

class EventBase {
 public:
  EventBase();
  void Init();
  
 private:
  static void log_CB(int sev, const char* msg);
  static void fatal_CB(int err);

  static event_base *eb;

}

#endif
