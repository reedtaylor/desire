#include "stdio_serial/stdio_serial.h"

#include <iostream>
#include <termios.h>
#include <event2/event.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "desire_util.h"
#include "flags.h"
#include "dispatch.h"

/* 
 * callback for the stdio read events
 */
static void stdio_read_CB(__attribute__ ((unused)) int fd,
		       __attribute__ ((unused)) short what,
		       void *desire_state_ptr) {
  
  DesireState *desire_state = (DesireState *)desire_state_ptr;
  
  if (!std::cin.good()) {
    LOG(INFO) << "stdin died";
    exit(0);
  }

  char in_string[CHARBUF_SIZE];
  std::cin >> in_string;

  VLOG(4) << "STDIN:  " << in_string;
  //std::cerr << ">> STDIN >> " << in_string << "\n"; // todo replace this with propagation
  DispatchToDE(in_string, desire_state);
}


void SetupStdioSerial(DesireState *desire_state) {
  DLOG(INFO) << "Connecting to stdin";

  // Setup read callback event for stdin
  event* stdin_read = event_new(desire_state->eb, STDIN_FILENO, EV_READ | EV_PERSIST, stdio_read_CB, desire_state);
  event_add(stdin_read, NULL);
}
