#include <iostream>
#include <cstdio>
#include <vector>
#include <event2/event.h>
#include <glog/logging.h>
#include <cassert>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

#define ASSERTNOERRL(x, l) { \
    if (x < 0) { \
      LOG(ERROR) << "Assertion failure on line " \
		 << l \
		 << ". Error: " << strerror(errno); \
      exit(1); \
    } \
  }
#define ASSERTNOERR(x) ASSERTNOERRL(x, __LINE__)
  
/*
 * This would ask event base to use our logging mehanism.
 */
static void logCob(int sev, const char* msg) {
  switch (sev) {
  case _EVENT_LOG_DEBUG: DLOG(INFO) << msg; break;
  case _EVENT_LOG_WARN: LOG(WARNING) << msg; break;
  case _EVENT_LOG_ERR: LOG(ERROR) << msg; break;
  case _EVENT_LOG_MSG:
  default: LOG(INFO) << msg; break;
  }
}

static void fatalCob(int err) {
  LOG(FATAL) << "Something went wrong. Fatal-ing with error: " << err;
}

const int kStrArrayLen = 3;
const std::string kStrArray[] = { "foo", "bar", "baz" };
unsigned int chosenStr = 0;

static void pickFortuneCookieCob(evutil_socket_t fd, short what, void* args) {
  DLOG(INFO) << "Choosing a new fortune cookie.";
  chosenStr = (chosenStr + 1) % kStrArrayLen;
}

static void respondWithCookie(evutil_socket_t fd, short what, void* args) {
  DLOG(INFO) << "About to respond with the current fortune cookie.";
  char str[100];
  sprintf(str, "%s", kStrArray[chosenStr].c_str());
  write(fd, str, strlen(str));
  close(fd);
}

static void acceptConnCob(evutil_socket_t fd, short what, void* eb) {
  LOG(INFO) << "The socket " << fd
            << " is ready for reading.";
  int new_sockfd = accept(fd, NULL, 0);
  ASSERTNOERR(new_sockfd);
  DLOG(INFO) << "Accepted connection with sockfd: " << new_sockfd;
  event_add(event_new((event_base*)eb, new_sockfd, EV_READ, respondWithCookie, NULL), NULL);
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  // Set up the logging callback and enable debug logging
  DLOG(INFO) << "Setting the cb";
  event_set_log_callback(logCob);
  event_enable_debug_mode();
  event_set_fatal_callback(fatalCob);

  DLOG(INFO) << "Getting the event_base";
  event_base* eb = event_base_new();
  if (!eb) {
    LOG(FATAL) << "Could not create event base";
  }
  
  event* e = event_new(eb, -1, EV_TIMEOUT | EV_PERSIST, pickFortuneCookieCob, NULL);
  timeval twoSec = {2, 0};
  event_add(e, &twoSec);
  
  /*
   * To create a server:
   * 1. Create a socket.
   * 2. Bind it to a particular port.
   * 3. Start listen()-ing.
   * 4. accept() connections. This would block waiting for connections.
   */
  
  // 1. Create a TCP socket
  int fortuneSockFd, opt = 1;
  fortuneSockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  ASSERTNOERR(fortuneSockFd);
  LOG(INFO) << "Sock fd: " << fortuneSockFd;
  ASSERTNOERR(setsockopt(fortuneSockFd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) > 0);
  
  // 2. Bind it to a port.
  const int TIME_SERVICE_PORT = 9090;
  const int LISTEN_QUEUE = 100;
  sockaddr_in fortuneSrvAddr;
  bzero(&fortuneSrvAddr, sizeof(struct sockaddr_in));
  fortuneSrvAddr.sin_family = AF_INET;
  fortuneSrvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  fortuneSrvAddr.sin_port = htons(TIME_SERVICE_PORT);
  bind(fortuneSockFd, (sockaddr*)&fortuneSrvAddr, sizeof(fortuneSrvAddr));
  
  // 3. listen()
  LOG(INFO) << "Going to start listening.";
  ASSERTNOERR(listen(fortuneSockFd, LISTEN_QUEUE));

  event* t = event_new(eb, fortuneSockFd, EV_READ | EV_PERSIST, acceptConnCob, eb);
  event_add(t, NULL);
  event_base_dispatch(eb);
}
