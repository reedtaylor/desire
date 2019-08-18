#ifndef _STDIO_INTERFACE_H
#define  _STDIO_INTERFACE_H

#include <gflags/gflags.h>

class Dispatcher; // forward reference
#include "interface.h"

// StdioInterface is a class that derives Interface and specifies it for the
// 2-way ascii on stdio.
//
// In the not too distant future want to handle the following:
// - todo: optionally pretty print (issue #9)
// - todo: optionally sanity check for well formed messages / typos / upcase (issue #10)
// - todo: replicate this but for named pipes which will be a lot more
//         usable for local comms with programs that want to open files (issue #8)
//
// For details on what an Interface class needs/does see that base
// class interface.h directly
class StdioInterface : public Interface {
  
 public:
  StdioInterface() {};
  
  // Attach the _dispatcher pointer
  // (little to no setup is needed for stdio)
  void Init(Dispatcher *dispathcher_ptr) override;
  
  
  // Push a string out onto the UART serial device
  void Send(const std::string message) override;
  
  // Get a string from the UART serial device
  const std::string Recv() override;
  
  // Return "DecentUART" as a human readable name for this interface
  const std::string GetInterfaceName() override;
  
  // take a string from the callback annd pass it to the dispatcher
  // todo: this is dumb and should go away into the base class callback (issue #16)
  void ReadCB() override;

  // return the file descriptor for the UART serial device
  int GetFileDescriptor() override;
};


#endif
