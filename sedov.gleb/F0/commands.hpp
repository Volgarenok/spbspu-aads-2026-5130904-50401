#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "../common/vector.hpp"
#include "scheduler.hpp"
#include "avltree.hpp"

namespace sedov
{
  using CommandHandler = void(*)(const std::vector< std::string > &, std::ostream &, Scheduler &);
  using CommandMap = AVLTree< std::string, CommandHandler >;

  CommandMap makeCommandMap();

  void cCreateProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cDeleteProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cListProfiles(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cCreateSchedule(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cDeleteSchedule(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cListSchedules(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cAddTask(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cRemoveTask(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cViewTask(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cListTasks(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cShowUnplaced(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cAutoPlace(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cFindFreeWindow(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cMergeSchedules(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cCompareMerge(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cOptimizeRange(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cFindBestDay(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cExportProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cImportProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched);
}

#endif
