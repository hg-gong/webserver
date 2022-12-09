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
  auto ploop = std::make_unique<EventLoop>();
  auto pserver = std::make_unique<Server>(ploop.get());
  // implement echo server
  pserver->OnConnect([](Connection *conn) {
    conn->Read();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      return;
    }
    std::cout << "Message from client" << conn->GetSocket()->GetFd() << ": "
              << conn->ReadBuffer() << std::endl;
    conn->SetSendBuffer(conn->ReadBuffer());
    conn->Write();
  });

  ploop->loop();
  return 0;
}
