#include "interfaces/decent_uart.h"

#include <termios.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "dispatcher.h"

// Define command line flags (via gflags)
// These can be referenced using FLAGS_[name]
DEFINE_string(decent_device_path, "/dev/serial0", "Path to serial device where the Decent machine is connected");

void DecentUart::Init(Dispatcher *dispatcher_ptr) {
  DLOG(INFO) << "DecentUart: Connecting to " << FLAGS_decent_device_path;

  _dispatcher = dispatcher_ptr;
  
  _file_handle = fopen(FLAGS_decent_device_path.c_str(), "r+");
  int file_descriptor = fileno(_file_handle);

  if (file_descriptor < 0) {
    // todo: verify this is a valid way to assess a good file open
    LOG(FATAL) << "DecentUart: Could not open "
	       << FLAGS_decent_device_path
	       << " -- Error: " << strerror(errno);
  }
  LOG(INFO) << "DecentUart: Opened "
	    << FLAGS_decent_device_path;

  // apply settings for the serial port (115200 8N1)
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
}

void DecentUart::Send(const std::string message) {
  fputs(message.c_str(), _file_handle);
  fputc('\n', _file_handle);
}

const std::string DecentUart::Recv() {
  char in_string[_UART_CHARBUF_SIZE];
  if (!fgets(in_string, _UART_CHARBUF_SIZE, _file_handle)) {
    LOG(FATAL) << "DecentUart: interface died";
    exit(0);
  }

  const std::string message(in_string);
  return message;
}

const std::string DecentUart::GetInterfaceName() {
  const std::string name("DecentUART");
  return name;
}

void DecentUart::ReadCB() {
  const std::string in_string = Recv(); 
  CHECK_NOTNULL(_dispatcher);
  _dispatcher->DispatchFromDE(in_string);
}

int DecentUart::GetFileDescriptor() {
  return fileno(_file_handle);
}


