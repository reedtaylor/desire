#include "interfaces/tcp_interface.h"

//#include <termios.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"

void TcpInterface::Init(__attribute__((unused)) Dispatcher *dispatcher_ptr) {
  // this should never happen but we are obliged to implement this method
  LOG(FATAL) << "TcpInterface: Called Init() with missing port";
}


void TcpInterface::Init(Dispatcher *dispatcher_ptr, int file_descriptor) {
  DLOG(INFO) << "TcpInterface: Connecting to fd " << file_descriptor;

  // todo: move this into the base Init override and assert that _file_handle is not null there
  _dispatcher = dispatcher_ptr;
  
  _file_handle = fdopen(file_descriptor, "r+");
 
  if (file_descriptor < 0) {
    // todo: verify this is a valid way to assess a good file open
    LOG(FATAL) << "TcpInterface: Could not open "
	       << file_descriptor
	       << " -- Error: " << strerror(errno);
  }
  LOG(INFO) << "TcpInterface: Opened "
	    << file_descriptor;
}

void TcpInterface::Send(const std::string message) {
  fputs(message.c_str(), _file_handle);
  fflush(_file_handle);
  //  fputc('\n', _file_handle);
}

const std::string TcpInterface::Recv() {
  char in_string[_TCP_CHARBUF_SIZE];
  if (!fgets(in_string, _TCP_CHARBUF_SIZE, _file_handle)) {
    LOG(INFO) << "TcpInterface: interface died: " << GetInterfaceName();
    fclose(_file_handle);
    _dispatcher->RemoveAndFreeController(this);
  }

  const std::string message(in_string);
  return message;
}

const std::string TcpInterface::GetInterfaceName() {
  const std::string name("TcpInterface fd " + std::to_string(GetFileDescriptor()));
  return name;
}

int TcpInterface::GetFileDescriptor() {
  return fileno(_file_handle);
}


