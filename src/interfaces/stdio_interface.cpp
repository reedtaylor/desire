#include "interfaces/stdio_interface.h"

#include <termios.h>
#include <iostream>
#include <string>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"

void StdioInterface::Init(Dispatcher *dispatcher_ptr) {
  DLOG(INFO) << "StdioInterface: Connecting to stdin";
  _dispatcher = dispatcher_ptr;
}

void StdioInterface::Send(const std::string message) {
  std::cout << message;
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

void StdioInterface::ReadCB() {
  const std::string in_string = Recv(); 
  CHECK_NOTNULL(_dispatcher);
  _dispatcher->DispatchFromController(in_string, GetInterfaceName());
}

int StdioInterface::GetFileDescriptor() {
  return STDIN_FILENO;
}
