#include "finance_system.hpp"

#include <stdexcept>

void ulanova::FinanceSystem::create_profile(const std::string& name)
{
  if (has_profile(name))
  {
    throw std::logic_error("profile already exists");
  }

  const Saving default_saving{"default", 0, 0, 999, parse_date("01.01.1970")};

  ulanova::Vector< Saving > savings;
  ulanova::Vector< Operation > operations;
  Profile profile{name, 0, savings, operations};
  profile.savings.push_back(default_saving);
  profiles_.add(name, profile);
}

bool ulanova::FinanceSystem::has_profile(const std::string& name) const
{
  return profiles_.has(name);
}

long long ulanova::FinanceSystem::get_balance(const std::string& name, const std::string& date) const
{
  const Date target_date = parse_date(date);
  const Profile* profile = profiles_.find(name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  long long balance = 0;

  for (size_t i = 0; i < profile->operations.getsize(); ++i)
  {
    const Operation& operation = profile->operations[i];

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

void ulanova::FinanceSystem::drop_profile(const std::string& name)
{
  profiles_.drop(name);
}

void ulanova::FinanceSystem::add_income(const std::string& name, long long amount, const std::string& date)
{
  Profile* profile = profiles_.find(name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  Operation operation{amount, parse_date(date), true};
  profile->operations.push_back(operation);
  profile->balance += amount;
}

void ulanova::FinanceSystem::add_expense(const std::string& name, long long amount, const std::string& date)
{
  Profile* profile = profiles_.find(name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  Operation operation{amount, parse_date(date), false};
  profile->operations.push_back(operation);
  profile->balance -= amount;
}

ulanova::Cashflow ulanova::FinanceSystem::get_cashflow(
  const std::string& name,
  const std::string& from_date,
  const std::string& to_date) const
{
  const Date from = parse_date(from_date);
  const Date to = parse_date(to_date);
  const Profile* profile = profiles_.find(name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  Cashflow cashflow{0, 0, 0};

  for (size_t i = 0; i < profile->operations.getsize(); ++i)
  {
    const Operation& operation = profile->operations[i];

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

ulanova::Vector< ulanova::Saving > ulanova::FinanceSystem::get_savings(
  const std::string& name,
  const std::string& date) const
{
  parse_date(date);

  const Profile* profile = profiles_.find(name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  return profile->savings;
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
