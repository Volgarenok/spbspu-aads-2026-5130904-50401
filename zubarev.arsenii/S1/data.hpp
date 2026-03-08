#ifndef DATA_HPP
#define DATA_HPP

#include <string>

template < class T >
class List;

struct Data
{
  std::string name;
  List< int >* numbers;
};

#endif
