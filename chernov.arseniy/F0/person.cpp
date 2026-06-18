#include "person.hpp"
#include "date_utils.hpp"

chernov::Person::Person(const std::string & id,
  const std::string & surname,
  const std::string & name,
  const std::string & patronymic,
  const std::string & gender):
  id_(id),
  surname_(surname == "-" ? "" : surname),
  name_(name == "-" ? "" : name),
  patronymic_(patronymic == "-" ? "" : patronymic),
  gender_(gender == "Male" || gender == "Female" ? gender : "Unknown")
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
  surname_ = (s == "-" ? "" : s);
}

void chernov::Person::setName(const std::string & n)
{
  name_ = (n == "-" ? "" : n);
}

void chernov::Person::setPatronymic(const std::string & p)
{
  patronymic_ = (p == "-" ? "" : p);
}

void chernov::Person::setGender(const std::string & g)
{
  if (g == "Male" || g == "Female" || g == "Unknown") {
    gender_ = g;
  }
}

bool chernov::Person::setBirthDate(const std::string & dateStr, std::string & errorMsg)
{
  if (dateStr.empty() || dateStr == "-") {
    birthDate_.clear();
    return true;
  }
  if (!isValidDate(dateStr)) {
    errorMsg = "invalid date";
    return false;
  }
  std::string storage = toStorageFormat(dateStr);
  if (!deathDate_.empty() && deathDate_ < storage) {
    errorMsg = "birth date after death";
    return false;
  }
  birthDate_ = storage;
  return true;
}

bool chernov::Person::setDeathDate(const std::string & dateStr, std::string & errorMsg)
{
  if (dateStr.empty() || dateStr == "-") {
    deathDate_.clear();
    return true;
  }
  if (!isValidDate(dateStr)) {
    errorMsg = "invalid date";
    return false;
  }
  std::string storage = toStorageFormat(dateStr);
  if (!birthDate_.empty() && storage < birthDate_) {
    errorMsg = "death date before birth";
    return false;
  }
  deathDate_ = storage;
  return true;
}

void chernov::Person::setInfo(const std::string & info)
{
  info_ = info;
}

const chernov::Vector< std::string > & chernov::Person::getParents() const
{
  return parents_;
}
const chernov::Vector< std::string > & chernov::Person::getChildren() const
{
  return children_;
}
const chernov::Vector< std::string > & chernov::Person::getSpouses() const
{
  return spouses_;
}

size_t chernov::Person::getParentsCount() const
{
  return parents_.getSize();
}
size_t chernov::Person::getChildrenCount() const
{
  return children_.getSize();
}
size_t chernov::Person::getSpousesCount() const
{
  return spouses_.getSize();
}

bool chernov::Person::hasParent(const std::string & id) const
{
  for (size_t i = 0; i < parents_.getSize(); ++i) {
    if (parents_[i] == id) {
      return true;
    }
  }
  return false;
}

bool chernov::Person::hasChild(const std::string & id) const
{
  for (size_t i = 0; i < children_.getSize(); ++i) {
    if (children_[i] == id) {
      return true;
    }
  }
  return false;
}

bool chernov::Person::hasSpouse(const std::string & id) const
{
  for (size_t i = 0; i < spouses_.getSize(); ++i) {
    if (spouses_[i] == id) {
      return true;
    }
  }
  return false;
}

void chernov::Person::addParent(const std::string & id)
{
  if (!hasParent(id)) {
    parents_.pushBack(id);
  }
}

void chernov::Person::removeParent(const std::string & id)
{
  for (size_t i = 0; i < parents_.getSize(); ++i) {
    if (parents_[i] == id) {
      parents_.erase(i);
      break;
    }
  }
}

void chernov::Person::addChild(const std::string & id)
{
  if (!hasChild(id)) {
    children_.pushBack(id);
  }
}

void chernov::Person::removeChild(const std::string & id)
{
  for (size_t i = 0; i < children_.getSize(); ++i) {
    if (children_[i] == id) {
      children_.erase(i);
      break;
    }
  }
}

void chernov::Person::addSpouse(const std::string & id)
{
  if (!hasSpouse(id)) {
    spouses_.pushBack(id);
  }
}

void chernov::Person::removeSpouse(const std::string & id)
{
  for (size_t i = 0; i < spouses_.getSize(); ++i) {
    if (spouses_[i] == id) {
      spouses_.erase(i);
      break;
    }
  }
}
