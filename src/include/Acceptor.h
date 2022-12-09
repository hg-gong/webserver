#pragma once
#include <functional>

class EventLoop;
class Socket;
class Channel;
class Acceptor {
 private:
  /* data */
  EventLoop *loop;
  Socket *sock;
  Channel *acceptChannel;
  std::function<void(Socket *)> newConnectionCallback;

 public:
  Acceptor(EventLoop *_loop);
  ~Acceptor();

  void accpetConnection();
  void setNewConnectionCallback(std::function<void(Socket *)>);
};
