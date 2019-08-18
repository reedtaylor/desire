//#include <iostream>
#include <glog/logging.h>
#include <gflags/gflags.h>
//#include <cassert>
#include <signal.h>

//#include "desire_util.h"
//#include "events.h"
//#include "de_serial/de_serial.h"
//#include "stdio_serial/stdio_serial.h"
#include "dispatcher.h"

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
  
  // Initialize dispatcher
  Dispatcher *dispatcher = new Dispatcher();
  dispatcher->Init();

  // launch the event handler loop
  dispatcher->RunDispatchLoop();
}
