#ifndef _DECENT_UART_H
#define  _DECENT_UART_H

#include <gflags/gflags.h>

class DecentUart; // forward reference to avoid cyclic dependency
#include "dispatcher.h"
#include "interface.h"

#define DE1_MACHINE_NAME "DE1"

// For reasons we read into a fixed size char[] array.  This is that size.
#define _UART_CHARBUF_SIZE 1024


// Command line flag: -decent_device_path=[string]
// Allows a user to override the path to the Decent UART device.
// Default = /dev/serial0
// (Per best practice see .cpp file for the DEFINE_ call)
DECLARE_string(decent_device_path);

// DecentUART is a class that derives Interface and specifies it for the
// UART on the back of the DE1 / DE1PRO.  How to handle the pinout etc.
// is documented elsewhere.
//
// For details on what an Interface class needs/does see that base
// class interface.h directly
class DecentUart : public Interface {
  
public:
  DecentUart() {};

  // Initialize the UART (115200 8N1)
  // and attach the _dispatcher pointer
  // also stash a pointer to the filehandle in _file_handle
  // for use by Recv() and Send()
  void Init(Dispatcher *dispatcher_ptr) override;

  // Push a string out onto the UART serial device
  void Send(const std::string message) override;

  // Get a string from the UART serial device
  const std::string Recv() override;

  // Return "DecentUART" as a human readable name for this interface
  const std::string GetInterfaceName() override;

  // return the file descriptor for the UART serial device
  int GetFileDescriptor() override;
  
protected:
  // this is a place to hang onto the file stream
  // for use by Recv() and Send()
  FILE *_file_handle;

};


#endif
