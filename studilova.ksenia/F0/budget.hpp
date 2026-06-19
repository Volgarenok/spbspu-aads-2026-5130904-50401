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

      const std::string& getName() const;

    private:
      std::string name_;
  };
}

#endif
