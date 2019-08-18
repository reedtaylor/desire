#include <glog/logging.h>
#include <gflags/gflags.h>
#include <signal.h>

#include "dispatcher.h"


// SignalHandler -- for catching system signals via signal()
// 
// We want to close cleanly even if the user ctrl-C's on the command
// line (specifically giving glog a chance to flush the logs
// but could also do some interface cleanup here later on).

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
  
  // Initialize dispatcher -- the bulk of the work of setting up desire
  // is done in this Init() function call
  Dispatcher *dispatcher = new Dispatcher();
  dispatcher->Init();

  // Launch the event handler loop -- this call never returns
  // Note that we could even consider moving this into the Dispatcher::Init()
  // call but it seemed more intutitve to have the "busy loop" so to speak
  // be present at the bottom of main()
  dispatcher->RunDispatchLoop();
}
