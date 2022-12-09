/**
 * @file ThreadPool.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

/**
 * @brief
 *
 */
class ThreadPool {
 private:
  /* data */
  std::mutex m;
  std::condition_variable cv;
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  int size_;
  bool stop;

 public:
  ThreadPool(int size = std::thread::hardware_concurrency());
  ~ThreadPool();

  template <class F, class... Args>
  auto add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

template <class F, class... Args>
auto ThreadPool::add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
  // 定义函数或函数对象f的返回类型
  using return_type = typename std::result_of<F(Args...)>::type;

  // 使用std::make_shared创建一个std::packaged_task对象，该对象包装函数或函数对象f和它的参数args
  // 因为bind了，所以不需要填参数，所以packged_task中写的是<return_type()>
  // packaged_task包装过后的对象可以一部执行
  auto task =
      std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  // 使用task->get_future()获取task的std::future对象
  std::future<return_type> res = task->get_future();

  // 使用unique_lock加锁并在临界区内添加新任务
  {
    std::unique_lock<std::mutex> lock(m);
    // 如果线程池停止运行，则抛出异常
    if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
    // 将task包装在一个lambda函数中并添加到任务队列中
    tasks.emplace([task]() { (*task)(); });
  }

  // 通知一个等待的线程
  cv.notify_one();

  // 当异步执行完成返回std::future对象
  return res;
}
