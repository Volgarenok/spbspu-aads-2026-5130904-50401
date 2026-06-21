#include "tree.hpp"
#include <iostream>
#include "date_utils.hpp"
#include "relationship.hpp"

chernov::Tree::Tree(const std::string & name, const std::string & description):
  name_(name),
  description_(description),
  idToIndex_(16),
  nextId_(1)
{}

void chernov::Tree::swap(Tree & other) noexcept
{
  std::swap(name_, other.name_);
  std::swap(description_, other.description_);
  std::swap(persons_, other.persons_);
  std::swap(idToIndex_, other.idToIndex_);
  std::swap(nextId_, other.nextId_);
}

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
  out << "<PARENTS: " << p->getParentsCount();
  for (size_t i = 0; i < p->getParentsCount(); ++i) {
    out << " " << p->getParents()[i];
  }
  out << ">\n";
  out << "<SPOUSES: " << (p->hasSpouse() ? 1 : 0);
  if (p->hasSpouse()) {
    out << " " << p->getSpouse();
  }
  out << ">\n";
  out << "<CHILDREN: " << p->getChildrenCount();
  for (size_t i = 0; i < p->getChildrenCount(); ++i) {
    out << " " << p->getChildren()[i];
  }
  out << ">\n";
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

bool chernov::Tree::addParent(const std::string & childId, const std::string & parentId, std::string & errorMsg)
{
  Person * child = findPerson(childId);
  Person * parent = findPerson(parentId);
  if (!child) {
    errorMsg = "child '" + childId + "' not found";
    return false;
  }
  if (!parent) {
    errorMsg = "parent '" + parentId + "' not found";
    return false;
  }
  if (child->hasParent(parentId)) {
    errorMsg = "already a parent";
    return false;
  }
  if (isDescendant(childId, parentId)) {
    errorMsg = "cycle detected";
    return false;
  }
  if (child->getParentsCount() >= 2) {
    errorMsg = "already has two parents";
    return false;
  }
  const std::string & newGender = parent->getGender();
  if (newGender != "Unknown") {
    for (size_t i = 0; i < child->getParentsCount(); ++i) {
      const std::string & pid = child->getParents()[i];
      const Person * existingParent = findPerson(pid);
      if (existingParent && existingParent->getGender() == newGender) {
        if (newGender == "Male") {
          errorMsg = "Person already has a father";
        } else {
          errorMsg = "Person already has a mother";
        }
        return false;
      }
    }
  }
  child->addParent(parentId);
  parent->addChild(childId);
  return true;
}

bool chernov::Tree::removeParent(const std::string & childId, const std::string & parentId)
{
  Person * child = findPerson(childId);
  Person * parent = findPerson(parentId);
  if (!child || !parent) {
    return false;
  }
  if (!child->hasParent(parentId)) {
    return false;
  }
  child->removeParent(parentId);
  parent->removeChild(childId);
  return true;
}

bool chernov::Tree::addSpouse(const std::string & id1, const std::string & id2, std::string & errorMsg)
{
  Person * p1 = findPerson(id1);
  Person * p2 = findPerson(id2);
  if (!p1 || !p2) {
    errorMsg = "person not found";
    return false;
  }
  if (id1 == id2) {
    errorMsg = "cannot marry oneself";
    return false;
  }
  if (p1->hasSpouse() || p2->hasSpouse()) {
    errorMsg = "already has a spouse";
    return false;
  }
  p1->setSpouse(id2);
  p2->setSpouse(id1);
  return true;
}

bool chernov::Tree::removeSpouse(const std::string & id1, const std::string & id2)
{
  Person * p1 = findPerson(id1);
  Person * p2 = findPerson(id2);
  if (!p1 || !p2) {
    return false;
  }
  if (p1->getSpouse() != id2 || p2->getSpouse() != id1) {
    return false;
  }
  p1->clearSpouse();
  p2->clearSpouse();
  return true;
}

void chernov::Tree::showConnections(const std::string & id, const std::string & type, std::ostream & out) const
{
  const Person * p = findPerson(id);
  if (!p) {
    out << "<ERROR: Person not found>\n";
    return;
  }
  if (type == "parents" || type == "all") {
    out << "<PARENTS:";
    for (size_t i = 0; i < p->getParentsCount(); ++i) {
      out << " " << p->getParents()[i];
    }
    out << ">\n";
  }
  if (type == "spouses" || type == "all") {
    out << "<SPOUSES:";
    if (p->hasSpouse()) {
      out << " " << p->getSpouse();
    }
    out << ">\n";
  }
  if (type == "children" || type == "all") {
    out << "<CHILDREN:";
    for (size_t i = 0; i < p->getChildrenCount(); ++i) {
      out << " " << p->getChildren()[i];
    }
    out << ">\n";
  }
}

