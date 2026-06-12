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

  void cCreateProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void cDeleteProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void cListProfiles(std::istream & in, std::ostream & out, Scheduler & sched);
  void cCreateSchedule(std::istream & in, std::ostream & out, Scheduler & sched);
  void cDeleteSchedule(std::istream & in, std::ostream & out, Scheduler & sched);
  void cListSchedules(std::istream & in, std::ostream & out, Scheduler & sched);
  void cAddTask(std::istream & in, std::ostream & out, Scheduler & sched);
  void cRemoveTask(std::istream & in, std::ostream & out, Scheduler & sched);
  void cViewTask(std::istream & in, std::ostream & out, Scheduler & sched);
  void cListTasks(std::istream & in, std::ostream & out, Scheduler & sched);
  void cShowUnplaced(std::istream & in, std::ostream & out, Scheduler & sched);
  void cAutoPlace(std::istream & in, std::ostream & out, Scheduler & sched);
  void cFindFreeWindow(std::istream & in, std::ostream & out, Scheduler & sched);
  void cMergeSchedules(std::istream & in, std::ostream & out, Scheduler & sched);
  void cCompareMerge(std::istream & in, std::ostream & out, Scheduler & sched);
  void cOptimizeRange(std::istream & in, std::ostream & out, Scheduler & sched);
  void cFindBestDay(std::istream & in, std::ostream & out, Scheduler & sched);
  void cExportProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void cImportProfile(std::istream & in, std::ostream & out, Scheduler & sched);
  void cHelp(std::istream & in, std::ostream & out, Scheduler & sched);
}

#endif
