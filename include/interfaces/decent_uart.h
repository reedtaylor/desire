#ifndef _DECENT_UART_H
#define  _DECENT_UART_H

#include <gflags/gflags.h>

#include "interface.h"
#include "listenable.h"

/*
 * Declare command line flags (via gflags)
 * These can be referenced using FLAGS_[name]
 * DEFINE_ call is in the .cpp source 
 */
DECLARE_string(decent_device_path);

class DecentUart : public Interface {

public:
  DecentUart() {};
  
private:
  FILE *_file_handle;

};


#endif
