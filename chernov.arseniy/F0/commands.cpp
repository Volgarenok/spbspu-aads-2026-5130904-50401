#include "commands.hpp"
#include <iostream>
#include "tree_manager.hpp"

namespace chernov {
  void cmdCreateTree(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() < 2 || args.getSize() > 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<CREATED: " << args[1] << ">\n";
  }

  void cmdDropTree(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 2) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK: Tree " << args[1] << " removed>\n";
  }

  void cmdRenameTree(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK: Renamed to " << args[2] << ">\n";
  }

  void cmdEditTreeDesc(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdListTrees(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 1) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<TREES:>\n";
  }

  void cmdAddPerson(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 6) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<CREATED: P0001 in " << args[1] << ">\n";
  }

  void cmdEditPerson(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 6) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdDeletePerson(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK: Person " << args[2] << " deleted>\n";
  }

  void cmdShowPerson(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<ID: " << args[2] << ">\n";
    out << "<SURNAME: >\n";
    out << "<NAME: >\n";
    out << "<PATRONYMIC: >\n";
    out << "<GENDER: >\n";
    out << "<BIRTHDATE: >\n";
    out << "<DEATHDATE: >\n";
    out << "<INFO: >\n";
    out << "<PARENTS: 0>\n";
    out << "<SPOUSES: 0>\n";
    out << "<CHILDREN: 0>\n";
  }

  void cmdSearchPerson(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<SEARCH RESULTS (name=\"...\")>\n";
  }

  void cmdListPersons(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() < 3 || args.getSize() > 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<LIST (...):>\n";
  }

  void cmdAddParent(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdRemoveParent(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdAddSpouse(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdRemoveSpouse(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdShowConnections(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<CHILDREN: >\n";
  }

  void cmdShowAncestors(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<ANCESTORS:>\n";
  }

  void cmdShowDescendants(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<DESCENDANTS:>\n";
  }

  void cmdShowRelatives(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<RELATIVES (depth=...):>\n";
  }

  void cmdFindCommonAncestor(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<COMMON ANCESTORS:>\n";
  }

  void cmdShowRelationship(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<RELATION: ...>\n";
  }

  void cmdCompareTrees(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<COMPARISON RESULTS:>\n";
  }

  void cmdMergePersons(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 7) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<CREATED: " << args[5] << ">\n<MERGE SUCCESS>\n";
  }

  void cmdSave(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK: Saved to " << args[2] << ">\n";
  }

  void cmdLoad(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK: Loaded " << args[1] << " from " << args[2] << ">\n";
  }

  void cmdShowTree(CommandArgs & args, TreeManager &, std::ostream & out) {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<TREE VIEW:>\n";
  }
}
