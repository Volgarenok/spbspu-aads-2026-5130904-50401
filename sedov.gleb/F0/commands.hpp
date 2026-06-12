#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "scheduler.hpp"
#include "avltree.hpp"

namespace sedov
{
  using CommandHandler = void(*)(std::istream &, std::ostream &, Scheduler &);
  using CommandMap = AVLTree< std::string, CommandHandler >;

  CommandMap makeCommandMap();

  void handleCreateProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleDeleteProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleListProfiles(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleCreateSchedule(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleDeleteSchedule(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleListSchedules(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleAddTask(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleRemoveTask(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleViewTask(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleListTasks(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleShowUnplaced(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleAutoPlace(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleFindFreeWindow(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleMergeSchedules(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleCompareMerge(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleOptimizeRange(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleFindBestDay(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleExportProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleImportProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void handleHelp(std::istream & in, std::ostream & out, Scheduler & sched);
}

#endif
