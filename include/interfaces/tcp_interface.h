#ifndef _TCP_INTERFACE_H
#define  _TCP_INTERFACE_H

#include <gflags/gflags.h>

class Dispatcher; // forward reference
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

  // Initialize the TCP interface and attach the _dispatcher pointer
  // also stash a pointer to the filehandle in _file_handle
  // for use by Recv() and Send().
  //
  // The first (override) version is never called and just throws an
  // error.  But we are required to implement it due to inheriting 
  // from virtual base class Interface.  The alternative would be to force
  // the creator to poke a file_descriptor in before calling Init()
  // which feels like it violates the spirit of an Init() call
  void Init(__attribute__((unused)) Dispatcher *dispatcher_ptr) override;
  void Init(Dispatcher *dispatcher_ptr, int file_descriptor);

  // Push a string out onto the TCP device
  void Send(const std::string message) override;

  // Get a string from the TCP device
  const std::string Recv() override;

  // Return "TcpInterface" as a human readable name for this interface
  const std::string GetInterfaceName() override;

  // take a string from the callback annd pass it to the dispatcher
  // todo: this is dumb and should go away into the base class callback
  void ReadCB() override;

  // return the file descriptor for the XXX serial device
  int GetFileDescriptor() override;
  
protected:
  // this is a place to hang onto the file stream
  // for use by Recv() and Send()
  FILE *_file_handle;

};


#endif
