#include "dispatch.h"

#include <string>
#include <iostream>
#include <glog/logging.h>

#include "desire_util.h"

int DispatchFromDE(std::string message,
		   __attribute__ ((unused)) DesireState *desire_state) {

  int dispatch_count = 0;
  
  // Dispatch to stdio
  std::cout << message;
  dispatch_count++;

  return dispatch_count;
}

int DispatchToDE(std::string message, DesireState *desire_state) {
  int dispatch_count = 0;

  fputs(message.c_str(), desire_state->de_serial);
  fputc('\n', desire_state->de_serial);
  LOG(INFO) << "stdin wrote '" << message << "' to DE";
  dispatch_count++;
  
  return dispatch_count;
} 


