#include <iostream>
#include <event2/event.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <cassert>
#include <signal.h>

#include "desire_util.h"
#include "events.h"
#include "de_serial/de_serial.h"
#include "stdio_serial/stdio_serial.h"

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

  // Generate & configure the Event Base from eventlib
  SetupEventBase(desire_state);

  // Initialize decent serial port and add to event loop
  SetupDESerial(desire_state);  

  // Initialize stdio serial interface and add to event loop
  SetupStdioSerial(desire_state);  

  // launch the event handler loop
  event_base_dispatch(desire_state->eb);
}
