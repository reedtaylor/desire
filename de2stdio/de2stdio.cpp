#define NDEBUG 0
#define NO_DE 0


#define CHARBUF_SIZE 1024

#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <event2/event.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <cassert>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <termios.h>
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

static void stdin_read_CB(int fd, short what, void* arg) {
  DLOG(INFO) << "stdin is readable";

  if(!cin.good()) {
    LOG(INFO) << "stdin died";
    exit(0);
  }
  
  string in_string;
  cin >> in_string;
  
  cerr << ">> STDIN >> " << in_string << "\n";
  VLOG(4) << "stdin:  " << in_string;
}

#if !NO_DE
static void de_read_CB(int fd, short what, void* arg) {
  DLOG(INFO) << "DE serial is readable";

  FILE *de_serial = fdopen(fd, "r+");
  if (!de_serial) {
    LOG(INFO) << "DE serial died";
    exit(0);
  }

  char *in_string = new char[CHARBUF_SIZE];
  if (!fgets(in_string, CHARBUF_SIZE, de_serial)) {
    LOG(INFO) << "DE serial died";
    exit(0);
  }

  cerr << ">>> DE >>>> " << in_string << "\n";
  VLOG(4) << "DE:     " << in_string;
  
  
}
#endif
  


int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Set up the logging callbacks
  DLOG(INFO) << "Passing logging callbacks to eventlib";
  event_set_log_callback(log_CB);
  event_set_fatal_callback(fatal_CB);
  event_enable_debug_mode(); // enables a bunch of verbosity; compute-intensive so may disable

  // Set up the eventlib fundamentals
  DLOG(INFO) << "Generating the event_base for primary reads";
  event_base* eb = event_base_new();
  if (!eb) {
   LOG(FATAL) << "Could not create event base";
  }

#if !NO_DE
  // Connect to the Decent device
  DLOG(INFO) << "Connecting to Decent device at " << FLAGS_decent_device_path;
  FILE *de_serial = fopen(FLAGS_decent_device_path.c_str(), "r+");
  if (!de_serial) {
    LOG(FATAL) << "Could not open DE serial interface " << FLAGS_decent_device_path << " -- Error: " << strerror(errno);
  }
  LOG(INFO) << "Opened DE machine serial file at " << FLAGS_decent_device_path;

  // Setup read callback event for DE serial device
  int de_serial_fd = fileno(de_serial);
  event* de_read = event_new(eb, de_serial_fd, EV_READ | EV_PERSIST, de_read_CB, eb);
  event_add(de_read, NULL);
#endif
  
  // Connect to stdin
  int stdin_fd = STDIN_FILENO;
  event* stdin_read = event_new(eb, stdin_fd, EV_READ | EV_PERSIST, stdin_read_CB, eb);
  event_add(stdin_read, NULL);


  event_base_dispatch(eb);
}
