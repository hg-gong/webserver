#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
 private:
  /* data */
  int epfd;
  struct epoll_event* events;

 public:
  Epoll(/* args */);
  ~Epoll();

  void updateChannel(Channel*);
  void deleteChannel(Channel*);

  std::vector<Channel*> pool(int timeout = -1);
};
