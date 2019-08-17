#ifndef _INTERFACE_H
#define _INTERFACE_H

class Interface {

 public:
  virtual void Init() {};
  virtual void Send(const std::string /* message */) {};
  virtual const std::string Recv() {};
  virtual const std::string GetInterfaceName() {};

  virtual void ReadCB(int fd, short what, void *dispatcher_ptr) {};
  virtual int GetFileDescriptor();
};


#endif
