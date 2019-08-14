#define NDEBUG 1

#include <iostream>
#include <cstdio>
#include <vector>
#include <event2/event.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <cassert>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

/*
 * Define some variables that can be overridden
 * on the command line with flags (via gflags)
 */
DEFINE_string(decent_device_path, "/dev/serial0", "Path to serial device where the Decent machine is connected");


/*
 * Set up eventlib to use glog
 */
static void log_CB(int sev, const char* msg) {
  switch (sev) {
  case _EVENT_LOG_DEBUG: DLOG(INFO) << msg; break;
  case _EVENT_LOG_WARN: LOG(WARNING) << msg; break;
  case _EVENT_LOG_ERR: LOG(ERROR) << msg; break;
  case _EVENT_LOG_MSG:
  default: LOG(INFO) << msg; break;
  }
}

static void fatal_CB(int err) {
  LOG(FATAL) << "Something went wrong. Fatal-ing with error: " << err;
}

static void stdin_read_CB(int fd, short what, void* eb) {
  DLOG(INFO) << "stdin is ready for reading";

  if(!cin.good()) {
    LOG(INFO) << "stdin died";
    exit(0);
  }
  
  string in_string;
  getline (cin, in_string);

  cerr << ">>>>> " << in_string << "\n";
  VLOG(4) << "stdin:  " << in_string;
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Set up the logging callbacks
  DLOG(INFO) << "Passing logging callbacks to eventlib";
  event_set_log_callback(log_CB);
  event_set_fatal_callback(fatal_CB);
  //event_enable_debug_mode(); // enables a bunch of verbosity; compute-intensive so may disable

  // Set up the eventlib fundamentals
  DLOG(INFO) << "Generating the event_base for primary reads";
  event_base* eb = event_base_new();
  if (!eb) {
   LOG(FATAL) << "Could not create event base";
  }

  // Connect to the Decent device
  DLOG(INFO) << "Connecting to Decent device at " << FLAGS_decent_device_path;

  int stdin_fd = STDIN_FILENO;
  event* stdinRead = event_new(eb, stdin_fd, EV_READ | EV_PERSIST, stdin_read_CB, eb);
  event_add(stdinRead, NULL);
  event_base_dispatch(eb);
}
