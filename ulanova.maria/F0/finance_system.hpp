#ifndef FINANCE_SYSTEM_HPP
#define FINANCE_SYSTEM_HPP

#include "profile.hpp"
#include "../common/vector.hpp"
#include "hash_table.hpp"

#include <string>

namespace ulanova
{
  struct Cashflow
  {
    long long income;
    long long expense;
    long long total;
  };

  class FinanceSystem
  {
  public:
    void create_profile(const std::string& name);
    void drop_profile(const std::string& name);
    void add_income(const std::string& name, long long amount, const std::string& date);
    void add_expense(const std::string& name, long long amount, const std::string& date);
    void create_saving(const std::string& saving_name, const std::string& profile_name,
      long long target_sum, int priority,
      const std::string& start_date);
    void finish_saving(const std::string& saving_name, const std::string& date);
    void close_saving(const std::string& saving_name, const std::string& date);
    bool has_profile(const std::string& name) const;
    long long get_balance(const std::string& name, const std::string& date) const;
    Cashflow get_cashflow(const std::string& name, const std::string& from_date, const std::string& to_date) const;
    Vector< Saving > get_savings(const std::string& name, const std::string& date) const;
    std::string calculate_goal_date(const std::string& profile_name,
      const std::string& saving_name,
      const std::string& from_date,
      const std::string& to_date) const;
    std::string recommend_priority(const std::string& profile_name,
      const std::string& saving_name,
      const std::string& from_date,
      const std::string& to_date,
      int& priority) const;
  private:
    HashTable< Profile > profiles_;
  };
}


#endif
