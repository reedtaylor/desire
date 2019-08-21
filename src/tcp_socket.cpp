#include "tcp_socket.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "dispatcher.h"
#include "interfaces/tcp_interface.h"

// Define command line flags (via gflags)
// These can be referenced using FLAGS_[name]
DEFINE_int32(tcp_port, 9090, "Port number for accepting connections");

// todo: add a bool flag for turning tcp on and off

void TcpSocket::Init(Dispatcher *dispatcher_ptr) {

  _dispatcher = dispatcher_ptr;

  // To create a server:
  // 1. Create a socket.
  // 2. Bind it to a particular port.
  // 3. Start listen()-ing.
  // 4. accept() connections. This would block waiting for connections.
  
  // 1. Create a TCP socket
  int opt = 1;
  _socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(_socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR,
	     (char *)&opt, sizeof(opt));

  // todo: assert that both of the above were successful
  LOG(INFO) << "TcpSocket: Sock fd = " << _socket_file_descriptor;
  
  // 2. Bind it to a port.
  const int TIME_SERVICE_PORT = FLAGS_tcp_port;
  const int LISTEN_QUEUE = 100;
  sockaddr_in de_addr;
  bzero(&de_addr, sizeof(struct sockaddr_in));
  de_addr.sin_family = AF_INET;
  de_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  de_addr.sin_port = htons(TIME_SERVICE_PORT);
  bind(_socket_file_descriptor, (sockaddr*)&de_addr, sizeof(de_addr));
  
  // 3. listen()
  LOG(INFO) << "TcpSocket: Going to start listening on socket "
	    << _socket_file_descriptor;
  listen(_socket_file_descriptor, LISTEN_QUEUE);
  // todo: assert success above

}

int TcpSocket::GetFileDescriptor() {
  return _socket_file_descriptor;
}

void TcpSocket::CallBack(int fd,
	      __attribute__((unused)) short what,
	      __attribute__((unused)) void* socket) {
  
  LOG(INFO) << "TcpSocket: The socket " << fd
            << " is ready for reading.";
  int new_sockfd = accept(fd, NULL, 0);
  // todo: assert above is OK

  LOG(INFO) << "Accepted connection with socket fd: " << new_sockfd;

  TcpInterface *tcp_controller = new TcpInterface;
  tcp_controller->Init(((TcpSocket *)socket)->_dispatcher, new_sockfd);
  ((TcpSocket *)socket)->_dispatcher->AddController(tcp_controller);
  
}