void chernov::Tree::removeAllConnections(const std::string & id)
{
  Person * p = findPerson(id);
  if (!p) {
    return;
  }

  while (p->getParentsCount() > 0) {
    std::string parentId = p->getParents()[0];
    removeParent(id, parentId);
  }

  while (p->getChildrenCount() > 0) {
    std::string childId = p->getChildren()[0];
    removeParent(childId, id);
  }
  if (p->hasSpouse()) {
    std::string spouseId = p->getSpouse();
    removeSpouse(id, spouseId);
  }
}

bool chernov::Tree::isDescendant(const std::string & ancestorId, const std::string & personId) const
{
  const Person * ancestor = findPerson(ancestorId);
  if (!ancestor) {
    return false;
  }
  for (size_t i = 0; i < ancestor->getChildrenCount(); ++i) {
    const std::string & childId = ancestor->getChildren()[i];
    if (childId == personId) {
      return true;
    }
    if (isDescendant(childId, personId)) {
      return true;
    }
  }
  return false;
}

void chernov::Tree::showAncestors(const std::string & id, int maxDepth, std::ostream & out) const
{
  const Person * p = findPerson(id);
  if (!p) {
    out << "<ERROR: Person not found>\n";
    return;
  }
  Vector< std::string > ancestors = detail::findAncestors(*this, id, maxDepth);
  out << "<ANCESTORS:>\n";
  for (size_t i = 0; i < ancestors.getSize(); ++i) {
    const Person * anc = findPerson(ancestors[i]);
    if (anc) {
      out << "<  " << anc->getId() << ": " << anc->getSurname() << " " << anc->getName() << ">\n";
    }
  }
}

void chernov::Tree::showDescendants(const std::string & id, int maxDepth, std::ostream & out) const
{
  const Person * p = findPerson(id);
  if (!p) {
    out << "<ERROR: Person not found>\n";
    return;
  }
  Vector< std::string > descendants = detail::findDescendants(*this, id, maxDepth);
  out << "<DESCENDANTS:>\n";
  for (size_t i = 0; i < descendants.getSize(); ++i) {
    const Person * desc = findPerson(descendants[i]);
    if (desc) {
      out << "<  " << desc->getId() << ": " << desc->getSurname() << " " << desc->getName() << ">\n";
    }
  }
}

void chernov::Tree::showRelatives(const std::string & id, int maxDepth, std::ostream & out) const
{
  const Person * p = findPerson(id);
  if (!p) {
    out << "<ERROR: Person not found>\n";
    return;
  }
  Vector< detail::Relative > relatives = detail::findRelatives(*this, id, maxDepth);
  out << "<RELATIVES (depth=" << (maxDepth == -1 ? "all" : std::to_string(maxDepth)) << "):>\n";
  for (size_t i = 0; i < relatives.getSize(); ++i) {
    const Person * rel = findPerson(relatives[i].id);
    if (rel) {
      out << "<  " << rel->getId() << ": " << rel->getSurname() << " " << rel->getName();
      if (!relatives[i].relation.empty()) {
        out << " [" << relatives[i].relation << "]";
      }
      out << ">\n";
    }
  }
}

chernov::Vector< std::string > chernov::Tree::findCommonAncestors(
  const std::string & id1, const std::string & id2) const
{
  return detail::findCommonAncestors(*this, id1, id2);
}

void chernov::Tree::showRelationship(const std::string & id1, const std::string & id2, std::ostream & out) const
{
  std::string rel = detail::getRelationship(*this, id1, id2);
  out << "<RELATION: " << rel << ">\n";
}

chernov::Vector< std::string > chernov::Tree::getAllPersonIds() const
{
  Vector< std::string > ids;
  for (auto it = idToIndex_.cbegin(); it != idToIndex_.cend(); ++it) {
    ids.pushBack((*it).first);
  }
  return ids;
}

size_t chernov::Tree::getNextId() const
{
  return nextId_;
}

bool chernov::Tree::addPersonWithId(const std::string & id,
  const std::string & surname,
  const std::string & name,
  const std::string & patronymic,
  const std::string & gender,
  std::string & errorMsg)
{
  if (idToIndex_.has(id)) {
    errorMsg = "person with id " + id + " already exists";
    return false;
  }
  if (id.size() < 2 || id[0] != 'P') {
    errorMsg = "invalid id format";
    return false;
  }
  size_t num = 0;
  try {
    num = std::stoul(id.substr(1));
  } catch (...) {
    errorMsg = "invalid id number";
    return false;
  }
  Person person(id, surname, name, patronymic, gender);
  persons_.pushBack(person);
  size_t index = persons_.getSize() - 1;
  idToIndex_.add(id, index);
  if (num >= nextId_) {
    nextId_ = num + 1;
  }
  return true;
}

void chernov::Tree::setNextId(size_t nextId)
{
  if (nextId > nextId_) {
    nextId_ = nextId;
  }
}

void chernov::Tree::setName(const std::string & name)
{
  name_ = name;
}
