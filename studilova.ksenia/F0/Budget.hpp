#ifndef BUDGET_HPP
#define BUDGET_HPP

#include <string>

namespace studilova
{
  class Budget
  {
    public:
      Budget();
      explicit Budget(const std::string& name);

    private:
      std::string name_;
  };
}

#endif
