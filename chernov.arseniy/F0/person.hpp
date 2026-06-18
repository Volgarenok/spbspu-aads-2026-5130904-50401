#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>

namespace chernov {
  class Person {
  public:
    Person() = default;
    explicit Person(const std::string & id);
    const std::string & getId() const;

  private:
    std::string id_;
  };
}

#endif
