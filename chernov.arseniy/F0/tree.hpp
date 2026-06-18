#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include "hashers.hpp"
#include "person.hpp"
#include <cuckooht.hpp>
#include <vector.hpp>

namespace chernov {
  class Tree {
  public:
    Tree(const std::string & name, const std::string & description = "");

    const std::string & getName() const;
    const std::string & getDescription() const;
    void setDescription(const std::string & desc);
    size_t personCount() const;

    std::string addPerson(const std::string & surname,
      const std::string & name,
      const std::string & patronymic,
      const std::string & gender);
    bool deletePerson(const std::string & id);
    Person * findPerson(const std::string & id);
    const Person * findPerson(const std::string & id) const;
    Vector< std::string > searchPerson(const std::string & field, const std::string & value) const;
    void listPersons(std::ostream & out, const std::string & filter) const;
    bool showPerson(const std::string & id, std::ostream & out) const;
    bool editPerson(
      const std::string & id, const std::string & field, const std::string & value, std::string & errorMsg);

    bool addParent(const std::string & childId, const std::string & parentId, std::string & errorMsg);
    bool removeParent(const std::string & childId, const std::string & parentId);
    bool addSpouse(const std::string & id1, const std::string & id2, std::string & errorMsg);
    bool removeSpouse(const std::string & id1, const std::string & id2);
    void showConnections(const std::string & id, const std::string & type, std::ostream & out) const;

    void showAncestors(const std::string & id, int maxDepth, std::ostream & out) const;
    void showDescendants(const std::string & id, int maxDepth, std::ostream & out) const;
    void showRelatives(const std::string & id, int maxDepth, std::ostream & out) const;

    Vector< std::string > findCommonAncestors(const std::string & id1, const std::string & id2) const;
    void showRelationship(const std::string & id1, const std::string & id2, std::ostream & out) const;

  private:
    std::string name_;
    std::string description_;
    Vector< Person > persons_;
    CuckooHT< std::string, size_t, Hasher1, Hasher2, std::equal_to< std::string > > idToIndex_;
    size_t nextId_;

    std::string generateId();
    void removeAllConnections(const std::string & id);
    bool isDescendant(const std::string & ancestorId, const std::string & personId) const;
  };
}

#endif
