#include "interfaces/tcp_interface.h"

#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"
#include "string_util.h"

int TcpInterface::Init(Dispatcher *dispatcher_ptr, int file_descriptor) {
  DLOG(INFO) << "TcpInterface: Connecting to fd " << file_descriptor;
  
  _file_handle = fdopen(file_descriptor, "r+");
  if ((file_descriptor < 0) || (_file_handle == NULL)) {
    LOG(FATAL) << "TcpInterface: Could not open " << file_descriptor
	       << " -- Error: " << strerror(errno);
    return -1;
  }
  LOG(INFO) << "TcpInterface: Opened " << file_descriptor;
	    
  return Init(dispatcher_ptr);
}

int TcpInterface::Init(Dispatcher *dispatcher_ptr) {
  // ensure this is OK and prevent direct calls to this function
  CHECK_NOTNULL(_file_handle);

  CHECK_NOTNULL(dispatcher_ptr);
  _dispatcher = dispatcher_ptr;
  return 1;
}

int TcpInterface::Send(const std::string message) {
  std::string strip_message = trim(message);
  int sent_str = fputs(strip_message.c_str(), _file_handle);
  int sent_eol = fputc('\n', _file_handle);
  int flushed = fflush(_file_handle);
  if ((sent_str < 0) || (sent_eol < 0) || (flushed == EOF)) {
    LOG(INFO) << "TcpInterface: Controller TCP " << GetInterfaceName()
	      << " write failed -- Error: " << strerror(errno);
    return -1;
  }
    
  return 1;
}

const std::string TcpInterface::Recv() {
  char in_string[_TCP_CHARBUF_SIZE] = "";
  fgets(in_string, _TCP_CHARBUF_SIZE, _file_handle);
  if (feof(_file_handle) || ferror(_file_handle)) {
    if (strlen(in_string) > 0) {
      LOG(WARNING) << "TcpInterface: Controller " << GetInterfaceName()
		<< " read failed; input buffer '" << in_string
		<< "' not newline-terminated. Dropping message.";
    }
    return std::string("");   // empty string signals dead interface
  }

  const std::string message(in_string);
  return message;
}

const std::string TcpInterface::GetInterfaceName() {
  const std::string name("TCP:" + std::to_string(GetFileDescriptor()));
  return name;
}

int TcpInterface::GetFileDescriptor() {
  return fileno(_file_handle);
}


