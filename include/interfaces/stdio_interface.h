#ifndef _STDIO_INTERFACE_H
#define  _STDIO_INTERFACE_H

#include <gflags/gflags.h>

#include "dispatcher.h"
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
  
  
  // Push a string to stdout
  void Send(const std::string message) override;
  
  // Get a string from stdin
  const std::string Recv() override;
  
  // Return "stdio" as a human readable interface name
  const std::string GetInterfaceName() override;
  
  // return the file descriptor
  int GetFileDescriptor() override;
};


#endif
