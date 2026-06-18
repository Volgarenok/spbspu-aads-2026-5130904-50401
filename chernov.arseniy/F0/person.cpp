#include "person.hpp"
#include "date_utils.hpp"

chernov::Person::Person(const std::string & id,
  const std::string & surname,
  const std::string & name,
  const std::string & patronymic,
  const std::string & gender):
  id_(id),
  surname_(surname),
  name_(name),
  patronymic_(patronymic),
  gender_(gender)
{}

const std::string & chernov::Person::getId() const
{
  return id_;
}

const std::string & chernov::Person::getSurname() const
{
  return surname_;
}

const std::string & chernov::Person::getName() const
{
  return name_;
}

const std::string & chernov::Person::getPatronymic() const
{
  return patronymic_;
}

const std::string & chernov::Person::getGender() const
{
  return gender_;
}

const std::string & chernov::Person::getBirthDate() const
{
  return birthDate_;
}

const std::string & chernov::Person::getDeathDate() const
{
  return deathDate_;
}

const std::string & chernov::Person::getInfo() const
{
  return info_;
}

void chernov::Person::setSurname(const std::string & s)
{
  surname_ = (s == "-") ? "" : s;
}

void chernov::Person::setName(const std::string & s)
{
  name_ = (s == "-") ? "" : s;
}

void chernov::Person::setPatronymic(const std::string & s)
{
  patronymic_ = (s == "-") ? "" : s;
}

void chernov::Person::setGender(const std::string & g)
{
  if (g == "Male" || g == "Female" || g == "Unknown") {
    gender_ = g;
  }
}

bool chernov::Person::setBirthDate(const std::string & dateStr)
{
  if (dateStr.empty() || dateStr == "-") {
    birthDate_.clear();
    return true;
  }
  if (!isValidDate(dateStr)) {
    return false;
  }
  birthDate_ = toStorageFormat(dateStr);
  return true;
}

bool chernov::Person::setDeathDate(const std::string & dateStr)
{
  if (dateStr.empty() || dateStr == "-") {
    deathDate_.clear();
    return true;
  }
  if (!isValidDate(dateStr)) {
    return false;
  }
  deathDate_ = toStorageFormat(dateStr);
  return true;
}

void chernov::Person::setInfo(const std::string & info)
{
  info_ = info;
}
