#include "finance_system.hpp"
#include "../common/vector.hpp"

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
      out << "Баланс: " << system.get_balance(name, date) << "\n";
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

  void cashflow(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string name;
    std::string from_date;
    std::string to_date;
    in >> name >> from_date >> to_date;

    try
    {
      const ulanova::Cashflow result = system.get_cashflow(name, from_date, to_date);

      out << "Доходы: " << result.income << "\n";
      out << "Расходы: " << result.expense << "\n";
      out << "Остаток: " << result.total << "\n";
    }
    catch (const std::logic_error&)
    {
      out << "Профиль не существует\n";
    }
  }

  void show_savings(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string name;
    std::string date;
    in >> name >> date;

    try
    {
      const ulanova::Vector< ulanova::Saving > savings = system.get_savings(name, date);

      if (savings.isEmpty())
      {
        out << "Нет накопительных счетов\n";
        return;
      }

      for (size_t i = 0; i < savings.getsize(); ++i)
      {
        out << savings[i].name << " ";
        out << savings[i].current_sum << " ";
        out << savings[i].target_sum << " ";
        out << savings[i].priority << " приоритет\n";
      }
    }
    catch (const std::logic_error&)
    {
      out << "Профиль не существует\n";
    }
  }

  void create_saving(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string saving_name;
    std::string profile_name;
    long long target_sum = 0;
    int priority = 0;
    std::string start_date;

    in >> saving_name >> profile_name >> target_sum >> priority >> start_date;

    try
    {
      system.create_saving(saving_name, profile_name, target_sum, priority, start_date);
    }
    catch (const std::logic_error& error)
    {
      const std::string message = error.what();

      if (message == "saving already exists")
      {
        out << "Счет уже существует\n";
      }
      else
      {
        out << "Профиль не существует\n";
      }
    }

  }

  void finish_saving(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string saving_name;
    std::string date;
    in >> saving_name >> date;

    try
    {
      system.finish_saving(saving_name, date);
      out << "Цель достигнута. Счет " << saving_name << " закрыт ";
      out << date << "\n";
    }
    catch (const std::logic_error&)
    {
      out << "Счет не существует\n";
    }
  }

  void close_saving(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string saving_name;
    std::string date;
    in >> saving_name >> date;

    try
    {
      system.close_saving(saving_name, date);
    }
    catch (const std::logic_error&)
    {
      out << "Счет не существует\n";
    }
  }

  void calce_date(std::istream& in, std::ostream& out, ulanova::FinanceSystem& system)
  {
    std::string profile_name;
    std::string saving_name;
    std::string from_date;
    std::string to_date;

    in >> profile_name >> saving_name >> from_date >> to_date;

    try
    {
      const std::string result = system.calculate_goal_date(profile_name,
        saving_name, from_date, to_date);
      out << "Цель будет достигнута: " << result << "\n";
    }
    catch (const std::logic_error& error)
    {
      const std::string message = error.what();

      if (message == "saving not found")
      {
        out << "Счет не существует\n";
      }
      else
      {
        out << "Профиль не существует\n";
      }
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
  commands["cashflow"] = cashflow;
  commands["show-savings"] = show_savings;
  commands["create-saving"] = create_saving;
  commands["finish-saving"] = finish_saving;
  commands["close-saving"] = close_saving;
  commands["calc-date"] = calce_date;

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
