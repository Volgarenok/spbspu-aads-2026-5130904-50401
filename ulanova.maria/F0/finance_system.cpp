#include "finance_system.hpp"

#include <stdexcept>

namespace
{
  std::string calculate_goal_date_with_savings(const ulanova::Profile& profile,
    ulanova::Vector< ulanova::Saving > savings,
    const std::string& saving_name,
    const std::string& from_date,
    const std::string& to_date)
  {
    const ulanova::Date from = ulanova::parse_date(from_date);
    const ulanova::Date to = ulanova::parse_date(to_date);
    size_t target_index = savings.getsize();

    for (size_t i = 0; i < savings.getsize(); ++i)
    {
      if (savings[i].name == saving_name)
      {
        target_index = i;
      }
    }

    if (target_index == savings.getsize())
    {
      throw std::logic_error("saving not found");
    }

    if (savings[target_index].current_sum >= savings[target_index].target_sum)
    {
      return ulanova::date_to_string(from);
    }

    long long total_income = 0;
    long long income_count = 0;
    for (size_t i = 0; i < profile.operations.getsize(); ++i)
    {
      const ulanova::Operation& op = profile.operations[i];
      if (op.is_income)
      {
        total_income += op.amount;
        ++income_count;
      }
    }

    if (income_count == 0 || total_income == 0)
    {
      return "не достигнута";
    }

    const long long avg_income = total_income / income_count;

    const int target_priority = savings[target_index].priority;
    long long higher_priority_need = 0;
    for (size_t i = 0; i < savings.getsize(); ++i)
    {
      if (i == target_index) continue;
      if (savings[i].name == "default") continue;
      if (savings[i].priority < target_priority)
      {
        const long long need = savings[i].target_sum - savings[i].current_sum;
        if (need > 0)
        {
          higher_priority_need += need;
        }
      }
    }

    const long long our_need = savings[target_index].target_sum - savings[target_index].current_sum;
    const long long total_need = higher_priority_need + our_need;
    const long long periods = (total_need + avg_income - 1) / avg_income;

    ulanova::Date result_date = from;
    for (long long p = 0; p < periods; ++p)
    {
      result_date = ulanova::add_days(result_date, 30);
    }

    if (!ulanova::is_before_or_equal(result_date, to))
    {
      return "не достигнута";
    }

    return ulanova::date_to_string(result_date);
  }

  void distribute_income(ulanova::Profile& profile, long long amount, const ulanova::Date& date)
  {
    long long remaining = amount;

    for (int priority = 0; priority <= 999 && remaining > 0; ++priority)
    {
      for (size_t i = 0; i < profile.savings.getsize() && remaining > 0; ++i)
      {
        ulanova::Saving& saving = profile.savings[i];

        if (saving.priority != priority) continue;
        if (saving.name == "default") continue;
        if (!ulanova::is_before_or_equal(saving.start_date, date)) continue;

        const long long need = saving.target_sum - saving.current_sum;
        if (need <= 0) continue;

        const long long deposit = (remaining < need) ? remaining : need;
        saving.current_sum += deposit;
        remaining -= deposit;

        ulanova::Operation op{deposit, date, false};
        profile.operations.push_back(op);
        profile.balance -= deposit;
      }
    }
  }
}

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

  const Date parsed_date = parse_date(date);
  Operation operation{amount, parsed_date, true};
  profile->operations.push_back(operation);
  profile->balance += amount;

  distribute_income(*profile, amount, parsed_date);
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
  const Date target = parse_date(date);

  const Profile* profile = profiles_.find(name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  ulanova::Vector< Saving > result;
  for (size_t i = 0; i < profile->savings.getsize(); ++i)
  {
    if (profile->savings[i].name == "default") continue;
    if (is_before_or_equal(profile->savings[i].start_date, target))
    {
      result.push_back(profile->savings[i]);
    }
  }
  return result;
}

