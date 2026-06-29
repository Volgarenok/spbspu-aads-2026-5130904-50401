#include <iostream>
#include <limits>
#include "mymap.hpp"
#include "structs.hpp"
#include "commands.hpp"

int main()
{
  goltsov::DateTime current_time;
  std::cout << "Enter the current time: ";
  std::cin >> current_time;
  while (std::cin.eof() || std::cin.fail())
  {
    if (std::cin.eof())
    {
      return 0;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    std::cout << "Incorrect format, correct format: \"YYYY-MM-DD_HH:MM:SS\". Enter again: ";
    std::cin >> current_time;
  }
  goltsov::State current_state {nullptr, nullptr, goltsov::Map< std::string, goltsov::Context >{}, current_time};
  current_state.contexts.insert({"Base_context", goltsov::Context {}});
  current_state.contexts.at("Base_context").name_context = "Base_context";
  current_state.current_context = &current_state.contexts.at("Base_context");
  current_state.current_context->schedules.insert({"Base_schedule", goltsov::Schedule {}});
  current_state.current_context->schedules.at("Base_schedule").name_schedule = "Base_schedule";
  current_state.current_schedule = &current_state.current_context->schedules.at("Base_schedule");

  goltsov::Map< std::string, void (*)(std::istream&, std::ostream&, goltsov::State&) > commands;
  commands["add"] = goltsov::parsingAdd;
  commands["add_protected"] = goltsov::parsingAddProtected;
  commands["remove"] = goltsov::parsingRemove;
  commands["list"] = goltsov::parsingList;
  commands["merge"] = goltsov::parsingMerge;
  commands["show_unplanned"] = goltsov::parsingShowUnplanned;
  commands["unplanned_remove"] = goltsov::parsingUnplannedRemove;
  commands["unplanned_force"] = goltsov::parsingUnplannedForce;
  commands["merge_schedule_other_context"] = goltsov::parsingMergeScheduleOtherContext;
  commands["add_schedule_other_context"] = goltsov::parsingAddScheduleOtherContext;
  commands["add_force_schedule_other_context"] = goltsov::parsingAddForceScheduleOtherContext;
  commands["switch_schedule"] = goltsov::parsingSwitchSchedule;
  commands["switch_context"] = goltsov::parsingSwitchContext;
  commands["stats"] = goltsov::parsingStats;
  commands["new_schedule"] = goltsov::parsingNewSchedule;
  commands["new_context"] = goltsov::parsingNewContext;
  commands["load_schedule"] = goltsov::parsingLoadSchedule;
  commands["save_schedule"] = goltsov::parsingSaveSchedule;
  commands["load_context"] = goltsov::parsingLoadContext;
  commands["save_context"] = goltsov::parsingSaveContext;
  commands["find_gap"] = goltsov::parsingFindGap;
  commands["find_gap_on_interval"] = goltsov::parsingFindGapOnInterval;
  commands["find_common_gap"] = goltsov::parsingFindCommonGap;
  commands["find_common_gap_on_interval"] = goltsov::parsingFindCommonGapOnInterval;
  commands["exit"] = goltsov::parsingExit;
  commands["list_schedules"] = goltsov::parsingListSchedules;
  commands["list_contexts"] = goltsov::parsingListContexts;

  while(!std::cin.eof())
  {
    std::string command;
    if (std::cin >> command)
    {
      if (!commands.count(command))
      {
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }
      try
      {
       commands[command](std::cin, std::cout, current_state);
      }
      catch(const std::exception& e)
      {
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cerr << e.what() << '\n';
        if (std::cin.fail() && !std::cin.eof())
        {
          std::cin.clear();
        }
      }
    }
    else
    {
      if (!std::cin.eof())
      {
        std::cin.clear();
      }
    }
  }
}
