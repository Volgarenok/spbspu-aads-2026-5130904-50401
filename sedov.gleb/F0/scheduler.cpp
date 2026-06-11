#include "scheduler.hpp"

namespace sedov
{
  Scheduler::Scheduler() noexcept
  {
    currentProfile_.clear();
  }

  bool Scheduler::createProfile(const std::string & name)
  {
    if (name.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    for (size_t i = 0; i < name.size(); ++i)
    {
      char c = name[i];
      if (!std::isalnum(c) && c != '_')
      {
        std::cout << "[ERROR] Profile name contains invalid characters. Use letters, digits, and underscores only\n";
        return false;
      }
    }
    if (profiles_.contains(ProfileKey{name}))
    {
      std::cout << "[ERROR] Profile \"" << name << "\" already exists\n";
      return false;
    }
    profiles_.insert(ProfileKey{name}, Profile(name));
    std::cout << "[OK] Profile \"" << name << "\" created\n";
    return true;
  }

  bool Scheduler::deleteProfile(const std::string& name)
  {
    if (name.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    if (!profiles_.erase(ProfileKey{name}))
    {
      std::cout << "[ERROR] Profile \"" << name << "\" not found\n";
      return false;
    }
    std::cout << "[OK] Profile \"" << name << "\" deleted\n";
    return true;
  }

  void Scheduler::listProfiles() const
  {
    List< std::pair< const ProfileKey, Profile > > raw;
    profiles_.getAll(raw);
    std::cout << "Profile list:\n";
    int i = 1;
    for (auto it = raw.begin(); it != raw.end(); ++it, ++i)
    {
      std::cout << "  " << i << ". " << (*it).second.getName() << "\n";
    }
  }

  bool Scheduler::createSchedule(const std::string & profName, const std::string & schedName)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    if (profile.scheduleExists(schedName))
    {
      std::cout << "[ERROR] Schedule \"" << schedName << "\" already exists\n";
      return false;
    }
    profile.addSchedule(schedName);
    profiles_.insert(ProfileKey{profName}, profile);
    std::cout << "[OK] Schedule \"" << schedName << "\" created in profile \"" << profName << "\"\n";
    return true;
  }

  bool Scheduler::deleteSchedule(const std::string & profName, const std::string & schedName)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    if (!profile.removeSchedule(schedName))
    {
      std::cout << "[ERROR] Schedule \"" << schedName << "\" not found\n";
      return false;
    }
    profiles_.insert(ProfileKey{profName}, profile);
    std::cout << "[OK] Schedule \"" << schedName << "\" deleted from profile \"" << profName << "\"\n";
    return true;
  }

  bool Scheduler::listSchedules(const std::string & profName)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    List< Schedule > scheds;
    profile.getAllSchedules(scheds);
    std::cout << "Schedules in profile \"" << profName << "\":\n";
    if (scheds.size() == 0)
    {
        std::cout << "  No schedules\n";
    }
    else
    {
      for (auto it = scheds.begin(); it != scheds.end(); ++it)
      {
        std::cout << "  - " << (*it).getName() << " (" << (*it).taskCount() << " tasks)\n";
      }
    }
    return true;
  }
}