void ulanova::FinanceSystem::create_saving(const std::string& saving_name,
  const std::string& profile_name,
  long long target_sum,
  int priority,
  const std::string& start_date)
{
  Profile* profile = profiles_.find(profile_name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  const Date parsed_start_date = parse_date(start_date);

  for (size_t i = 0; i < profile->savings.getsize(); ++i)
  {
    if (profile->savings[i].name == saving_name)
    {
      throw std::logic_error("saving already exists");
    }
  }

  Saving saving{saving_name, 0 , target_sum, priority, parsed_start_date};
  profile->savings.push_back(saving);
  const long long free_balance = profile->balance;
  if (free_balance > 0)
  {
    distribute_income(*profile, free_balance, parsed_start_date);
  }
}

void ulanova::FinanceSystem::finish_saving(const std::string& profile_name, const std::string& saving_name, const std::string& date)
{
  parse_date(date);
  Profile* profile = profiles_.find(profile_name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  for (size_t j = 0; j < profile->savings.getsize(); ++j)
  {
    if (profile->savings[j].name == saving_name)
    {
      if (saving_name == "default")
      {
        throw std::logic_error("system saving");
      }
      profile->savings.erase(j);
      return;
    }
  }

  throw std::logic_error("saving not found");
}

void ulanova::FinanceSystem::close_saving(const std::string& profile_name, const std::string& saving_name, const std::string& date)
{
   const Date parsed_date = parse_date(date);
  Profile* profile = profiles_.find(profile_name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  for (size_t j = 0; j < profile->savings.getsize(); ++j)
  {
    if (profile->savings[j].name == saving_name)
    {
      if (saving_name == "default")
      {
        throw std::logic_error("system saving");
      }
      const long long returned = profile->savings[j].current_sum;
      if (returned > 0)
      {
        Operation op{returned, parsed_date, true};
        profile->operations.push_back(op);
        profile->balance += returned;
      }
      profile->savings.erase(j);
      return;
    }
  }

  throw std::logic_error("saving not found");
}

std::string ulanova::FinanceSystem::calculate_goal_date(
  const std::string& profile_name,
  const std::string& saving_name,
  const std::string& from_date,
  const std::string& to_date) const
{
  const Profile* profile = profiles_.find(profile_name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  return calculate_goal_date_with_savings(*profile,
    profile->savings, saving_name, from_date, to_date);
}

std::string ulanova::FinanceSystem::recommend_priority(
  const std::string& profile_name,
  const std::string& saving_name,
  const std::string& from_date,
  const std::string& to_date,
  int& priority) const
{
  const Profile* profile = profiles_.find(profile_name);

  if (profile == nullptr)
  {
    throw std::logic_error("profile not found");
  }

  bool has_saving = false;

  for (size_t i = 0; i < profile->savings.getsize(); ++i)
  {
    if (profile->savings[i].name == saving_name)
    {
      has_saving = true;
    }
  }

  if (!has_saving)
  {
    throw std::logic_error("saving not found");
  }

  std::string best_date = "не достигнута";
  int best_priority = 0;
  for (size_t i = 0; i < profile->savings.getsize(); ++i)
  {
    if (profile->savings[i].name == saving_name)
    {
      best_priority = profile->savings[i].priority;
    }
  }

  for (int current_priority = 0; current_priority <= 10; ++current_priority)
  {
    Vector< Saving > savings = profile->savings;

    for (size_t i = 0; i < savings.getsize(); ++i)
    {
      if (savings[i].name == saving_name)
      {
        savings[i].priority = current_priority;
      }
    }

    const std::string result = calculate_goal_date_with_savings(*profile,
      savings, saving_name, from_date, to_date);

    if (result != "не достигнута" )
    {
      if (best_date == "не достигнута" || ulanova::parse_date(result) < ulanova::parse_date(best_date))
      {
        best_date = result;
        best_priority = current_priority;
      }
    }
  }

  priority = best_priority;
  return best_date;
}

