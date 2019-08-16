#include "de_serial/de_serial.h"

#include <iostream>
#include <termios.h>
#include <event2/event.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "desire_util.h"
#include "dispatch.h"


/*
 * Define command line flags (via gflags)
 * These can be referenced using FLAGS_[name]
 */
DEFINE_string(decent_device_path, "/dev/serial0", "Path to serial device where the Decent machine is connected");


/* 
 * callback for the DE serial port read events
 */
static void de_read_CB(__attribute__ ((unused)) int fd,
		       __attribute__ ((unused)) short what,
		       void *desire_state_ptr) {

  DesireState *desire_state = (DesireState *)desire_state_ptr;
  FILE *de_serial = desire_state->de_serial;

  if (!de_serial) {
    LOG(INFO) << "DE serial died";
    exit(0);
  }

  char in_string[CHARBUF_SIZE];
  if (!fgets(in_string, CHARBUF_SIZE, de_serial)) {
    LOG(INFO) << "DE serial died";
    exit(0);
  }

  VLOG(4) << "DE:     " << in_string;
  //std::cerr << ">>> DE >>>> " << in_string << "\n"; // todo replace this with propagation
  DispatchFromDE(in_string, desire_state);
}


void SetupDESerial(DesireState *desire_state) {
  DLOG(INFO) << "Connecting to Decent device at " << FLAGS_decent_device_path;

  desire_state->de_serial = fopen(FLAGS_decent_device_path.c_str(), "r+");
  int de_serial_fd = fileno(desire_state->de_serial);

  if (de_serial_fd < 0) {
    LOG(FATAL) << "Could not open DE serial interface " << FLAGS_decent_device_path << " -- Error: " << strerror(errno);
  }
  LOG(INFO) << "Opened DE machine serial interface at " << FLAGS_decent_device_path;

  // apply settings for the serial port (115200 8N1)
  struct termios settings;
  tcgetattr(de_serial_fd, &settings);
  speed_t baud = B115200; /* baud rate */
  cfsetospeed(&settings, baud); /* baud rate */
  settings.c_cflag &= ~PARENB; /* no parity */
  settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
  settings.c_cflag &= ~CSIZE;
  settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
  settings.c_lflag = ICANON; /* canonical mode */
  settings.c_oflag &= ~OPOST; /* raw output */
  tcsetattr(de_serial_fd, TCSANOW, &settings); /* apply the settings */
  tcflush(de_serial_fd, TCOFLUSH);
  
  // Setup read callback event for DE serial device
  event* de_read = event_new(desire_state->eb, de_serial_fd, EV_READ | EV_PERSIST, de_read_CB, desire_state);
  event_add(de_read, NULL);
}
