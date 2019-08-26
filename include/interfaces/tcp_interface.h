#ifndef _TCP_INTERFACE_H
#define  _TCP_INTERFACE_H

#include <gflags/gflags.h>

#include "dispatcher.h"
#include "interface.h"

// For reasons we read into a fixed size char[] array.  This is that size.
#define _TCP_CHARBUF_SIZE 1024


// TcpInterface is a class that derives Interface and specifies it for
// connections over a TCP port.
//
// For details on what an Interface class needs/does see that base
// class interface.h directly
class TcpInterface : public Interface {
  
public:
  TcpInterface() {};
  ~TcpInterface() {};

  // Initialize the TCP interface and attach the _dispatcher pointer
  // also stash a pointer to the filehandle in _file_handle
  // for use by Recv() and Send().
  // do not call Init(Dispatcher *) directly; we need the fd
  void Init(Dispatcher *dispatcher_ptr, int file_descriptor);
  void Init(Dispatcher *dispatcher_ptr) override;

  // Push a string out onto the TCP device
  void Send(const std::string message) override;

  // Get a string from the TCP device
  const std::string Recv() override;

  // Return "TcpInterface" as a human readable name for this interface
  const std::string GetInterfaceName() override;

  // return the file descriptor for the XXX serial device
  int GetFileDescriptor() override;
  
protected:
  // this is a place to hang onto the file stream
  // for use by Recv() and Send()
  FILE *_file_handle;

};


#endif
