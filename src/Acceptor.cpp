#include "Acceptor.h"
#include "Channel.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop* _loop)
    : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
  sock = new Socket();
  InetAddress* addr = new InetAddress("127.0.0.1", 1234);
  sock->bind(addr);
  sock->listen();
  acceptChannel = new Channel(_loop, sock->getFd());
  std::function(void()) cb = std::bind(&Acceptor::accpetConnection, this);
  acceptChannel->setReadCallback(cb);
  acceptCHannel->enableRead();
  delete addr;
}

Acceptor::~Acceptor() {
  delete sock;
  delete acceptChannel;
}

void Acceptor::accpetConnection() {
  InetAddress* clnt_addr = new InetAddress();
  Socket* clnt_sock = new Socket(sock->accpet(clnt_addr));
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(),
         clnt_addr->getIp(), clnt_addr->getPort());
  clnt_sock->setnonblocking();
  newConnectionCallback(clnt_sock);
  delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb) {
  newConnectionCallback = _cb;
}
