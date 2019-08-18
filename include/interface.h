#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <glog/logging.h>

class Dispatcher; // forward declaration

class Interface {

 public:
  virtual void Init(Dispatcher *) = 0;
  virtual void Send(const std::string /* message */) = 0;
  virtual const std::string Recv() = 0;

  virtual const std::string GetInterfaceName()= 0;

  virtual int GetFileDescriptor() = 0;
  virtual void ReadCB() = 0;

  static void CallBack(__attribute__((unused)) int fd,
		       __attribute__((unused)) short what,
		       __attribute__((unused)) void* interface) {
    ((Interface *)interface)->ReadCB();
  };

  Dispatcher *_dispatcher;
};


#endif
