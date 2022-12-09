#pragma once
#include <cassert>
#include <stdexcept>
#define OS_LINUX

#define ASSERT(expr, message) assert((expr) && (message))

#define UNREACHABLE(message) throw std::logic_error(message)
