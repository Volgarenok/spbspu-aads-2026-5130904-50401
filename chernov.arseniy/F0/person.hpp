#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>

namespace chernov {
  class Person {
  public:
    Person() = default;
    Person(const std::string & id,
      const std::string & surname,
      const std::string & name,
      const std::string & patronymic,
      const std::string & gender);

    const std::string & getId() const;
    const std::string & getSurname() const;
    const std::string & getName() const;
    const std::string & getPatronymic() const;
    const std::string & getGender() const;
    const std::string & getBirthDate() const;
    const std::string & getDeathDate() const;
    const std::string & getInfo() const;

    void setSurname(const std::string & s);
    void setName(const std::string & s);
    void setPatronymic(const std::string & s);
    void setGender(const std::string & g);
    bool setBirthDate(const std::string & dateStr);
    bool setDeathDate(const std::string & dateStr);
    void setInfo(const std::string & info);

  private:
    std::string id_;
    std::string surname_;
    std::string name_;
    std::string patronymic_;
    std::string gender_;
    std::string birthDate_;
    std::string deathDate_;
    std::string info_;
  };
}

#endif
