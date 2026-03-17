#ifndef FUNC_STACK_QUEUE_HPP
#define FUNC_STACK_QUEUE_HPP
#include "all.hpp"
#include <iosfwd>
namespace zubarev
{
  std::string input(std::istream& in, bool& error);
  Queue<std::string> fromStrToQueue(std::string str);
}

#endif
