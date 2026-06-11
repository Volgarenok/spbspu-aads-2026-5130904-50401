#include "finance_system.hpp"

#include <stdexcept>

void ulanova::FinanceSystem::create_profile(const std::string& name)
{
  if (has_profile(name))
  {
    throw std::logic_error("profile already exists");
  }

  Profile profile{name, 0};
  profiles_.push_back(profile);
}

bool ulanova::FinanceSystem::has_profile(const std::string& name) const
{
  for (size_t i = 0; i < profiles_.size(); ++i)
  {
    if (profiles_[i].name == name)
    {
      return true;
    }
  }

  return false;
}

