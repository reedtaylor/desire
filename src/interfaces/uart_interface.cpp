#include "interfaces/uart_interface.h"

#include <termios.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"
#include "string_util.h"


int UartInterface::Init(Dispatcher *dispatcher_ptr, const std::string filename, const std::string controller_name) {
  _controller_name = controller_name;
  return Init(dispatcher_ptr, filename);
}

int UartInterface::Init(Dispatcher *dispatcher_ptr, const std::string filename) {
  if (_controller_name.empty()) {
    _controller_name = filename;
    DLOG(INFO) << "UartInterface: Connecting to " << filename;
  } else {
    DLOG(INFO) << "UartInterface: Connecting controller " << _controller_name << " to file " << filename;
  }

  _file_handle = fopen(filename.c_str(), "r+");
  if (_file_handle == NULL) {
    LOG(FATAL) << "UartInterface: Could not connect to " << filename
		<< " -- Error: " << strerror(errno);
    return -1;
  } 

  // apply settings for the serial port (115200 8N1)
  int file_descriptor = fileno(_file_handle);
  struct termios settings;
  tcgetattr(file_descriptor, &settings);
  speed_t baud = B115200; /* baud rate */
  cfsetospeed(&settings, baud); /* baud rate */
  settings.c_cflag &= ~PARENB; /* no parity */
  settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
  settings.c_cflag &= ~CSIZE;
  settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
  settings.c_lflag = ICANON; /* canonical mode */
  settings.c_oflag &= ~OPOST; /* raw output */
  tcsetattr(file_descriptor, TCSANOW, &settings); /* apply the settings */
  tcflush(file_descriptor, TCOFLUSH);

  return Init(dispatcher_ptr);
}

int UartInterface::Init(Dispatcher *dispatcher_ptr) {
  // this function should not be called directly
  CHECK_NOTNULL(_file_handle);

  CHECK_NOTNULL(dispatcher_ptr);
  _dispatcher = dispatcher_ptr;
  return 1;
}

int UartInterface::Send(const std::string message) {
  std::string strip_message = trim(message);
  int sent_str = fputs(strip_message.c_str(), _file_handle);
  int sent_eol = fputc('\n', _file_handle);
  int flushed = fflush(_file_handle);
  if ((sent_str < 0) || (sent_eol < 0) || (flushed == EOF)) {
    LOG(INFO) << "UartInterface: Controller " << GetInterfaceName()
	      << " write failed -- Error: " << strerror(errno);
    return -1;
  }
    
  return 1;
}

const std::string UartInterface::Recv() {
  char in_string[_UART_CHARBUF_SIZE] = "";
  if (!fgets(in_string, _TCP_CHARBUF_SIZE, _file_handle)) {
    LOG(INFO) << "TcpInterface: Controller TCP " << GetInterfaceName()
	      << " read failed -- Error: " << strerror(errno);
    return std::string("");
  }

  const std::string message(in_string);
  DCHECK(message.length() > 0); // we are using zero length to mean bad read
  return message;
}

const std::string UartInterface::GetInterfaceName() {
  const std::string name = _controller_name;
  return name;
}

int UartInterface::GetFileDescriptor() {
  return fileno(_file_handle);
}
