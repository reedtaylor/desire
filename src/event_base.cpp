#include "event_base.h"

#include <event2/event.h>
#include <glog/logging.h>


void EventBase::Init() {
  DLOG(INFO) << "EventBase: Initializing";
  eb = event_base_new();
  if (!eb) {
   LOG(FATAL) << "EventBase: Could not create event base";
  }

  DLOG(INFO) << "EventBase: passing logging callbacks to eventlib";
  event_set_log_callback(log_CB);
  event_set_fatal_callback(fatal_CB);
  //event_enable_debug_mode(); // verbose & compute intensive
}

/*
 * Eventlib logging callbacks -- set up to use glog
 */
void EventBase::log_CB(int sev, const char* msg)  {
  switch (sev) {
  case _EVENT_LOG_DEBUG: DLOG(INFO) << "EventBase: " << msg; break;
  case _EVENT_LOG_WARN: LOG(WARNING) << "EventBase: " << msg; break;
  case _EVENT_LOG_ERR: LOG(ERROR) << "EventBase: " << msg; break;
  case _EVENT_LOG_MSG:
  default: LOG(INFO) << "EventBase: " << msg; break;
  }
}

void EventBase::fatal_CB(int err) {
  LOG(FATAL) << "EventBase: Something went wrong. Fatal-ing with error: " << err;
}


