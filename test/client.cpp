#include <string.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "util.h"

int main(int argc, char const *argv[]) {
  /* code */
  auto psock = std::make_unique<Socket>();
  auto paddr = std::make_unique<InetAddress>("127.0.0.1", 1234);
  psock->connect(paddr.get());
  auto pconn = std::make_unique<Connection>(nullptr, psock.get());

  while (true) {
    pconn->GetlineSendBuffer();
    pconn->Write();
    if (pconn->GetState() == Connection::State::Closed) {
      pconn->Close();
      break;
    }
    pconn->Read();
    std::cout << "Message from server" << pconn->ReadBuffer() << std::endl;
  }
  return 0;
}
