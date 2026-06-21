#include "commands.hpp"
#include <iostream>
#include "tree.hpp"
#include "tree_manager.hpp"

namespace chernov {
  void cmdCreateTree(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() < 2 || args.getSize() > 3) {
      out << "<ERROR: Invalid arguments for create-tree>\n";
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
    if (args.getSize() != 5) {
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
    if (args.getSize() != 3) {
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
    if (args.getSize() != 3) {
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
    if (args.getSize() != 4) {
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
    if (args.getSize() < 2 || args.getSize() > 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    if (!manager.hasTree(args[1])) {
      out << "<ERROR: Tree '" << args[1] << "' not found>\n";
      return;
    }
    Tree & tree = manager.getTree(args[1]);
    std::string filter;
    if (args.getSize() == 3) {
      filter = args[2];
      if (filter != "alive" && filter != "deceased" && filter != "male" && filter != "female") {
        out << "<ERROR: Invalid filter>\n";
        return;
      }
    }
    tree.listPersons(out, filter);
  }

  void cmdAddParent(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    std::string error;
    if (!tree.addParent(args[2], args[3], error)) {
      out << "<ERROR: " << error << ">\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdRemoveParent(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    if (!tree.removeParent(args[2], args[3])) {
      out << "<ERROR: Parent link not found>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdAddSpouse(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    std::string error;
    if (!tree.addSpouse(args[2], args[3], error)) {
      out << "<ERROR: " << error << ">\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdRemoveSpouse(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    if (!tree.removeSpouse(args[2], args[3])) {
      out << "<ERROR: Spouse link not found>\n";
      return;
    }
    out << "<OK>\n";
  }

  void cmdShowConnections(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    std::string type = "all";
    if (args.getSize() == 4)
      type = args[3];
    tree.showConnections(args[2], type, out);
  }

  void cmdShowAncestors(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    int depth = -1;
    if (args.getSize() == 4) {
      try {
        depth = std::stoi(args[3]);
      } catch (...) {
        out << "<ERROR: Invalid depth>\n";
        return;
      }
    }
    tree.showAncestors(args[2], depth, out);
  }

  void cmdShowDescendants(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    int depth = -1;
    if (args.getSize() == 4) {
      try {
        depth = std::stoi(args[3]);
      } catch (...) {
        out << "<ERROR: Invalid depth>\n";
        return;
      }
    }
    tree.showDescendants(args[2], depth, out);
  }

  void cmdShowRelatives(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    int depth = -1;
    if (args.getSize() == 4) {
      try {
        depth = std::stoi(args[3]);
      } catch (...) {
        out << "<ERROR: Invalid depth>\n";
        return;
      }
    }
    tree.showRelatives(args[2], depth, out);
  }

  void cmdFindCommonAncestor(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    Vector< std::string > common = tree.findCommonAncestors(args[2], args[3]);
    out << "<COMMON ANCESTORS:>\n";
    for (size_t i = 0; i < common.getSize(); ++i) {
      const Person * anc = tree.findPerson(common[i]);
      if (anc) {
        out << "<  " << anc->getId() << ": " << anc->getSurname() << " " << anc->getName() << ">\n";
      }
    }
  }

  void cmdShowRelationship(CommandArgs & args, TreeManager & manager, std::ostream & out)
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
    tree.showRelationship(args[2], args[3], out);
  }

  void cmdCompareTrees(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    manager.compareTrees(args[1], args[2], out);
  }

  void cmdMergePersons(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 6) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    manager.mergePersons(args[1], args[2], args[3], args[4], args[5], out);
  }

  void cmdSave(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() != 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    manager.saveTree(args[1], args[2], out);
  }

  void cmdLoad(CommandArgs & args, TreeManager & manager, std::ostream & out)
  {
    if (args.getSize() < 2 || args.getSize() > 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    std::string forcedName = (args.getSize() == 3) ? args[2] : "";
    manager.loadTree(args[1], forcedName, out);
  }

  void cmdShowTree(CommandArgs & args, TreeManager &, std::ostream & out)
  {
    if (args.getSize() != 3) {
      out << "<ERROR: Invalid arguments>\n";
      return;
    }
    out << "<TREE VIEW:>\n";
  }
}
