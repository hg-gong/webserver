#include <string.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include "Buffer.h"
#include "Socket.h"
#include "util.h"

int main(int argc, char const *argv[]) {
  /* code */
  std::unique_ptr<Socket> psock = std::make_unique<Socket>();
  std::unique_ptr<InetAddress> paddr =
      std::make_unique<InetAddress>("127.0.0.1", 1234);
  psock->connect(paddr.get());
  int sockfd = psock->getFd();
  std::unique_ptr<Buffer> sendBuffer = std::make_unique<Buffer>();
  std::unique_ptr<Buffer> readBuffer = std::make_unique<Buffer>();

  while (true) {
    sendBuffer->getline();
    ssize_t write_bytes =
        write(sockfd, sendBuffer->c_str(), sendBuffer->size());
    if (write_bytes == -1) {
      printf("socket already disconnected, can't write any more!\n");
      break;
    }
    int already_read = 0;
    char buf[1024];  //这个buf大小无所谓
    while (true) {
      bzero(&buf, sizeof(buf));
      ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
      if (read_bytes > 0) {
        readBuffer->append(buf, read_bytes);
        already_read += read_bytes;
      } else if (read_bytes == 0) {  // EOF
        printf("server disconnected!\n");
        exit(EXIT_SUCCESS);
      }
      if (already_read >= sendBuffer->size()) {
        printf("message from server: %s\n", readBuffer->c_str());
        break;
      }
    }
    readBuffer->clear();
  }

  return 0;
}
