#include "file_io.hpp"
#include <fstream>
#include <string>
#include "date_utils.hpp"
#include "tree.hpp"

bool chernov::detail::saveTree(const Tree & tree, const std::string & filename)
{
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }
  file << "# FamilyTree v1.0\n";
  file << "# Created: 2026-06-18\n";
  file << "\n[TREE_METADATA]\n";
  file << "name=" << tree.getName() << "\n";
  file << "next_id=" << tree.getNextId() << "\n";
  Vector< std::string > ids = tree.getAllPersonIds();
  for (size_t i = 0; i < ids.getSize(); ++i) {
    const Person * p = tree.findPerson(ids[i]);
    if (!p) {
      continue;
    }
    file << "\n[PERSON:" << p->getId() << "]\n";
    file << "lastName=" << p->getSurname() << "\n";
    file << "firstName=" << p->getName() << "\n";
    file << "patronymic=" << p->getPatronymic() << "\n";
    file << "gender=" << p->getGender() << "\n";
    file << "birthDate=" << toDisplayFormat(p->getBirthDate()) << "\n";
    file << "deathDate=" << toDisplayFormat(p->getDeathDate()) << "\n";
    file << "info=" << p->getInfo() << "\n";
    file << "parents=";
    for (size_t j = 0; j < p->getParentsCount(); ++j) {
      if (j > 0) {
        file << ",";
      }
      file << p->getParents()[j];
    }
    file << "\n";
    file << "spouses=";
    if (p->hasSpouse()) {
      file << p->getSpouse();
    }
    file << "\n";
    file << "children=";
    for (size_t j = 0; j < p->getChildrenCount(); ++j) {
      if (j > 0) {
        file << ",";
      }
      file << p->getChildren()[j];
    }
    file << "\n";
  }
  file << "\n[END]\n";
  return true;
}

bool chernov::detail::loadTree(
  Tree & tree, const std::string & filename, std::string & treeNameFromFile, std::string & errorMsg)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    errorMsg = "cannot open file";
    return false;
  }

  enum class State {
    Start,
    Metadata,
    Person,
    End
  };
  State state = State::Start;
  std::string line;
  size_t nextId = 0;
  Vector< PersonData > personDataList;
  PersonData currentPerson;
  bool personStarted = false;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    if (line == "[TREE_METADATA]") {
      if (state != State::Start) {
        errorMsg = "unexpected metadata section";
        return false;
      }
      state = State::Metadata;
      continue;
    }
    if (line == "[END]") {
      if (state == State::Person && personStarted) {
        personDataList.pushBack(currentPerson);
        personStarted = false;
      }
      state = State::End;
      break;
    }
    if (line.substr(0, 8) == "[PERSON:") {
      if (state == State::Person && personStarted) {
        personDataList.pushBack(currentPerson);
        personStarted = false;
      }
      size_t endPos = line.find(']');
      if (endPos == std::string::npos || endPos <= 8) {
        errorMsg = "invalid person section";
        return false;
      }
      std::string personId = line.substr(8, endPos - 8);
      currentPerson = PersonData();
      currentPerson.id = personId;
      state = State::Person;
      personStarted = true;
      continue;
    }
    if (state == State::Metadata) {
      size_t eqPos = line.find('=');
      if (eqPos == std::string::npos) {
        errorMsg = "invalid metadata line";
        return false;
      }
      std::string key = line.substr(0, eqPos);
      std::string value = line.substr(eqPos + 1);
      if (key == "name") {
        treeNameFromFile = value;
      } else if (key == "next_id") {
        try {
          nextId = std::stoul(value);
        } catch (...) {
          errorMsg = "invalid next_id";
          return false;
        }
      }
      continue;
    }
    if (state == State::Person) {
      size_t eqPos = line.find('=');
      if (eqPos == std::string::npos) {
        errorMsg = "invalid person field";
        return false;
      }
      std::string key = line.substr(0, eqPos);
      std::string value = line.substr(eqPos + 1);
      if (key == "lastName") {
        currentPerson.lastName = value;
      } else if (key == "firstName") {
        currentPerson.firstName = value;
      } else if (key == "patronymic") {
        currentPerson.patronymic = value;
      } else if (key == "gender") {
        currentPerson.gender = value;
      } else if (key == "birthDate") {
        currentPerson.birthDate = value;
      } else if (key == "deathDate") {
        currentPerson.deathDate = value;
      } else if (key == "info") {
        currentPerson.info = value;
      } else if (key == "parents") {
        currentPerson.parentsStr = value;
      } else if (key == "spouses") {
        currentPerson.spousesStr = value;
      } else if (key == "children") {
        currentPerson.childrenStr = value;
      } else {
        errorMsg = "unknown field: " + key;
        return false;
      }
      continue;
    }
  }
  if (state != State::End) {
    errorMsg = "missing [END]";
    return false;
  }

  for (size_t i = 0; i < personDataList.getSize(); ++i) {
    const PersonData & pd = personDataList[i];
    std::string addError;
    if (!tree.addPersonWithId(pd.id, pd.lastName, pd.firstName, pd.patronymic, pd.gender, addError)) {
      errorMsg = addError;
      return false;
    }
    Person * p = tree.findPerson(pd.id);
    if (!p) {
      errorMsg = "internal error";
      return false;
    }
    if (!pd.birthDate.empty()) {
      std::string dateError;
      if (!p->setBirthDate(pd.birthDate, dateError)) {
        errorMsg = "invalid birth date for " + pd.id + ": " + dateError;
        return false;
      }
    }
    if (!pd.deathDate.empty()) {
      std::string dateError;
      if (!p->setDeathDate(pd.deathDate, dateError)) {
        errorMsg = "invalid death date for " + pd.id + ": " + dateError;
        return false;
      }
    }
    p->setInfo(pd.info);
  }

  for (size_t i = 0; i < personDataList.getSize(); ++i) {
    const PersonData & pd = personDataList[i];
    Person * p = tree.findPerson(pd.id);
    if (!p) {
      continue;
    }

    if (!pd.parentsStr.empty()) {
      std::string str = pd.parentsStr;
      size_t start = 0;
      while (start < str.size()) {
        size_t comma = str.find(',', start);
        std::string parentId;
        if (comma == std::string::npos) {
          parentId = str.substr(start);
          start = str.size();
        } else {
          parentId = str.substr(start, comma - start);
          start = comma + 1;
        }
        std::string err;
        if (!tree.addParent(pd.id, parentId, err)) {
          errorMsg = "failed to add parent " + parentId + " to " + pd.id + ": " + err;
          return false;
        }
      }
    }
    if (!pd.spousesStr.empty() && !p->hasSpouse()) {
      std::string spouseId = pd.spousesStr;
      size_t comma = spouseId.find(',');
      if (comma != std::string::npos) {
        spouseId = spouseId.substr(0, comma);
      }
      std::string err;
      if (!tree.addSpouse(pd.id, spouseId, err)) {
        errorMsg = "failed to add spouse " + spouseId + " to " + pd.id + ": " + err;
        return false;
      }
    }
  }

  tree.setNextId(nextId);
  return true;
}
