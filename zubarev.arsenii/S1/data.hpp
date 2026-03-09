#ifndef DATA_HPP
#define DATA_HPP
#include "list.hpp"
#include <string>

namespace zubarev
{
  // class List;

  struct Data
  {
    std::string name;
    List< int > numbers;
  };
}

#endif
