#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "../common/vector.hpp"
#include "scheduler.hpp"
#include "avltree.hpp"

namespace sedov
{
  using CommandHandler = void(*)(const Vector< std::string > &, std::ostream &, Scheduler &);
  using CommandMap = AVLTree< std::string, CommandHandler >;

  CommandMap makeCommandMap();

  void cCreateProfile(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cDeleteProfile(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cListProfiles(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cCreateSchedule(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cDeleteSchedule(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cListSchedules(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cAddTask(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cRemoveTask(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cViewTask(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cListTasks(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cShowUnplaced(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cAutoPlace(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cFindFreeWindow(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cMergeSchedules(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cCompareMerge(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cOptimizeRange(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cFindBestDay(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cExportProfile(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
  void cImportProfile(const Vector< std::string > & args, std::ostream & out, Scheduler & sched);
}

#endif
