#include "Server.h"
#include <unistd.h>
#include <functional>
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"

Server::Server(EventLoop *_loop) : mainReactor(_loop), acceptor(nullptr) {
  acceptor = new Acceptor(mainReactor);
  std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
  acceptor->setNewConnectionCallback(cb);

  int size = std::thread::hardware_concurrency();
  thpool = new ThreadPool(size);
  for (int i = 0; i < size; i++) {
    subReactors.push_back(new EventLoop());
  }

  for (int i = 0; i < size; i++) {
    std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
    thpool->add(sub_loop);
  }
}

Server::~Server() {
  delete acceptor;
  delete thpool;
}

void Server::newConnection(Socket *sock) {
  if (sock->getFd() != -1) {
    int random = sock->getFd() % subReactors.size();
    Connection *conn = new Connection(subReactors[random], sock);
    std::function<void(Socket *)> cb = std::bind(&Server::deleteConnetion, this, std::placeholders::_1);
    conn->SetDeleteConnectionCallback(cb);
    conn->SetOnConnectCallback(on_connect_callback_);
    connections[sock->getFd()] = conn;
  }
}

void Server::deleteConnetion(Socket *sock) {
  int sockfd = sock->getFd();
  auto it = connections.find(sockfd);
  if (it != connections.end()) {
    Connection *conn = connections[sockfd];
    connections.erase(sockfd);
    delete conn;
    conn = nullptr;
  }
}

void Server::OnConnect(std::function<void(Connection *)> fn) { on_connect_callback_ = std::move(fn); }