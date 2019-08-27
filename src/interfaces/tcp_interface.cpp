#include "interfaces/tcp_interface.h"

//#include <termios.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"

void TcpInterface::Init(Dispatcher *dispatcher_ptr, int file_descriptor) {
  DLOG(INFO) << "TcpInterface: Connecting to fd " << file_descriptor;
  
  _file_handle = fdopen(file_descriptor, "r+");
  if ((file_descriptor < 0) || (_file_handle == NULL)) {
    LOG(FATAL) << "TcpInterface: Could not open " << file_descriptor
	       << " -- Error: " << strerror(errno);
  }
  LOG(INFO) << "TcpInterface: Opened " << file_descriptor;
	    
  return Init(dispatcher_ptr);
}

void TcpInterface::Init(Dispatcher *dispatcher_ptr) {
  // ensure this is OK and prevent direct calls to this function
  CHECK_NOTNULL(_file_handle);

  CHECK_NOTNULL(dispatcher_ptr);
  _dispatcher = dispatcher_ptr;
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


