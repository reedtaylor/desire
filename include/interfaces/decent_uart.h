#ifndef _DECENT_UART_H
#define  _DECENT_UART_H

#include <gflags/gflags.h>

#include "interface.h"
class Dispatcher; // forward reference

#define _UART_CHARBUF_SIZE 1024

/*
 * Declare command line flags (via gflags)
 * These can be referenced using FLAGS_[name]
 * DEFINE_ call is in the .cpp source 
 */
DECLARE_string(decent_device_path);

class DecentUart : public Interface {
  
public:
  DecentUart() {};

  void Init(Dispatcher *dispatcher_ptr) override;
  void Send(const std::string message) override;
  const std::string Recv() override;
  const std::string GetInterfaceName() override;
  
  void ReadCB() override;
  int GetFileDescriptor() override;
  
private:
  FILE *_file_handle;

};


#endif
