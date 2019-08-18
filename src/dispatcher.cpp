#include "dispatcher.h"

#include <glog/logging.h>

#include "interface.h"
#include "tcp_socket.h"

void Dispatcher::Init() {
  LOG(INFO) << "Dispatcher: Initializing";

  _event_base = new EventBase();
  _event_base->Init();

  _decent_uart = new DecentUart();
  _decent_uart->Init(this);
  AddReadEventForInterface(_decent_uart); // todo: assert this worked

  // For now we are adding stdio controller here inside Dispatcher::Init()
  // may want to move out into main later on? not sure yet where
  // we should be generating most of the interfaces, which will be
  // in part governed by command line stuff.
  // seems better to keep it in here for now since I think new TCP
  // connections will be getting added inside this class, hence this will
  // be the one place we are calling AddController() from.
  StdioInterface *stdio_interface = new StdioInterface();
  stdio_interface->Init(this);
  AddController(stdio_interface);

  TcpSocket *tcp_socket = new TcpSocket();
  tcp_socket->Init(this);
  AddReadEventForTcpSocket(tcp_socket);
}


void Dispatcher::DispatchFromDE(const std::string message) {
  for (auto& interface: _controllers) { // fancy c++11 iterator shit
    interface->Send(message);
    VLOG(4) << "Dispatcher: >>>> Sent " << message << " to " << interface->GetInterfaceName();
  }
}

void Dispatcher::DispatchFromController(const std::string message,
					const std::string interface_name) {
  _decent_uart->Send(message);
  VLOG(3) << "Dispatcher: <<<< Sent " << message << " to " << _decent_uart->GetInterfaceName() << " from " << interface_name;
}

void Dispatcher::AddController(Interface *new_controller) {
  // assert that new_controller is initialized and has a way to
  // communicate back to a dispatcher (presumably this one)
  CHECK_NOTNULL(new_controller->_dispatcher);

  // append the new controller onto the vector of controllers
  _controllers.push_back(new_controller);

  // establish the EventBase listener for the new controller
  AddReadEventForInterface(new_controller);

  // todo: checks/assertions that the above were each successful
  
  LOG(INFO) << "Dispatcher: Added new controller " << new_controller->GetInterfaceName();
}

void Dispatcher::AddReadEventForInterface(Interface *interface) {
  CHECK_NOTNULL(_event_base);
  CHECK_NOTNULL(_event_base->eb);
  // todo: may want to add some health checks for the event base here beyond
  // just non-null pointer
  

  // this is the way you add a read event to the event_base as specified
  // by libevent.
  //
  // Quick reference on these params:
  // 1. a pointer to the actial libevent event_base
  // 2. file descriptor for the interface (required!)
  // 3. EV_READ = listen for read events
  //    EV_PERSIST = don't stop listening when you get each read event
  // 4. static callback wrapper function (implemented as a utility function in
  //    the Interface base class)
  // 5. a pointer to the Interface object being listened.  We are passing
  //    this via a void* pointer, this is what allows the static callback
  //    wrapper implemented in the Interface base class to know which
  //    actual interface inheritied class should act on the callback at
  //    runtime.  (preventing us from needing to write N instances of
  //    this AddReadEvent... function for N different flavors of interface)
  event* new_event = event_new(_event_base->eb, 
			       interface->GetFileDescriptor(),
			       EV_READ | EV_PERSIST,
			       Interface::CallBack,
			       interface);
  event_add(new_event, NULL);

  LOG(INFO) << "Dispatcher: Added read event for controller " << interface->GetInterfaceName();
}


void Dispatcher::AddReadEventForTcpSocket(TcpSocket *socket) {
  CHECK_NOTNULL(_event_base);
  CHECK_NOTNULL(_event_base->eb);

  event* new_event = event_new(_event_base->eb, 
			       socket->GetFileDescriptor(),
			       EV_READ | EV_PERSIST,
			       TcpSocket::CallBack,
			       socket);
  event_add(new_event, NULL);

  LOG(INFO) << "Dispatcher: Added read event for TCP Socket fd " << socket->GetFileDescriptor();
  
}


void Dispatcher::RunDispatchLoop() {
  event_base_dispatch(_event_base->eb);
}

