#include "person.hpp"

chernov::Person::Person(const std::string & id):
  id_(id)
{}

const std::string & chernov::Person::getId() const
{
  return id_;
}
