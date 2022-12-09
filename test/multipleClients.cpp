#include <unistd.h>
#include <cstring>

#include <functional>
#include <iostream>

#include "Buffer.h"
#include "Connection.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

void OneClient(int msgs, int wait) {
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
    std::cout << "Message from server: " << pconn->ReadBuffer() << std::endl;
  }
}

int main(int argc, char *const argv[]) {
  int threads = 100;
  int msgs = 10000;
  int wait = 0;
  int o = -1;
  const char *optstring = "t:m:w:";
  while ((o = getopt(argc, argv, optstring)) != -1) {
    switch (o) {
      case 't':
        threads = std::stoi(optarg);
        break;
      case 'm':
        msgs = std::stoi(optarg);
        break;
      case 'w':
        wait = std::stoi(optarg);
        break;
      case '?':
        printf("error optopt: %c\n", optopt);
        printf("error opterr: %d\n", opterr);
        break;
      default:
        break;
    }
  }

  ThreadPool *poll = new ThreadPool(threads);
  std::function<void()> func = std::bind(OneClient, msgs, wait);
  for (int i = 0; i < threads; ++i) {
    poll->add(func);
  }
  delete poll;
  return 0;
}
