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

#include "desire_util.h"
#include "events.h"
#include "de_serial/de_serial.h"

using namespace std;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Struct for passing around key pointers and other state
  DesireState *desire_state = new DesireState;

  // Generate the Event Base from eventlib
  desire_state->eb = SetupEventBase();

  // Initialize decent serial port
  SetupDESerial(desire_state);  

  // launch the event handler loop
  event_base_dispatch(desire_state->eb);
}
