#ifndef TEST_HPP
#define TEST_HPP
#include <string>
#include <utility>

struct TestNoDefault
{
  int val_;
  std::string str_;

  TestNoDefault(int val, std::string str):
    val_(val),
    str_(str)
  {}
};

#endif
