#ifndef _TCP_SOCKET_
#define _TCP_SOCKET_

#include <gflags/gflags.h>

class Dispatcher; // forward reference


// Command line flag: -tcp_port=[int32]
// Allows a user to override the port for listening for TCP connections
// Default = 9090
// (Per best practice see .cpp file for the DEFINE_ call)
DECLARE_int32(tcp_port);


class TcpSocket {

 public:
  TcpSocket() {};
  void Init(Dispatcher *dispatcher_ptr);
  int GetFileDescriptor();
  static void CallBack(__attribute__((unused)) int fd,
		       __attribute__((unused)) short what,
		       __attribute__((unused)) void* socket);

 private:
  Dispatcher *_dispatcher;
  int _socket_file_descriptor;

};


#endif
