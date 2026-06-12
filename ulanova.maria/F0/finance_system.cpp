#include "finance_system.hpp"

#include <stdexcept>

void ulanova::FinanceSystem::create_profile(const std::string& name)
{
  if (has_profile(name))
  {
    throw std::logic_error("profile already exists");
  }

  const Saving default_saving{"default", 0, 0, 999};
  Profile profile{name, 0, {default_saving}, {}};
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

long long ulanova::FinanceSystem::get_balance(const std::string& name, const std::string& date) const
{
  const Date target_date = parse_date(date);

  for (size_t i = 0; i < profiles_.size(); ++i)
  {
    if (profiles_[i].name == name)
    {
      long long balance = 0;

      for (size_t j = 0; j < profiles_[i].operations.size(); ++j)
      {
        const Operation& operation = profiles_[i].operations[j];

        if (is_before_or_equal(operation.date, target_date))
        {
          if (operation.is_income)
          {
            balance += operation.amount;
          }
          else
          {
            balance -= operation.amount;
          }
        }
      }
      return balance;
    }
  }
  throw std::logic_error("profile not found");
}

void ulanova::FinanceSystem::drop_profile(const std::string& name)
{
  for (size_t i = 0; i < profiles_.size(); ++i)
  {
    if (profiles_[i].name == name)
    {
      profiles_.erase(profiles_.begin() + i);
      return;
    }
  }

  throw std::logic_error("profile not found");
}

void ulanova::FinanceSystem::add_income(const std::string& name, long long amount, const std::string& date)
{
  for (size_t i = 0; i < profiles_.size(); ++i)
  {
    if (profiles_[i].name == name)
    {
      Operation operation{amount, parse_date(date), true};
      profiles_[i].operations.push_back(operation);
      profiles_[i].balance += amount;
      return;
    }
  }

  throw std::logic_error("profile not found");
}

void ulanova::FinanceSystem::add_expense(const std::string& name, long long amount, const std::string& date)
{
  for (size_t i = 0; i < profiles_.size(); ++i)
  {
    if (profiles_[i].name == name)
    {
      Operation operation{amount, parse_date(date), false};
      profiles_[i].operations.push_back(operation);
      profiles_[i].balance -= amount;
      return;
    }
  }

  throw std::logic_error("profile not found");
}
