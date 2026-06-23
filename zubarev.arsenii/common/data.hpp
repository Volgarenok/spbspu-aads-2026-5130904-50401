#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include "list.hpp"
namespace zubarev
{

  struct Data
  {
    std::string name;
    List< size_t > numbers;
  };
}

#endif
