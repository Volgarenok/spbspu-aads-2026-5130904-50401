#include "finance_system.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace
{
  using command_t = void (*)(std::istream&, std::ostream&, ulanova::FinanceSystem&);

  void create_profile(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string name;
    in >> name;

    try
    {
      system.create_profile(name);
    }
    catch (const std::logic_error&)
    {
      out << "Профиль уже существует\n";
    }
  }

  void show_balance(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string name;
    std::string date;
    in >> name >> date;

    try
    {
      out << "Баланс: " << system.get_balance(name) << "\n";
    }
    catch (const std::logic_error&)
    {
      out << "Профиль не существует\n";
    }
  }

  void drop_profile(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string name;
    in >> name;

    try
    {
      system.drop_profile(name);
    }
    catch (const std::logic_error&)
    {
      out << "Профиль не найден\n";
    }
  }

  void add_income(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string name;
    long long amount = 0;
    std::string date;
    in >> name >> amount >> date;

    try
    {
      system.add_income(name, amount, date);
    }
    catch (const std::logic_error&)
    {
      out << "Профиль не существует\n";
    }
  }

  void add_expense(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string name;
    long long amount = 0;
    std::string date;
    in >> name >> amount >> date;

    try
    {
      system.add_expense(name, amount, date);
    }
    catch (const std::logic_error&)
    {
      out << "Профиль не существует\n";
    }
  }

  void invalid_command(std::istream& in, std::ostream& out)
  {
    out << "<INVALID COMMAND>\n";

    const std::streamsize max_size = std::numeric_limits< std::streamsize >::max();
    in.ignore(max_size, '\n');
  }
}

int main()
{
  using commands_t = std::unordered_map< std::string, command_t >;

  ulanova::FinanceSystem system;
  commands_t commands;
  commands["create-profile"] = create_profile;
  commands["show-balance"] = show_balance;
  commands["drop-profile"] = drop_profile;
  commands["add-income"] = add_income;
  commands["add-expense"] = add_expense;

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)(std::cin, std::cout, system);
    }
    catch (const std::out_of_range&)
    {
      invalid_command(std::cin, std::cout);
    }
    catch (const std::logic_error&)
    {
      invalid_command(std::cin, std::cout);
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "bad input\n";
    return 1;
  }

  return 0;
}
