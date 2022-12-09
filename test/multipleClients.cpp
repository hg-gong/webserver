#include <unistd.h>
#include <cstring>

#include <functional>
#include <iostream>

#include "Buffer.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

void OneClient(int msgs, int wait) {
  std::unique_ptr<Socket> sock = std::make_unique<Socket>();
  std::unique_ptr<InetAddress> addr = std::make_unique<InetAddress>("127.0.0.1", 1234);
  sock->connect(addr.get());
  int sockfd = sock->getFd();
  std::unique_ptr<Buffer> readBuffer = std::make_unique<Buffer>();
  std::unique_ptr<Buffer> sendBuffer = std::make_unique<Buffer>();

  sleep(wait);
  int count = 0;
  while (count < msgs) {
    sendBuffer->setBuf("I'm client");
    ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
    if (write_bytes == -1) {
      printf("socket already disconnected, can't write any more!\n");
      break;
    }
    int already_read = 0;
    char buf[1024];
    while (true) {
      memset(&buf, 0, sizeof(buf));
      ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
      if (read_bytes > 0) {
        readBuffer->append(buf, read_bytes);
        already_read += read_bytes;
      } else if (read_bytes == 0) {
        printf("server disconnected! \n");
        exit(EXIT_SUCCESS);
      }
      if (already_read >= sendBuffer->size()) {
        printf("count: %d, message from server: %s\n", count++, readBuffer->c_str());
        break;
      }
    }
    readBuffer->clear();
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
