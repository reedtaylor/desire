#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include <string>
#include <vector>

#include "interface.h"

class Dispatcher {

 public:
  void Init();
  void DispatchFromDE(const std::string message);
  void DispatchFromController(const std::string message,
			      const std::string interface_name);
  void AddController(Interface *new_controller);
  void AddReadEventForInterface(Interface *interface);
  void RunDispatchLoop();

 private:
  std::vector<Interface *> _interfaces;

  DecentUart *_decent_uart;
  EventBase *_event_base;
};


#endif
