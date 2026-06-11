#ifndef FINANCE_SYSTEM_HPP
#define FINANCE_SYSTEM_HPP

#include "profile.hpp"

#include <string>
#include <vector>

namespace ulanova
{
  class FinanceSystem
  {
  public:
    void create_profile(const std::string& name);
    void drop_profile(const std::string& name);
    void add_income(const std::string& name, long long amount, const std::string& date);
    void add_expense(const std::string& name, long long amount, const std::string& date);
    bool has_profile(const std::string& name) const;
    long long get_balance(const std::string& name) const;
  private:
    std::vector< Profile > profiles_;
  };
}


#endif
