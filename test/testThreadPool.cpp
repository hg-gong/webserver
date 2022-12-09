#include <iostream>
#include <memory>
#include <string>
#include "ThreadPool.h"

void print(int a, double b, const char *c, std::string d) { std::cout << a << b << c << d << std::endl; }

void test() { std::cout << "hellp" << std::endl; }

int main(int argc, char const *argv[]) {
  /* code */
  std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>(4);
  std::function<void()> func = std::bind(print, 1, 3.14, "hello", std::string("world"));
  pool->add(func);
  func = test;
  pool->add(func);
  return 0;
}
