#include "interfaces/stdio_interface.h"

#include <termios.h>
#include <iostream>
#include <string>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"

int StdioInterface::Init(Dispatcher *dispatcher_ptr) {
  DLOG(INFO) << "StdioInterface: Connecting to stdin";
  _dispatcher = dispatcher_ptr;
  return 1;
}

int StdioInterface::Send(const std::string message) {
  std::cout << message;
  if (std::cout.good()) {
    return 1;
  } else {
    return -1;
  }
}

const std::string StdioInterface::Recv() {
  if (!std::cin.good()) {
    LOG(INFO) << "StdioInterface: stdin closed; exiting";
    exit(0);
  }

  std::string message;
  std::cin >> message;
  const std::string cmessage = message;
  return cmessage;
}

const std::string StdioInterface::GetInterfaceName() {
  const std::string name("stdin");
  return name;
}

int StdioInterface::GetFileDescriptor() {
  return STDIN_FILENO;
}
