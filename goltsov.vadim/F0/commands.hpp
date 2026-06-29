#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <myvector.hpp>
#include "mymap.hpp"
#include "structs.hpp"

namespace goltsov
{
  namespace detail
  {
    std::istream& skipSpaces(std::istream&);
    bool isEndOfLine(std::istream&);
    bool pushTask(State&, Task&, const TimeInterval&);
    bool pushSoftTask(State&, Task&, const TimeInterval&);
    void pushUnplanned(State&);
    MapIterator< DateTime, Task > pushProtectedTask(State&, Task&);
    MapIterator< DateTime, Task > pushProtectedTaskForce(State&, Task&);
    std::pair< size_t, size_t > mergeInterval(State&, MapIterator< DateTime, Task >,
      MapIterator< DateTime, Task >);
    std::pair< DateTime, DateTime > findCommonGapInVector(State&, const DateTime&,
      const DateTime&, const TimeInterval&, const goltsov::Vector< std::string >&);
    template< class Predicate >
    MapIterator< DateTime, Task > findByPredicate(Schedule&, Predicate);
    template< class Predicate >
    MapIterator< DateTime, Task > rfindByPredicate(Schedule&, Predicate);
  }

  void parsingAdd(std::istream&, std::ostream&, State&);
  void parsingAddProtected(std::istream&, std::ostream&, State&);
  void parsingRemove(std::istream&, std::ostream&, State&);
  void parsingList(std::istream&, std::ostream&, State&);
  void parsingMerge(std::istream&, std::ostream&, State&);
  void parsingShowUnplanned(std::istream&, std::ostream&, State&);
  void parsingUnplannedRemove(std::istream&, std::ostream&, State&);
  void parsingUnplannedForce(std::istream&, std::ostream&, State&);
  void parsingMergeScheduleOtherContext(std::istream&, std::ostream&, State&);
  void parsingAddScheduleOtherContext(std::istream&, std::ostream&, State&);
  void parsingAddForceScheduleOtherContext(std::istream&, std::ostream&, State&);
  void parsingSwitchSchedule(std::istream&, std::ostream&, State&);
  void parsingSwitchContext(std::istream&, std::ostream&, State&);
  void parsingStats(std::istream&, std::ostream&, State&);
  void parsingNewSchedule(std::istream&, std::ostream&, State&);
  void parsingNewContext(std::istream&, std::ostream&, State&);
  void parsingLoadSchedule(std::istream&, std::ostream&, State&);
  void parsingSaveSchedule(std::istream&, std::ostream&, State&);
  void parsingLoadContext(std::istream&, std::ostream&, State&);
  void parsingSaveContext(std::istream&, std::ostream&, State&);
  void parsingFindGap(std::istream&, std::ostream&, State&);
  void parsingFindGapOnInterval(std::istream&, std::ostream&, State&);
  void parsingFindCommonGap(std::istream&, std::ostream&, State&);
  void parsingFindCommonGapOnInterval(std::istream&, std::ostream&, State&);
  void parsingExit(std::istream&, std::ostream&, State&);
  void parsingListSchedules(std::istream&, std::ostream&, State&);
  void parsingListContexts(std::istream&, std::ostream&, State&);

  void add(std::ostream&, State&, const std::string&, const std::string&, const std::string&,
    const DateTime&, const DateTime&, const TimeInterval&, const size_t&);
  void addProtected(std::ostream&, State&, const std::string&, const std::string&, const std::string&,
    const DateTime&, const DateTime&);
  void remove(std::ostream&, State&, const std::string&);
  void list(std::ostream&, State&);
  void merge(std::ostream&, State&, const std::string&);
  void showUnplanned(std::ostream&, State&);
  void unplannedRemove(std::ostream&, State&, const std::string&);
  void unplannedForce(std::ostream&, State&, const std::string&);
  void mergeScheduleOtherContext(std::ostream&, State&, const std::string&, const std::string&);
  void addScheduleOtherContext(std::ostream&, State&, const std::string&, const std::string&);
  void addForceScheduleOtherContext(std::ostream&, State&, const std::string&, const std::string&);
  void switchSchedule(std::ostream&, State&, const std::string&);
  void switchContext(std::ostream&, State&, const std::string&);
  void stats(std::ostream&, State&, const DateTime&, const DateTime&);
  void newSchedule(std::ostream&, State&, const std::string&);
  void newContext(std::ostream&, State&, const std::string&);
  void loadSchedule(std::ostream&, State&, const std::string&, const std::string&);
  void saveSchedule(std::ostream&, State&, const std::string&, const std::string&);
  void loadContext(std::ostream&, State&, const std::string&, const std::string&);
  void saveContext(std::ostream&, State&, const std::string&, const std::string&);
  void findGap(std::ostream&, State&, const TimeInterval&);
  void findGapOnInterval(std::ostream&, State&, const DateTime&, const DateTime&,
    const TimeInterval&);
  void findCommonGap(std::ostream&, State&, const TimeInterval&, const size_t&,
    const goltsov::Vector< std::string >&);
  void findCommonGapOnInterval(std::ostream&, State&, const DateTime&, const DateTime&,
    const TimeInterval&, const size_t&, const goltsov::Vector< std::string >&);
  void listSchedules(std::ostream&, State&);
  void listContexts(std::ostream&, State&);
}

#endif
