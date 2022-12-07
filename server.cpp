#include "Server.h"
#include <unistd.h>
#include <functional>
#include <memory>
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"

int main(int argc, char const *argv[]) {
  std::unique_ptr<EventLoop> ploop = std::make_unique<EventLoop>();
  std::unique_ptr<Server> pserver = std::make_unique<Server>(ploop.get());
  ploop->loop();
  return 0;
}
