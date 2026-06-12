#include "finance_system.hpp"

#include <stdexcept>

void ulanova::FinanceSystem::create_profile(const std::string& name)
{
  if (has_profile(name))
  {
    throw std::logic_error("profile already exists");
  }

  const Saving default_saving{"default", 0, 0, 999, parse_date("01.01.1970")};
  Profile profile{name, 0, ulanova::Vector< Saving >{}, ulanova::Vector< Operation >{}};
  profile.savings.push_back(default_saving);
  profiles_.push_back(profile);
}

bool ulanova::FinanceSystem::has_profile(const std::string& name) const
{
  for (size_t i = 0; i < profiles_.getsize(); ++i)
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

  for (size_t i = 0; i < profiles_.getsize(); ++i)
  {
    if (profiles_[i].name == name)
    {
      long long balance = 0;

      for (size_t j = 0; j < profiles_[i].operations.getsize(); ++j)
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
  for (size_t i = 0; i < profiles_.getsize(); ++i)
  {
    if (profiles_[i].name == name)
    {
      profiles_.erase(i);
      return;
    }
  }

  throw std::logic_error("profile not found");
}

void ulanova::FinanceSystem::add_income(const std::string& name, long long amount, const std::string& date)
{
  for (size_t i = 0; i < profiles_.getsize(); ++i)
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
  for (size_t i = 0; i < profiles_.getsize(); ++i)
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

ulanova::Cashflow ulanova::FinanceSystem::get_cashflow(
  const std::string& name,
  const std::string& from_date,
  const std::string& to_date) const
{
  const Date from = parse_date(from_date);
  const Date to = parse_date(to_date);

  for (size_t i = 0; i < profiles_.getsize(); ++i)
  {
    if (profiles_[i].name == name)
    {
      Cashflow cashflow{0, 0, 0};

      for (size_t j = 0; j < profiles_[i].operations.getsize(); ++j)
      {
        const Operation& operation = profiles_[i].operations[j];

        if (is_after_or_equal(operation.date, from) && is_before_or_equal(operation.date, to))
        {
          if (operation.is_income)
          {
            cashflow.income += operation.amount;
          }
          else
          {
            cashflow.expense += operation.amount;
          }
        }
      }

      cashflow.total = cashflow.income - cashflow.expense;
      return cashflow;
    }
  }

  throw std::logic_error("profile not found");
}

ulanova::Vector< ulanova::Saving > ulanova::FinanceSystem::get_savings(
  const std::string& name,
  const std::string& date) const
{
  parse_date(date);

  for (size_t i = 0; i < profiles_.getsize(); ++i)
  {
    if (profiles_[i].name == name)
    {
      return profiles_[i].savings;
    }
  }

  throw std::logic_error("profile not found");
}

void ulanova::FinanceSystem::create_saving(const std::string& saving_name,
  const std::string& profile_name,
  long long target_sum,
  int priority,
  const std::string& start_date)
{
  const Date parsed_start_date = parse_date(start_date);

  for (size_t i = 0; i < profiles_.getsize(); ++i)
  {
    if (profiles_[i].name == profile_name)
    {
      for (size_t j = 0; j < profiles_[i].savings.getsize(); ++j)
      {
        if (profiles_[i].savings[j].name == saving_name)
        {
          throw std::logic_error("saving already exists");
        }
      }

      Saving saving{saving_name, 0 , target_sum, priority, parsed_start_date};
      profiles_[i].savings.push_back(saving);
      return;
    }
  }

  throw std::logic_error("profile not found");
}

void ulanova::FinanceSystem::finish_saving(const std::string& saving_name, const std::string& date)
{
  parse_date(date);

  for (size_t i = 0; i < profiles_.getsize(); ++i)
  {
    for (size_t j = 0; j < profiles_[i].savings.getsize(); ++j)
    {
      if (profiles_[i].savings[j].name == saving_name)
      {
        if (saving_name == "default")
        {
          throw std::logic_error("system saving");
        }
        profiles_[i].savings.erase(j);
        return;
      }
    }
  }

  throw std::logic_error("saving not found");
}

void ulanova::FinanceSystem::close_saving(const std::string& saving_name, const std::string& date)
{
  parse_date(date);

  for (size_t i = 0; i < profiles_.getsize(); ++i)
  {
    for (size_t j = 0; j < profiles_.getsize(); ++j)
    {
      if (profiles_[i].savings[j].name == saving_name)
      {
        if (saving_name == "default")
        {
          throw std::logic_error("system saving");
        }

        profiles_[i].balance += profiles_[i].savings[j].current_sum;
        profiles_[i].savings.erase( j);
        return;
      }
    }
  }

  throw std::logic_error("saving not found");
}
