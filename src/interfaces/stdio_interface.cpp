#include "interfaces/stdio_interface.h"

#include <termios.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"

//#include <iostream>
//#include <event2/event.h>

//#include "desire_util.h"
//#include "dispatch.h"

// #include "stdio_serial/stdio_serial.h"

// #include <iostream>
// #include <termios.h>
// #include <event2/event.h>
// #include <glog/logging.h>
// #include <gflags/gflags.h>

// #include "desire_util.h"
// #include "flags.h"
// #include "dispatch.h"


StdioInterface::Init() {
  DLOG(INFO) << "StdioInterface: Connecting to stdin";
}

void StdioInterface::Send(const std::string message) {
  std::cout << message;
};

const std::string StdioInterface::Recv() {
  if (!std::cin.good()) {
    LOG(INFO) << "StdioInterface: stdin closed; exiting";
    exit(0);
  }

  const std::string message;
  std::cin >> message;
  return message;
};

const std::string DecentUart::GetInterfaceName() {
  const std::string name("stdin");
  return name;
}

void StdioInterface::ReadCB(int fd, short what, void *dispatcher_ptr) {
  Dispatcher dispatcher = *(Dispatcher *)dispatcher_ptr;

  const std::string in_string = Recv(); 
  dispatcher.DispatchFromController(in_string, GetInterfaceName());
};

int StdioInterface::GetFileDescriptor() {
  return STDIN_FILENO;
}
