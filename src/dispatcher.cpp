#include "dispatcher.h"

#include <glog/logging.h>

#include "interface.h"

void Dispatcher::Init() {
  LOG(INFO) << "Initializing Dispatcher";

  _event_base = new EventBase();
  _event_base->Init();

  _decent_uart = new DecentUart();
  _decent_uart->Init(this);
  AddReadEventForInterface(_decent_uart);

  // this may want to move out into main later on, not sure yet
  StdioInterface *stdio_interface = new StdioInterface();
  stdio_interface->Init(this);
  AddController(stdio_interface);
}


void Dispatcher::DispatchFromDE(const std::string message) {
  for (auto& interface: _interfaces) { // fancy c++11 shit
    interface->Send(message);
    VLOG(4) << "Dispatcher: Sent " << message << " to " << interface->GetInterfaceName();
  }
}

void Dispatcher::DispatchFromController(const std::string message,
					const std::string interface_name) {
  _decent_uart->Send(message);
  VLOG(4) << "Dispatcher: Sent " << message << " to " << _decent_uart->GetInterfaceName() << " from " << interface_name;
}

void Dispatcher::AddController(Interface *new_controller) {
  _interfaces.push_back(new_controller);

  AddReadEventForInterface(new_controller);

  LOG(INFO) << "Dispatcher: Added new controller " << new_controller->GetInterfaceName();
}

void Dispatcher::AddReadEventForInterface(Interface *interface) {
  event* new_event = event_new(_event_base->eb,
			       interface->GetFileDescriptor(),
			       EV_READ | EV_PERSIST,
			       Interface::CallBack,
			       interface);
  event_add(new_event, NULL);
  LOG(INFO) << "Dispatcher: Added read event for controller " << interface->GetInterfaceName();

}

void Dispatcher::RunDispatchLoop() {
  event_base_dispatch(_event_base->eb);
}

