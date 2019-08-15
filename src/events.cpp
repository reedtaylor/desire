#include "events.h"

#include <event2/event.h>
#include <glog/logging.h>

#include "desire_util.h"


/*
 * Eventlib logging callbacks -- set up to use glog
 */
static void eventlib_log_CB(int sev, const char* msg) {
  switch (sev) {
  case _EVENT_LOG_DEBUG: DLOG(INFO) << msg; break;
  case _EVENT_LOG_WARN: LOG(WARNING) << msg; break;
  case _EVENT_LOG_ERR: LOG(ERROR) << msg; break;
  case _EVENT_LOG_MSG:
  default: LOG(INFO) << msg; break;
  }
}

static void eventlib_fatal_CB(int err) {
  LOG(FATAL) << "Something went wrong. Fatal-ing with error: " << err;
}


/*
 * Initialize the event_base and return it
 */
event_base * SetupEventBase() {

  DLOG(INFO) << "Generating the event_base for primary reads";
  event_base* eb = event_base_new();
  if (!eb) {
   LOG(FATAL) << "Could not create event base";
  }

  DLOG(INFO) << "Passing logging callbacks to eventlib";
  event_set_log_callback(eventlib_log_CB);
  event_set_fatal_callback(eventlib_fatal_CB);
  //event_enable_debug_mode(); // verbose & compute intensive

  return eb; 
}

