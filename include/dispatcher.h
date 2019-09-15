#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include <string>
#include <vector>

#include "interface.h"
#include "interfaces/uart_interface.h"
#include "interfaces/stdio_interface.h"
#include "tcp_socket.h"
#include "interfaces/tcp_interface.h"
#include "event_base.h"
#include "parser/parser.h"



// Command line flag: -de1_device_path=[string]
//   Allows a user to override the path to the Decent UART device.
DECLARE_string(de1_device_path);
#define DE1_MACHINE_NAME "DE1"

// Command line flags:
// -decent_ble_device_path=[string]
//   Allows a user to override the path to the Decent BLE UART device.
// -use_decent_ble=true
//   Enable / disable Decent's BLE adaptor over UART
DECLARE_string(decent_ble_device_path);
DECLARE_bool(use_decent_ble);
#define DECENT_BLE_NAME "BLE"

// Command line flags:
// -use_decent_ble=true
//   Enable / disable control via stdio
DECLARE_bool(use_stdio_controller);
#define DECENT_BLE_NAME "BLE"


// Dispatcher -- this class is the heart of the desire system functioning
//  - Responsible for setting up the eventLib event_base
//  - Responsible for initializing and hooking to all interfaces
//  - [soon] Responsible for handling TCP connections
//  - Reponsible for passing messages to/from controllers / Decent machine

class Dispatcher {

 public:

  // Responsible for the bulk of system setup including:
  //  - Initializing the event_base
  //  - Initializing all individual interfaces including the DE machine
  //  - Placing all controller interfaces onto the _controllers vector
  //  - Adding Read events for each interface to the event_base
  void Init();

  // Takes a string that was received from the DE
  // interface and propagates it to all controllers in the _controllers
  // vector
  void DispatchFromDE(const std::string message);

  // Takes a string that was received from any controller and
  // propagates it to the DE interface
  void DispatchFromController(const std::string message,
			      const std::string interface_name);

  // Takes an initialized controller interface and:
  //  - places it onto the _controllers vector (to receive messages
  //    from the DE
  //  - creates a Read event on the EventBase for the interface
  //
  // NOTE: new_controller->Init() must have been called prior to this step
  //
  // This may be better as a private member function but leaving this
  // public for now as main() or someone else might hypothetically want
  // to add controllers to the dispatcher, and we'll allow for that for now
  void AddController(Interface *new_controller);

  // Remove a controller interface from all vectors,
  // delete it from event_base, and free it
  void RemoveAndFreeController(Interface *old_controler);
  
  // Wrapper function around the eventLib event_new() call that
  // places an individual read event on the event_base
  //
  // todo: either make this a protected/private member function, or move
  // it into the EventBase wrapper class as it doesn't totally make
  // sense to be implemented here
  void AddReadEventForInterface(Interface *interface);

  // Corresponding remove call, note this does not
  // deallocate any memory
  void RemoveReadEventForInterface(Interface *interface);

  
  // Wrapper function around the eventLib event_new() call that
  // places an individual read event on the event_base (for a tcp socket
  // rather than a controller)
  //
  // todo: either make this a protected/private member function, or move
  // it into the EventBase wrapper class as it doesn't totally make
  // sense to be implemented here
  void AddReadEventForTcpSocket(TcpSocket *socket);

  
  // Wrapper function around the event_base_dispatch call.  This has the
  // effect of kicking off the "busy loop" at the heart of the system
  // listening for events, and never returns (<-- oversimplified but basically
  // right)
  //
  // This could in principle be called from Init() and marked protected/
  // private, or migrated into the EventBase class.  But for now we call
  // this from main() and given that, this is the right place
  void RunDispatchLoop();

  static void CallBack(__attribute__((unused)) int fd,
		       __attribute__((unused)) short what,
		       void* interface);
 
 private:

  // For now we keep all controller interfaces on this one vector
  // and everything the DE says gets broadcast out to everyone.
  // Later may want to monitor individual interfaces/controllers
  // for "subscribe" messages and be more nuanced about how we
  // broadcast messages.  But the DE itself doesn't (superficially?) appear
  // to heed unsubscribe messages anyway so this is low priority for now
  std::vector<Interface *> _controllers;

  // We keep the interface for the DE itself in this special place (i.e.
  // even though it inherits from Interface it is NOT supposed to be
  // on the _controllers vector.  Instead it is here.
  UartInterface *_de1_uart;

  // Event lib wrapper class instance
  EventBase *_event_base;

  // Parser class instance
  Parser *_parser;
};


#endif
