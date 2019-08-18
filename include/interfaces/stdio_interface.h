#ifndef _STDIO_INTERFACE_H
#define  _STDIO_INTERFACE_H

#include <gflags/gflags.h>

#include "interface.h"
class Dispatcher; // forward reference

class StdioInterface : public Interface {

 public:
  StdioInterface() {};

  void Init(Dispatcher *dispathcher_ptr) override;
  void Send(const std::string message) override;
  const std::string Recv() override;
  const std::string GetInterfaceName() override;
  
  void ReadCB() override;
  int GetFileDescriptor() override;
};


#endif
