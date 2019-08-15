#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <event2/event.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <cassert>
#include <signal.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "desire_util.h"
#include "events.h"
#include "de_serial/de_serial.h"

using namespace std;

void SignalHandler(int s){
  LOG(INFO) << "Caught signal " << s << " -- flushing and exiting";
  google::FlushLogFiles(google::INFO);
  exit(1); 
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Catch SIGINT so we can flush the logs
  signal (SIGINT,SignalHandler);
  
  // Struct for passing around key pointers and other state
  DesireState *desire_state = new DesireState;

  // Generate the Event Base from eventlib
  desire_state->eb = SetupEventBase();

  // Initialize decent serial port
  SetupDESerial(desire_state);  

  // launch the event handler loop
  event_base_dispatch(desire_state->eb);
}
