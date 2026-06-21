#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <string>

namespace chernov {

  class Tree;

  namespace detail {

    struct PersonData {
      std::string id;
      std::string lastName;
      std::string firstName;
      std::string patronymic;
      std::string gender;
      std::string birthDate;
      std::string deathDate;
      std::string info;
      std::string parentsStr;
      std::string spousesStr;
      std::string childrenStr;
    };

    bool saveTree(const Tree & tree, const std::string & filename);
    bool loadTree(Tree & tree, const std::string & filename, std::string & treeNameFromFile, std::string & errorMsg);
  }
}

#endif
