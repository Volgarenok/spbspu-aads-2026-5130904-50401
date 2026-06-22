#ifndef CAFE_HPP
#define CAFE_HPP
#include <string>
#include "menu.hpp"
#include "queue.hpp"

namespace zharov
{
  struct CafeSystem
  {
    Menus menus;
    Queues queues;
    std::string history_file;
  };
}
#endif
