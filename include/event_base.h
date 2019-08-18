#ifndef _EVENT_BASE_H
#define _EVENT_BASE_H

#include <event2/event.h>

// EventBase is a wrapper class for libevent stuff.  Mostly we are
// using this to hide some logging setup and provide a nice looking
// c++ style interface (notably an Init() call) to the other, more
// functionally oriented parts of the system

class EventBase {
 public:
  EventBase() {};

  // Initialize the libevent event base and attach some logging callbacks.
  void Init();
  event_base *eb;
  
  // libevent logging callbacks -- set up libevent to speak via glog
  __attribute__((unused)) static void log_CB(int sev, const char* msg);
  __attribute__((unused)) static void fatal_CB(int err);
};

#endif
