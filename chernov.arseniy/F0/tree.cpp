#include "tree.hpp"
#include <iostream>
#include "date_utils.hpp"

chernov::Tree::Tree(const std::string & name, const std::string & description):
  name_(name),
  description_(description),
  idToIndex_(16),
  nextId_(1)
{}

const std::string & chernov::Tree::getName() const
{
  return name_;
}

const std::string & chernov::Tree::getDescription() const
{
  return description_;
}

void chernov::Tree::setDescription(const std::string & desc)
{
  description_ = desc;
}

size_t chernov::Tree::personCount() const
{
  return persons_.getSize();
}

std::string chernov::Tree::generateId()
{
  std::string num = std::to_string(nextId_);
  while (num.size() < 4) {
    num = "0" + num;
  }
  ++nextId_;
  return "P" + num;
}

std::string chernov::Tree::addPerson(
  const std::string & surname, const std::string & name, const std::string & patronymic, const std::string & gender)
{
  std::string id = generateId();
  Person person(id, surname, name, patronymic, gender);
  persons_.pushBack(person);
  size_t index = persons_.getSize() - 1;
  idToIndex_.add(id, index);
  return id;
}

bool chernov::Tree::deletePerson(const std::string & id)
{
  if (!idToIndex_.has(id)) {
    return false;
  }
  size_t index = idToIndex_.at(id);
  removeAllConnections(id);
  persons_.erase(index);
  idToIndex_.remove(id);
  for (size_t i = index; i < persons_.getSize(); ++i) {
    idToIndex_.at(persons_[i].getId()) = i;
  }
  return true;
}

chernov::Person * chernov::Tree::findPerson(const std::string & id)
{
  if (!idToIndex_.has(id)) {
    return nullptr;
  }
  return &persons_[idToIndex_.at(id)];
}

const chernov::Person * chernov::Tree::findPerson(const std::string & id) const
{
  if (!idToIndex_.has(id)) {
    return nullptr;
  }
  return &persons_[idToIndex_.at(id)];
}

chernov::Vector< std::string > chernov::Tree::searchPerson(const std::string & field, const std::string & value) const
{
  Vector< std::string > result;
  for (size_t i = 0; i < persons_.getSize(); ++i) {
    const Person & p = persons_[i];
    bool match = false;
    if (field == "surname") {
      match = (p.getSurname() == value);
    } else if (field == "name") {
      match = (p.getName() == value);
    } else if (field == "patronymic") {
      match = (p.getPatronymic() == value);
    } else if (field == "gender") {
      match = (p.getGender() == value);
    } else if (field == "birthDate") {
      std::string storeVal = (value.empty() || value == "-") ? "" : toStorageFormat(value);
      match = (p.getBirthDate() == storeVal);
    } else if (field == "deathDate") {
      std::string storeVal = (value.empty() || value == "-") ? "" : toStorageFormat(value);
      match = (p.getDeathDate() == storeVal);
    } else if (field == "info") {
      match = (p.getInfo() == value);
    }
    if (match) {
      result.pushBack(p.getId());
    }
  }
  return result;
}

void chernov::Tree::listPersons(std::ostream & out, const std::string & filter) const
{
  out << "<LIST (" << filter << "):>\n";
  for (size_t i = 0; i < persons_.getSize(); ++i) {
    const Person & p = persons_[i];
    bool show = true;
    if (filter == "alive") {
      show = p.getDeathDate().empty();
    } else if (filter == "deceased") {
      show = !p.getDeathDate().empty();
    } else if (filter == "male") {
      show = (p.getGender() == "Male");
    } else if (filter == "female") {
      show = (p.getGender() == "Female");
    }
    if (show) {
      out << "<  " << p.getId() << ": " << p.getSurname() << " " << p.getName() << ">\n";
    }
  }
}

bool chernov::Tree::showPerson(const std::string & id, std::ostream & out) const
{
  const Person * p = findPerson(id);
  if (!p) {
    return false;
  }
  out << "<ID: " << p->getId() << ">\n";
  out << "<SURNAME: " << p->getSurname() << ">\n";
  out << "<NAME: " << p->getName() << ">\n";
  out << "<PATRONYMIC: " << p->getPatronymic() << ">\n";
  out << "<GENDER: " << p->getGender() << ">\n";
  out << "<BIRTHDATE: " << toDisplayFormat(p->getBirthDate()) << ">\n";
  out << "<DEATHDATE: " << toDisplayFormat(p->getDeathDate()) << ">\n";
  out << "<INFO: " << p->getInfo() << ">\n";
  out << "<PARENTS: 0>\n";
  out << "<SPOUSES: 0>\n";
  out << "<CHILDREN: 0>\n";
  return true;
}

bool chernov::Tree::editPerson(
  const std::string & id, const std::string & field, const std::string & value, std::string & errorMsg)
{
  Person * p = findPerson(id);
  if (!p) {
    errorMsg = "person not found";
    return false;
  }

  if (field == "surname") {
    p->setSurname(value);
  } else if (field == "name") {
    p->setName(value);
  } else if (field == "patronymic") {
    p->setPatronymic(value);
  } else if (field == "gender") {
    if (value != "Male" && value != "Female" && value != "Unknown") {
      errorMsg = "invalid gender";
      return false;
    }
    p->setGender(value);
  } else if (field == "birthDate") {
    return p->setBirthDate(value, errorMsg);
  } else if (field == "deathDate") {
    return p->setDeathDate(value, errorMsg);
  } else if (field == "info") {
    p->setInfo(value);
  } else {
    errorMsg = "unknown field";
    return false;
  }
  return true;
}

void chernov::Tree::removeAllConnections(const std::string &)
{}
