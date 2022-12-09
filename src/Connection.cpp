#include "Connection.h"

#include <string.h>
#include <unistd.h>
#include <iostream>
#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"

Connection::Connection(EventLoop* _loop, Socket* _sock)
    : loop(_loop), sock(_sock) {
  if (loop != nullptr) {
    channel = new Channel(loop, sock->getFd());
    channel->enableRead();
    channel->useET();
  }
  read_buffer_ = new Buffer();
  send_buffer_ = new Buffer();
  state_ = State::Connected;
}

Connection::~Connection() {
  if (loop != nullptr) {
    delete channel;
  }
  delete sock;
  delete read_buffer_;
  delete send_buffer_;
}

void Connection::Read() {
  ASSERT(state_ == State::Connected, "connection state is disconnected!");
  read_buffer_->clear();
  if (sock->IsNonBlocking()) {
    ReadNonBlocking();
  } else {
    ReadBlocking();
  }
}
void Connection::Write() {
  ASSERT(state_ == State::Connected, "connection state is disconnected!");
  if (sock->IsNonBlocking()) {
    WriteNonBlocking();
  } else {
    WriteBlocking();
  }
  send_buffer_->clear();
}

void Connection::ReadNonBlocking() {
  int sockfd = sock->getFd();
  char buf[1024];
  while (true) {
    memset(buf, 0, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      read_buffer_->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {  // 程序正常中断,继续读取
      printf("continue reading\n");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) |
                (errno ==
                 EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完
      break;
    } else if (bytes_read == 0) {  // EOF,客户端断开链接
      printf("EOF, client fd %d disconnected\n", sockfd);
      state_ = State::Closed;
      break;
    } else {
      printf("Connection reset by peer\n");
      state_ = State::Closed;
      break;
    }
  }
}

void Connection::WriteNonBlocking() {
  int sockfd = sock->getFd();
  char buf[send_buffer_->size()];
  memcpy(buf, send_buffer_->c_str(), send_buffer_->size());
  int data_size = send_buffer_->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EINTR) {
      printf("continue writing\n");
      continue;
    }
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    if (bytes_write == -1) {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
      break;
    }
    data_left -= bytes_write;
  }
}

/**
 * @brief Never used by server, only for client
 *
 */
void Connection::ReadBlocking() {
  int sockfd = sock->getFd();
  unsigned int rcv_size = 0;
  socklen_t len = sizeof(rcv_size);
  getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
  char buf[rcv_size];
  ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
  if (bytes_read > 0) {
    read_buffer_->append(buf, bytes_read);
  } else if (bytes_read == 0) {
    printf("read EOF, blocking client fd %d disconnected\n", sockfd);
    state_ = State::Closed;
  } else if (bytes_read == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}

/**
 * @brief Never used by server, only for client
 *
 */
void Connection::WriteBlocking() {
  // 没有处理send_buffer_数据大于TCP写缓冲区，的情况，可能会有bug
  int sockfd = sock->getFd();
  ssize_t bytes_write =
      write(sockfd, send_buffer_->c_str(), send_buffer_->size());
  if (bytes_write == -1) {
    printf("Other error on blocking client fd %d\n", sockfd);
    state_ = State::Closed;
  }
}

void Connection::Close() { deleteConnectionCallback(sock); }

Connection::State Connection::GetState() { return state_; }
void Connection::SetSendBuffer(const char* str) { send_buffer_->setBuf(str); }
Buffer* Connection::GetReadBuffer() { return read_buffer_; }
const char* Connection::ReadBuffer() { return read_buffer_->c_str(); }
Buffer* Connection::GetSendBuffer() { return send_buffer_; }
const char* Connection::SendBuffer() { return send_buffer_->c_str(); }

void Connection::SetDeleteConnectionCallback(
    std::function<void(Socket*)> callback) {
  deleteConnectionCallback = callback;
}

void Connection::SetOnConnectCallback(
    std::function<void(Connection*)> callback) {
  onConnectCallback = callback;
  channel->setReadCallback([this]() { onConnectCallback(this); });
}

void Connection::GetlineSendBuffer() { send_buffer_->getline(); }

Socket* Connection::GetSocket() { return sock; }