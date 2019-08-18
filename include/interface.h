#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <glog/logging.h>

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

  // Initialize the interface, performing any needed setup
  // Remember: it is vital for all nonabstract Interface inheritor classes
  // to set the _dispatcher pointer (from this base class) to refer to
  // the Dispatcher* pointer (handed to this function)
  virtual void Init(Dispatcher *) = 0;

  // Convert a std::string into whatever form is needed and send it out
  // on the real interface
  virtual void Send(const std::string /* message */) = 0;

  // Pull a single (likely newline terminated) message off the interface
  // and return it as a const std::string
  virtual const std::string Recv() = 0;

  // Return a human-readable name for this interface  
  virtual const std::string GetInterfaceName()= 0;

  // Return an integer file descriptor for this interface (needed by
  // libevent which is written in C and works on fd's)
  virtual int GetFileDescriptor() = 0;

  // virtual callback function for the interface. When an interface
  // gets this function call, what will typically need to happen is:
  // 1. The interface calls Recv() on itself obtaining a message
  // 2. The interface passes that message to _dispatcher->DispatchFromController
  //    so that the dispatcher can pass the message along to the DE
  //
  // like this:
  //  void MyInterface::ReadCB() {
  //    const std::string in_string = Recv(); 
  //    CHECK_NOTNULL(_dispatcher);
  //    _dispatcher->DispatchFromDE(in_string);
  //  }
  //
  // todo: this is stupid and I should just inline the above into the
  // static callback and make the virtual call to Recv() since it's the
  // only actually polymorphic thing here.  then there would be no need
  // for the virtual callback function at all
  virtual void ReadCB() = 0;

  // This static utility function is used to wrap the virtual callback
  // implemented above.  Cannot / should not be overridden.
  // The three weird fields are just what is specified by libevent,
  // the third one being the most interesting since it ends up being
  // a pointer to the actial object that is firing the read callback.
  // (See Dispatcher::AddReadEventForInterface for where this gets
  // set up.)
  // todo: rewrite this commentif we move the addreadevent call around
  static void CallBack(__attribute__((unused)) int fd,
		       __attribute__((unused)) short what,
		       __attribute__((unused)) void* interface) {
    ((Interface *)interface)->ReadCB();
  };

  // a pointer to the dispatcher that added this interface to the
  // event_base.  Required in order for read callbacks (above) to
  // be able to pass the received messages to the DE
  Dispatcher *_dispatcher;
};


#endif
