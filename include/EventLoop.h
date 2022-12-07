#pragma once
#include <functional>

class Epoll;
class Channel;
class EventLoop
{
private:
    /* data */
    Epoll* ep;
    bool quit;
public:
    EventLoop(/* args */);
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);
    
    void addThread(std::function<void()>);
};
