#pragma once
#include <functional>
class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection {
 public:
  enum State {
    Invalid = 1,
    Handshaking,
    Connected,
    Closed,
    Failed,
  };

  Connection(EventLoop *_loop, Socket *_sock);
  ~Connection();

  void Read();
  void Write();

  void SetDeleteConnectionCallback(std::function<void(Socket *)> callback);
  void SetOnConnectCallback(std::function<void(Connection *)> callback);
  State GetState();
  void Close();
  void SetSendBuffer(const char *str);
  Buffer *GetReadBuffer();
  const char *ReadBuffer();
  Buffer *GetSendBuffer();
  const char *SendBuffer();
  void GetlineSendBuffer();
  Socket *GetSocket();

  void OnConnect(std::function<void()> fn);

 private:
  /* data */
  EventLoop *loop;
  Socket *sock;
  Channel *channel{nullptr};
  State state_{State::Invalid};
  Buffer *read_buffer_{nullptr};
  Buffer *send_buffer_{nullptr};

  std::function<void(Connection *)> onConnectCallback;
  std::function<void(Socket *)> deleteConnectionCallback;

  void ReadNonBlocking();
  void WriteNonBlocking();
  void ReadBlocking();
  void WriteBlocking();
};
