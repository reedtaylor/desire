#include "dispatcher.h"

#include <glog/logging.h>
#include <iterator>
#include <algorithm>

#include "interface.h"
#include "tcp_socket.h"
#include "string_util.h"
#include "parser/parser.h"


// Define command line flags (via gflags)
// These can be referenced using FLAGS_[name]
DEFINE_string(de1_device_path, "/dev/ttySC0", "Path to serial device where the Decent machine is connected");

DEFINE_string(decent_ble_device_path, "/dev/ttySC1", "Path to serial device where the Decent BLE adaptor is connected");
DEFINE_bool(use_decent_ble, true, "Enable a to serial device for the Decent BLE adaptor");

DEFINE_bool(use_stdio_controller, false, "Enable stdio control for the Decent BLE adaptor");

void Dispatcher::Init() {
  LOG(INFO) << "Dispatcher: Initializing";

  // Set up the event_base
  _event_base = new EventBase();
  _event_base->Init();

  // Instantiate the parser
  _parser = new Parser();
  
  // Add the special interface for the DE (note, adds a read event directly
  // rather than calling AddController since this is not a controller)
  _de1_uart = new UartInterface();
  _de1_uart->Init(this, FLAGS_de1_device_path.c_str(), std::string(DE1_MACHINE_NAME));
  AddReadEventForInterface(_de1_uart); // todo: verify this worked

  // Add the Decent BLE adaptor via UART
  if (FLAGS_use_decent_ble) {
    UartInterface *ble_uart = new UartInterface();
    ble_uart->Init(this, FLAGS_decent_ble_device_path.c_str(), std::string(DECENT_BLE_NAME));
    AddController(ble_uart);
  }

  
  // For now we are adding stdio controller here inside Dispatcher::Init()
  // may want to move out into main later on? not sure yet where
  // we should be generating most of the interfaces, which will be
  // in part governed by command line stuff.
  // seems better to keep it in here for now since I think new TCP
  // connections will be getting added inside this class, hence this will
  // be the one place we are calling AddController() from.
  if (FLAGS_use_stdio_controller) {
    StdioInterface *stdio_interface = new StdioInterface();
    stdio_interface->Init(this);
    AddController(stdio_interface);
  }
  
  TcpSocket *tcp_socket = new TcpSocket();
  tcp_socket->Init(this);
  AddReadEventForTcpSocket(tcp_socket);

}


void Dispatcher::DispatchFromDE(const std::string message) {
  if (!_parser->IsWellFormed(message)) {
    LOG(WARNING) << "Dispatcher got badly-formed message from DE: " << trim(message);
  } else {
    // Parse / Unpack some messages for logging
    U16 command = _parser->GetCommand(message);
    LOG(INFO) << "Hello command " << (char)command;
    switch (command) {
    case ((U16) 'N'):
      //T_StateInfo I_StateInfo;
      break;
    case ((U16) 'Q'):
      T_WaterLevels I_WaterLevels;
      std::string hexString = _parser->GetHexString(message);

      LOG(INFO) << "Hexstring: " << hexString;

      _parser->Unpack((U8 *)(&I_WaterLevels.Level), hexString, sizeof(I_WaterLevels.Level));
      hexString.erase(0, sizeof(I_WaterLevels.Level));
      _parser->Unpack((U8 *)(&I_WaterLevels.StartFillLevel), hexString, sizeof(I_WaterLevels.StartFillLevel));

      LOG(INFO) << "Hex: " << std::hex << I_WaterLevels.Level.value;
      LOG(INFO) << "DE1 Water Level: " << (I_WaterLevels.Level.value>>8) + (double)(I_WaterLevels.Level.value & 0xFF)/(1<<8);
      break;
    }
  }
  
  for (auto& interface: _controllers) { // fancy c++11 iterator shit
    int sent = interface->Send(message);
    if (sent < 0) {
      LOG(INFO) << "Dispatcher: Send to " << interface->GetInterfaceName() << " failed; freeing.";
      RemoveAndFreeController(interface);
      return;
    }
    VLOG(4) << "Dispatcher:  >>> Sent " << trim(message) << " to " << interface->GetInterfaceName();
  }
}

void Dispatcher::DispatchFromController(const std::string message,
					const std::string interface_name) {
  _de1_uart->Send(message);
  VLOG(3) << "Dispatcher: <<<  Sent " << trim(message) << " to " << _de1_uart->GetInterfaceName() << " from " << interface_name;
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

void Dispatcher::RemoveAndFreeController(Interface *old_controller) {

  std::vector<Interface *>::iterator to_erase =
    std::find(_controllers.begin(), _controllers.end(), old_controller);

  // And then erase if found
  if (to_erase != _controllers.end()){
    _controllers.erase(to_erase);
  } else {
    LOG(WARNING) << "Failed to remove controller from vector: " << old_controller;
  }

  // remove the EventBase listener for the new controller
  RemoveReadEventForInterface(old_controller);

  // controllers are created in the dispatcher; the dispatcher taketh away
  std::string dead_name = old_controller->GetInterfaceName();
  delete old_controller;
  LOG(INFO) << "Dispatcher: Removed controller " << dead_name;
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
			       Dispatcher::CallBack,
			       interface);
  event_add(new_event, NULL);
  interface->_event = new_event;

  LOG(INFO) << "Dispatcher: Added read event for interface " << interface->GetInterfaceName();
}

void Dispatcher::RemoveReadEventForInterface(Interface *interface) {
  event_free(interface->_event);
  LOG(INFO) << "Dispatcher: Removed read event for TCP Socket fd " << interface->GetFileDescriptor();
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

void Dispatcher::CallBack(__attribute__((unused)) int fd,
			  __attribute__((unused)) short what,
			  void* interface_ptr) {
  Interface *interface = (Interface *)interface_ptr;
  const std::string in_string = interface->Recv();

  // empty string means bad read
  if(in_string.length() == 0) {
    LOG(INFO) << "Dispatcher: Interface " << interface->GetInterfaceName()
	      << " read failed.  Closing & freeing";
    interface->_dispatcher->RemoveAndFreeController(interface);
    return;
  }
  if (interface->GetInterfaceName() == DE1_MACHINE_NAME) {
    interface->_dispatcher->DispatchFromDE(in_string);
  } else {
    interface->_dispatcher->DispatchFromController(in_string, interface->GetInterfaceName());
  }
}

