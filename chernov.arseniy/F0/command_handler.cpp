#include "command_handler.hpp"
#include <iostream>
#include "commands.hpp"

namespace chernov {
  CommandHandler::CommandHandler(TreeManager & manager):
    manager_(manager),
    commands_(64)
  {
    commands_.add("create-tree", cmdCreateTree);
    commands_.add("drop-tree", cmdDropTree);
    commands_.add("rename-tree", cmdRenameTree);
    commands_.add("edit-tree-desc", cmdEditTreeDesc);
    commands_.add("list-trees", cmdListTrees);
    commands_.add("add-person", cmdAddPerson);
    commands_.add("edit-person", cmdEditPerson);
    commands_.add("delete-person", cmdDeletePerson);
    commands_.add("show-person", cmdShowPerson);
    commands_.add("search-person", cmdSearchPerson);
    commands_.add("list-persons", cmdListPersons);
    commands_.add("add-parent", cmdAddParent);
    commands_.add("remove-parent", cmdRemoveParent);
    commands_.add("add-spouse", cmdAddSpouse);
    commands_.add("remove-spouse", cmdRemoveSpouse);
    commands_.add("show-connections", cmdShowConnections);
    commands_.add("show-ancestors", cmdShowAncestors);
    commands_.add("show-descendants", cmdShowDescendants);
    commands_.add("show-relatives", cmdShowRelatives);
    commands_.add("find-common-ancestor", cmdFindCommonAncestor);
    commands_.add("show-relationship", cmdShowRelationship);
    commands_.add("compare-trees", cmdCompareTrees);
    commands_.add("merge-persons", cmdMergePersons);
    commands_.add("save", cmdSave);
    commands_.add("load", cmdLoad);
    commands_.add("show-tree", cmdShowTree);
  }

  void CommandHandler::handle(const std::string & line)
  {
    Vector< std::string > args;
    std::string current;
    bool inQuotes = false;

    size_t i = 0;
    while (i < line.size()) {
      char c = line[i];
      if (c == '\"') {
        inQuotes = !inQuotes;
      } else if (c == ' '  && !inQuotes) {
        if (!current.empty()) {
          args.pushBack(current);
          current.clear();
        }
      } else {
        current += c;
      }
      ++i;
    }
    if (!current.empty()) {
      args.pushBack(current);
    }

    if (args.isEmpty()) {
      return;
    }

    try {
      CommandFunc func = commands_.at(args[0]);
      func(args, manager_, std::cout);
    } catch (const std::out_of_range &) {
      std::cout << "<ERROR: Unknown command>\n";
    }
  }
}
