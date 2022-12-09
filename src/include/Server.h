#pragma once
#include <functional>
#include <map>
#include <vector>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server {
 private:
  /* data */
  EventLoop* mainReactor;
  Acceptor* acceptor;
  std::map<int, Connection*> connections;
  std::vector<EventLoop*> subReactors;
  ThreadPool* thpool;
  std::function<void(Connection*)> on_connect_callback_;

 public:
  Server(EventLoop*);
  ~Server();

  void newConnection(Socket* sock);
  void deleteConnetion(Socket* sock);
  void OnConnect(std::function<void(Connection*)> fn);
};
