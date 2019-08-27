#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <glog/logging.h>
#include <event2/event.h>

// Interface is the base class for all our interfaces that receive and
// transmit decent-style data frames, and that are attached to the
// event_base for listening.
//
// Interface is an abstract class so it must be derived and overridden to
// implement a real world interface of some form.  Each such interface
// must provide the following
//  - An Init() implementation, for initialization (obviously) but
//    ALSO (CRITICALLY) to assign the _dispatcher pointer to refer back
//    to the core dispatcher instance (needed for callbacks)
//  - Send and Recv functions representing Decent data frames as ascii strings
//  - An interface name
//  - A real, oldschool integer file_descriptor (since this is needed by
//    the event_base which is written in C)
//  - A callback function to hand controller packets off to the dispatcher
//
// There is also a static callback wrapper utility function, detailed below
// 


class Dispatcher; // forward declaration

class Interface {

 public:
  virtual ~Interface() = default;
  
  // Initialize the interface, performing any needed setup
  // Remember: it is vital for all nonabstract Interface inheritor classes
  // to set the _dispatcher pointer (from this base class) to refer to
  // the Dispatcher* pointer (handed to this function)
  // returns nonnegative on success, negative on fail
  virtual int Init(Dispatcher *) = 0;

  // Convert a std::string into whatever form is needed and send it out
  // on the real interface
  // returns nonnegative on success, negative on fail
  virtual int Send(const std::string /* message */) = 0;

  // Pull a single (likely newline terminated) message off the interface
  // and return it as a const std::string
  virtual const std::string Recv() = 0;

  // Return a human-readable name for this interface  
  virtual const std::string GetInterfaceName()= 0;

  // Return an integer file descriptor for this interface (needed by
  // libevent which is written in C and works on fd's)
  virtual int GetFileDescriptor() = 0;

  // a pointer to the dispatcher that added this interface to the
  // event_base.  Uses during read callbacks (see Dispatcher::CallBack())
  Dispatcher *_dispatcher = NULL;

  // a pointer to the libevent event_base event for this specific interface
  // used during cleanup to free the event
  event *_event;
};


#endif
