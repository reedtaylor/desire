#include <iostream>
#include <cstdio>
#include <vector>
#include <event2/event.h>
#include <glog/logging.h>
#include <cassert>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define ASSERTNOERRL(x, l) { \
    if (x < 0) { \
      LOG(ERROR) << "Assertion failure on line " \
		 << l \
		 << ". Error: " << strerror(errno); \
      exit(1); \
    } \
  }
#define ASSERTNOERR(x) ASSERTNOERRL(x, __LINE__)
  
/*
 * Ask event base to use our logging mehanism.
 */
static void logCB(int sev, const char* msg) {
  switch (sev) {
  case _EVENT_LOG_DEBUG: DLOG(INFO) << msg; break;
  case _EVENT_LOG_WARN: LOG(WARNING) << msg; break;
  case _EVENT_LOG_ERR: LOG(ERROR) << msg; break;
  case _EVENT_LOG_MSG:
  default: LOG(INFO) << msg; break;
  }
}

static void fatalCB(int err) {
  LOG(FATAL) << "Something went wrong. Fatal-ing with error: " << err;
}

static void stdinReadCB(int fd, short what, void* eb) {
  DLOG(INFO) << "stdin is ready for reading";

  if(!cin.good()) {
    LOG(INFO) << "stdin died";
    exit(0);
  }
  
  string inString;
  getline (cin, inString);


  cerr << ">>>>> " << inString << "\n";

  LOG(INFO) << ">>>>> " << inString;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  // Set up the logging callbacks
  DLOG(INFO) << "Setting the callback for logging";
  event_set_log_callback(logCB);
  event_enable_debug_mode(); // enables a bunch of verbosity; compute-intensive so may disable
  event_set_fatal_callback(fatalCB);

  DLOG(INFO) << "Getting the event_base";
  event_base* eb = event_base_new();
  if (!eb) {
   LOG(FATAL) << "Could not create event base";
  }

  int stdinFD = STDIN_FILENO;

  event* stdinRead = event_new(eb, stdinFD, EV_READ | EV_PERSIST, stdinReadCB, eb);
  event_add(stdinRead, NULL);
  event_base_dispatch(eb);
}
