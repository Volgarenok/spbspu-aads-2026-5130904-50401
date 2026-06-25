#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "bstree.hpp"
#include "iostream"
namespace madieva {
  struct Dictionary {
    BSTree< int, std::string, std::less< int > > dictionary;
  };

  struct Dictionarycollection {
    BSTree< std::string,
      Dictionary, std::less< std::string > > collection;
  };

  struct Commands {
    using function = void(*)(std::istream &,
      std::ostream &, madieva::Dictionarycollection &);
    BSTree< std::string, function, std::less< std::string > > comm;
  };

  void cmd_print(std::istream & in,
    std::ostream & out, madieva::Dictionarycollection & storage)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.collection.has(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    const Dictionary & dict = storage.collection.get(name);
    if (dict.dictionary.begin() == dict.dictionary.end())
    {
      out << "<EMPTY>\n";
      return;
    }
    out << name;
    TCIter< int, std::string, std::less< int > > it = dict.dictionary.begin();
    for(; it != dict.dictionary.end(); ++it) {
      out << " " << (*it).first << " " << (*it).second;
    }
    out << "\n";
    return;
  }

  void cmd_complement(std::istream & in,
    std::ostream & out, madieva::Dictionarycollection & storage)
  {
    std::string new_name;
    if (!(in >> new_name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string name1;
    if (!(in >> name1)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string name2;
    if (!(in >> name2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (!storage.collection.has(name1)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.collection.has(name2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary & dict1 = storage.collection.get(name1);
    const Dictionary & dict2 = storage.collection.get(name2);
    TCIter< int, std::string, std::less< int > > it1 = dict1.dictionary.begin();
    Dictionary new_dict;
    for (; it1 != dict1.dictionary.end(); ++it1) {
      int key = (*it1).first;
      const std::string & value = (*it1).second;
      if (!dict2.dictionary.has(key)) {
        new_dict.dictionary.push(key, value);
      }
    }
   if (storage.collection.has(new_name)) {
      storage.collection.drop(new_name);
    }
    storage.collection.push(new_name, new_dict);
  }

  void cmd_intersect(std::istream & in,
    std::ostream & out, madieva::Dictionarycollection & storage)
  {
    std::string new_name;
    if (!(in >> new_name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string name1;
    if (!(in >> name1)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string name2;
    if (!(in >> name2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (!storage.collection.has(name1)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.collection.has(name2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary & dict1 = storage.collection.get(name1);
    const Dictionary & dict2 = storage.collection.get(name2);
    TCIter< int, std::string, std::less< int > > it1 = dict1.dictionary.begin();
    Dictionary new_dict;
    for (; it1 != dict1.dictionary.end(); ++it1) {
      int key = (*it1).first;
      const std::string & value = (*it1).second;
      if (dict2.dictionary.has(key)) {
        new_dict.dictionary.push(key, value);
      }
    }
    if (storage.collection.has(new_name)) {
      storage.collection.drop(new_name);
    }
    storage.collection.push(new_name, new_dict);
  }

  void cmd_union(std::istream & in,
    std::ostream & out, madieva::Dictionarycollection & storage)
  {
    std::string new_name;
    if (!(in >> new_name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string name1;
    if (!(in >> name1)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string name2;
    if (!(in >> name2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (!storage.collection.has(name1)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.collection.has(name2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Dictionary & dict1 = storage.collection.get(name1);
    const Dictionary & dict2 = storage.collection.get(name2);
    TCIter< int, std::string, std::less< int > > it1 = dict1.dictionary.begin();
    Dictionary new_dict;
    for (; it1 != dict1.dictionary.end(); ++it1) {
      int key = (*it1).first;
      const std::string & value = (*it1).second;
      new_dict.dictionary.push(key, value);
    }
    TCIter< int, std::string, std::less< int > > it2 = dict2.dictionary.begin();
    for (; it2 != dict2.dictionary.end(); ++it2) {
      int key = (*it2).first;
      const std::string & value = (*it2).second;
      if (!dict1.dictionary.has(key)) {
        new_dict.dictionary.push(key, value);
      }
    }
    if (storage.collection.has(new_name)) {
      storage.collection.drop(new_name);
    }
    storage.collection.push(new_name, new_dict);
  }

}

#endif
