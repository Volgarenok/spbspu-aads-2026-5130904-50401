#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <vector.hpp>

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
    void setName(const std::string & n);
    void setPatronymic(const std::string & p);
    void setGender(const std::string & g);
    bool setBirthDate(const std::string & dateStr, std::string & errorMsg);
    bool setDeathDate(const std::string & dateStr, std::string & errorMsg);
    void setInfo(const std::string & info);

    const Vector< std::string > & getParents() const;
    const Vector< std::string > & getChildren() const;
    const std::string & getSpouse() const;
    size_t getParentsCount() const;
    size_t getChildrenCount() const;
    bool hasSpouse() const;
    bool hasParent(const std::string & id) const;
    bool hasChild(const std::string & id) const;

    void addParent(const std::string & id);
    void removeParent(const std::string & id);
    void addChild(const std::string & id);
    void removeChild(const std::string & id);
    void setSpouse(const std::string & id);
    void clearSpouse();

  private:
    std::string id_;
    std::string surname_;
    std::string name_;
    std::string patronymic_;
    std::string gender_;
    std::string birthDate_;
    std::string deathDate_;
    std::string info_;

    Vector< std::string > parents_;
    Vector< std::string > children_;
    std::string spouseId_;
  };
}

#endif
