#include "commands.hpp"

namespace sedov
{
  CommandMap makeCommandMap()
  {
    CommandMap cmds;

    cmds.insert("create_profile", cCreateProfile);
    cmds.insert("delete_profile", cDeleteProfile);
    cmds.insert("list_profiles", cListProfiles);
    cmds.insert("create_schedule", cCreateSchedule);
    cmds.insert("delete_schedule", cDeleteSchedule);
    cmds.insert("list_schedules", cListSchedules);
    cmds.insert("add_task", cAddTask);
    cmds.insert("remove_task", cRemoveTask);
    cmds.insert("view_task", cViewTask);
    cmds.insert("list_tasks", cListTasks);
    cmds.insert("show_unplaced", cShowUnplaced);
    cmds.insert("auto_place", cAutoPlace);
    cmds.insert("find_free_window", cFindFreeWindow);
    cmds.insert("merge_schedules", cMergeSchedules);
    cmds.insert("compare_merge", cCompareMerge);
    cmds.insert("optimize_range", cOptimizeRange);
    cmds.insert("find_best_day", cFindBestDay);
    cmds.insert("export_profile", cExportProfile);
    cmds.insert("import_profile", cImportProfile);

    return cmds;
  }

  void cCreateProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 1)
    {
      out << "[ERROR] Usage: create_profile <name>\n";
      return;
    }
    sched.createProfile(args[0]);
  }

  void cDeleteProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 1)
    {
      out << "[ERROR] Usage: delete_profile <name>\n";
      return;
    }
    sched.deleteProfile(args[0]);
  }

  void cListProfiles(const std::vector<std::string>&, std::ostream&, Scheduler & sched)
  {
    sched.listProfiles();
  }

  void cCreateSchedule(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 2)
    {
      out << "[ERROR] Usage: create_schedule <profile> <name>\n";
      return;
    }
    sched.createSchedule(args[0], args[1]);
  }

  void cDeleteSchedule(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 2)
    {
      out << "[ERROR] Usage: delete_schedule <profile> <name>\n";
      return;
    }
    sched.deleteSchedule(args[0], args[1]);
  }

  void cListSchedules(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 1)
    {
      out << "[ERROR] Usage: list_schedules <profile>\n";
      return;
    }
    sched.listSchedules(args[0]);
  }

  void cAddTask(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 7)
    {
      out << "[ERROR] Usage: add_task <profile> <schedule> <title> <date> <time_start> <time_end> <importance>\n";
      return;
    }
    sched.addTask(args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
  }

  void cRemoveTask(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 3)
    {
      out << "[ERROR] Usage: remove_task <profile> <schedule> <id>\n";
      return;
    }
    int id = 0;
    try
    {
      id = std::stoi(args[2]);
    }
    catch (const std::out_of_range &)
    {
      out << "[ERROR] Task ID value is too large\n";
      return;
    }
    catch (...)
    {
      out << "[ERROR] Invalid task ID\n";
      return;
    }
    sched.removeTask(args[0], args[1], id);
  }

  void cViewTask(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 3)
    {
      out << "[ERROR] Usage: view_task <profile> <schedule> <id>\n";
      return;
    }
    int id = 0;
    try
    {
      id = std::stoi(args[2]);
    }
    catch (const std::out_of_range &)
    {
      out << "[ERROR] Task ID value is too large\n";
      return;
    }
    catch (...)
    {
      out << "[ERROR] Invalid task ID\n";
      return;
    }
    sched.viewTask(args[0], args[1], id);
  }

  void cListTasks(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 3)
    {
      out << "[ERROR] Usage: list_tasks <profile> <schedule> <date>\n";
      return;
    }
    sched.listTasks(args[0], args[1], args[2]);
  }

  void cShowUnplaced(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 1)
    {
      out << "[ERROR] Usage: show_unplaced <profile>\n";
      return;
    }
    sched.showUnplaced(args[0]);
  }

  void cAutoPlace(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 3)
    {
      out << "[ERROR] Usage: auto_place <profile> <date_from> <date_to>\n";
      return;
    }
    sched.autoPlace(args[0], args[1], args[2]);
  }

  void cFindFreeWindow(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 7)
    {
      out << "[ERROR] Usage: find_free_window <p1> <s1> <p2> <s2> <from> <to> <min_hours>\n";
      return;
    }
    int minHours = 0;
    try
    {
      minHours = std::stoi(args[6]);
    }
    catch (const std::out_of_range &)
    {
      out << "[ERROR] Task ID value is too large\n";
      return;
    }
    catch (...)
    { out << "[ERROR] Invalid min_hours value\n";
      return;
    }
    sched.findFreeWindow(args[0], args[1], args[2], args[3], args[4], args[5], minHours);
  }

  void cMergeSchedules(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 4)
    {
      out << "[ERROR] Usage: merge_schedules <profile> <new_name> <s1> <s2>\n";
      return;
    }
    sched.mergeSchedules(args[0], args[1], args[2], args[3]);
  }

  void cCompareMerge(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 3)
    {
      out << "[ERROR] Usage: compare_merge <profile> <s1> <s2>\n";
      return;
    }
    sched.compareMerge(args[0], args[1], args[2]);
  }

  void cOptimizeRange(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 4)
    {
      out << "[ERROR] Usage: optimize_range <profile> <from> <to> <criterion> [schedules...]\n";
      return;
    }
    std::vector< std::string > schedules;
    for (size_t i = 4; i < args.size(); ++i)
    {
      schedules.push_back(args[i]);
    }
    sched.optimizeRange(args[0], args[1], args[2], args[3], schedules);
  }

  void cFindBestDay(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 5)
    {
      out << "[ERROR] Usage: find_best_day <p1> <s1> <p2> <s2> <min_hours>\n";
      return;
    }
    int minHours = 0;
    try
    {
      minHours = std::stoi(args[4]);
    }
    catch (const std::out_of_range &)
    {
      out << "[ERROR] Task ID value is too large\n";
      return;
    }
    catch (...)
    {
      out << "[ERROR] Invalid min_hours value\n";
      return;
    }
    sched.findBestDay(args[0], args[1], args[2], args[3], minHours);
  }

  void cExportProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 2)
    {
      out << "[ERROR] Usage: export_profile <name> <filename>\n";
      return;
    }
    sched.exportProfile(args[0], args[1]);
  }

  void cImportProfile(const std::vector< std::string > & args, std::ostream & out, Scheduler & sched)
  {
    if (args.size() < 1)
    {
      out << "[ERROR] Usage: import_profile <filename>\n";
      return;
    }
    sched.importProfile(args[0]);
  }
}
