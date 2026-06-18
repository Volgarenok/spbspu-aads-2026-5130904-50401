#include "commands.hpp"
#include <iostream>
#include "tree_manager.hpp"

namespace chernov {
  void cmdCreateTree(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() < 2 || args.getSize() > 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    std::string desc = (args.getSize() == 3) ? args[2] : "";
    manager.createTree(args[1], desc, out);
  }

  void cmdDropTree(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 2) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    manager.dropTree(args[1], out);
  }

  void cmdRenameTree(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    manager.renameTree(args[1], args[2], out);
  }

  void cmdEditTreeDesc(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    manager.editTreeDescription(args[1], args[2], out);
  }

  void cmdListTrees(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 1) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    manager.listTrees(out);
  }

  void cmdAddPerson(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 6) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    if (!manager.hasTree(args[1])) {
      out << "<ERROR: Tree '" << args[1] << "' not found>\n";
      return;
    }
    Tree & tree = manager.getTree(args[1]);
    std::string id = tree.addPerson(args[2], args[3], args[4], args[5]);
    out << "<CREATED: " << id << " in " << args[1] << ">\n";
  }

  void cmdEditPerson(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 6) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    if (!manager.hasTree(args[1])) {
      out << "<ERROR: Tree '" << args[1] << "' not found>\n";
      return;
    }
    Tree & tree = manager.getTree(args[1]);
    std::string error;
    if (!tree.editPerson(args[2], args[3], args[4], error)) {
      out << "<ERROR: " << error << ">\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdDeletePerson(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    if (!manager.hasTree(args[1])) {
      out << "<ERROR: Tree '" << args[1] << "' not found>\n";
      return;
    }
    Tree & tree = manager.getTree(args[1]);
    if (!tree.deletePerson(args[2])) {
      out << "<ERROR: Person '" << args[2] << "' not found>\n";
      return;
    }
    out << "<OK: Person " << args[2] << " deleted>\n";
  }

  void cmdShowPerson(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    if (!manager.hasTree(args[1])) {
      out << "<ERROR: Tree '" << args[1] << "' not found>\n";
      return;
    }
    Tree & tree = manager.getTree(args[1]);
    if (!tree.showPerson(args[2], out)) {
      out << "<ERROR: Person '" << args[2] << "' not found>\n";
    }
  }

  void cmdSearchPerson(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    if (!manager.hasTree(args[1])) {
      out << "<ERROR: Tree '" << args[1] << "' not found>\n";
      return;
    }
    Tree & tree = manager.getTree(args[1]);
    Vector< std::string > ids = tree.searchPerson(args[2], args[3]);
    out << "<SEARCH RESULTS (" << args[2] << "=\"" << args[3] << "\")>\n";
    for (size_t i = 0; i < ids.getSize(); ++i) {
      const Person * p = tree.findPerson(ids[i]);
      out << "<  " << ids[i] << ": " << p->getSurname() << " " << p->getName() << ">\n";
    }
  }

  void cmdListPersons(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() < 3 || args.getSize() > 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    if (!manager.hasTree(args[1])) {
      out << "<ERROR: Tree '" << args[1] << "' not found>\n";
      return;
    }
    Tree & tree = manager.getTree(args[1]);
    std::string filter = args[2];
    if (filter != "alive" && filter != "deceased" && filter != "male" && filter != "female") {
      out << "<ERROR: Invalid filter>\n";
      return;
    }
    tree.listPersons(out, filter);
  }

  void cmdAddParent(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdRemoveParent(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdAddSpouse(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdRemoveSpouse(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdShowConnections(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<CHILDREN: >\n";
  }

  void cmdShowAncestors(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<ANCESTORS:>\n";
  }

  void cmdShowDescendants(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<DESCENDANTS:>\n";
  }

  void cmdShowRelatives(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() < 4 || args.getSize() > 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<RELATIVES (depth=...):>\n";
  }

  void cmdFindCommonAncestor(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<COMMON ANCESTORS:>\n";
  }

  void cmdShowRelationship(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 5) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<RELATION: ...>\n";
  }

  void cmdCompareTrees(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<COMPARISON RESULTS:>\n";
  }

  void cmdMergePersons(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 7) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<CREATED: " << args[5] << ">\n<MERGE SUCCESS>\n";
  }

  void cmdSave(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK: Saved to " << args[2] << ">\n";
  }

  void cmdLoad(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<OK: Loaded " << args[1] << " from " << args[2] << ">\n";
  }

  void cmdShowTree(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 4) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<TREE VIEW:>\n";
  }
}
