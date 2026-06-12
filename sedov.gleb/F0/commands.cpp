#include "commands.hpp"
#include "vector.hpp"

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

  void cCreateProfile(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string name;
    if (!(in >> name))
    {
      out << "[ERROR] Usage: create_profile <name>\n";
      return;
    }
    sched.createProfile(name);
  }

  void cDeleteProfile(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string name;
    if (!(in >> name))
    {
      out << "[ERROR] Usage: delete_profile <name>\n";
      return;
    }
    sched.deleteProfile(name);
  }

  void cListProfiles(std::istream &, std::ostream &, Scheduler & sched)
  {
    sched.listProfiles();
  }

  void cCreateSchedule(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, schedName;
    if (!(in >> profName >> schedName))
    {
      out << "[ERROR] Usage: create_schedule <profile> <name>\n";
      return;
    }
    sched.createSchedule(profName, schedName);
  }

  void cDeleteSchedule(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, schedName;
    if (!(in >> profName >> schedName))
    {
      out << "[ERROR] Usage: delete_schedule <profile> <name>\n";
      return;
    }
    sched.deleteSchedule(profName, schedName);
  }

  void cListSchedules(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName;
    if (!(in >> profName))
    {
      out << "[ERROR] Usage: list_schedules <profile>\n";
      return;
    }
    sched.listSchedules(profName);
  }

  void cAddTask(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, schedName, title, date, timeStart, timeEnd, imp;
    if (!(in >> profName >> schedName >> title >> date >> timeStart >> timeEnd >> imp))
    {
      out << "[ERROR] Usage: add_task <profile> <schedule> <title> <date> <time_start> <time_end> <importance>\n";
      return;
    }
    sched.addTask(profName, schedName, title, date, timeStart, timeEnd, imp);
  }

  void cRemoveTask(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, schedName;
    int id;
    if (!(in >> profName >> schedName >> id))
    {
      out << "[ERROR] Usage: remove_task <profile> <schedule> <id>\n";
      return;
    }
    sched.removeTask(profName, schedName, id);
  }

  void cViewTask(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, schedName;
    int id;
    if (!(in >> profName >> schedName >> id))
    {
      out << "[ERROR] Usage: view_task <profile> <schedule> <id>\n";
      return;
    }
    sched.viewTask(profName, schedName, id);
  }

  void cListTasks(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, schedName, date;
    if (!(in >> profName >> schedName >> date))
    {
      out << "[ERROR] Usage: list_tasks <profile> <schedule> <date>\n";
      return;
    }
    sched.listTasks(profName, schedName, date);
  }

  void cShowUnplaced(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName;
    if (!(in >> profName))
    {
      out << "[ERROR] Usage: show_unplaced <profile>\n";
      return;
    }
    sched.showUnplaced(profName);
  }

  void cAutoPlace(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, dateFrom, dateTo;
    if (!(in >> profName >> dateFrom >> dateTo))
    {
      out << "[ERROR] Usage: auto_place <profile> <date_from> <date_to>\n";
      return;
    }
    sched.autoPlace(profName, dateFrom, dateTo);
  }

  void cFindFreeWindow(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string p1, s1, p2, s2, dateFrom, dateTo;
    int min_hours;
    if (!(in >> p1 >> s1 >> p2 >> s2 >> dateFrom >> dateTo >> min_hours))
    {
      out << "[ERROR] Usage: find_free_window <p1> <s1> <p2> <s2> <from> <to> <min_hours>\n";
      return;
    }
    sched.findFreeWindow(p1, s1, p2, s2, dateFrom, dateTo, min_hours);
  }

  void cMergeSchedules(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, newName, s1, s2;
    if (!(in >> profName >> newName >> s1 >> s2))
    {
      out << "[ERROR] Usage: merge_schedules <profile> <new_name> <s1> <s2>\n";
      return;
    }
    sched.mergeSchedules(profName, newName, s1, s2);
  }

  void cCompareMerge(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, s1, s2;
    if (!(in >> profName >> s1 >> s2))
    {
      out << "[ERROR] Usage: compare_merge <profile> <s1> <s2>\n";
      return;
    }
    sched.compareMerge(profName, s1, s2);
  }

  void cOptimizeRange(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string profName, dateFrom, dateTo, criterion;
    if (!(in >> profName >> dateFrom >> dateTo >> criterion))
    {
      out << "[ERROR] Usage: optimize_range <profile> <from> <to> <criterion> [schedules...]\n";
      return;
    }
    std::vector< std::string > schedules;
    std::string schedName;
    while (in >> schedName)
    {
      schedules.push_back(schedName);
    }
    sched.optimizeRange(profName, dateFrom, dateTo, criterion, schedules);
  }

  void cFindBestDay(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string p1, s1, p2, s2;
    int min_hours;
    if (!(in >> p1 >> s1 >> p2 >> s2 >> min_hours))
    {
      out << "[ERROR] Usage: find_best_day <p1> <s1> <p2> <s2> <min_hours>\n";
      return;
    }
    sched.findBestDay(p1, s1, p2, s2, min_hours);
  }

  void cExportProfile(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string name, filename;
    if (!(in >> name >> filename))
    {
      out << "[ERROR] Usage: export_profile <name> <filename>\n";
      return;
    }
    sched.exportProfile(name, filename);
  }

  void cImportProfile(std::istream & in, std::ostream & out, Scheduler & sched)
  {
    std::string filename;
    if (!(in >> filename))
    {
      out << "[ERROR] Usage: import_profile <filename>\n";
      return;
    }
    sched.importProfile(filename);
  }
}
