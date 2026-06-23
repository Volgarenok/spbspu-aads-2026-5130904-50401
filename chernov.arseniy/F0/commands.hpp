#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <vector.hpp>

namespace chernov {
  class TreeManager;

  using CommandArgs = Vector< std::string >;

  void cmdCreateTree(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdDropTree(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdRenameTree(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdEditTreeDesc(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdListTrees(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdAddPerson(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdEditPerson(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdDeletePerson(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdShowPerson(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdSearchPerson(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdListPersons(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdAddParent(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdRemoveParent(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdAddSpouse(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdRemoveSpouse(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdShowConnections(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdShowAncestors(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdShowDescendants(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdShowRelatives(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdFindCommonAncestor(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdShowRelationship(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdCompareTrees(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdMergePersons(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdSave(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdLoad(CommandArgs & args, TreeManager & manager, std::ostream & out);
  void cmdShowTree(CommandArgs & args, TreeManager & manager, std::ostream & out);
}

#endif
