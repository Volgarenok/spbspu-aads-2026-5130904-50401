#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>

#include "date.hpp"
#include "finance_system.hpp"

BOOST_AUTO_TEST_CASE(create_profile_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK(!system.has_profile("Мария"));
  system.create_profile("Мария");
  BOOST_CHECK(system.has_profile("Мария"));
}

BOOST_AUTO_TEST_CASE(create_profile_duplicate_throws_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  BOOST_CHECK_THROW(system.create_profile("Мария"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(drop_profile_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.drop_profile("Мария");
  BOOST_CHECK(!system.has_profile("Мария"));
}

BOOST_AUTO_TEST_CASE(drop_profile_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK_THROW(system.drop_profile("Мария"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(get_balance_empty_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  BOOST_CHECK(system.get_balance("Мария", "01.01.2026") == 0);
}

BOOST_AUTO_TEST_CASE(get_balance_after_income_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 5000, "01.04.2026");
  BOOST_CHECK(system.get_balance("Мария", "01.04.2026") == 5000);
}

BOOST_AUTO_TEST_CASE(get_balance_income_minus_expense_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 5000, "01.04.2026");
  system.add_expense("Мария", 2000, "02.04.2026");
  BOOST_CHECK(system.get_balance("Мария", "02.04.2026") == 3000);
}

BOOST_AUTO_TEST_CASE(get_balance_date_filter_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 5000, "01.04.2026");
  system.add_income("Мария", 3000, "01.05.2026");
  BOOST_CHECK(system.get_balance("Мария", "01.04.2026") == 5000);
  BOOST_CHECK(system.get_balance("Мария", "01.05.2026") == 8000);
}

BOOST_AUTO_TEST_CASE(get_balance_profile_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK_THROW(system.get_balance("Мария", "01.04.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(add_income_profile_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK_THROW(system.add_income("Мария", 5000, "01.04.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(add_expense_profile_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK_THROW(system.add_expense("Мария", 5000, "01.04.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(get_cashflow_basic_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 10000, "01.04.2026");
  system.add_expense("Мария", 3000, "15.04.2026");

  const ulanova::Cashflow cf = system.get_cashflow("Мария", "01.04.2026", "30.04.2026");
  BOOST_CHECK(cf.income == 10000);
  BOOST_CHECK(cf.expense == 3000);
  BOOST_CHECK(cf.total == 7000);
}

BOOST_AUTO_TEST_CASE(get_cashflow_date_filter_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 10000, "01.03.2026");
  system.add_income("Мария", 5000, "01.04.2026");

  const ulanova::Cashflow cf = system.get_cashflow("Мария", "01.04.2026", "30.04.2026");
  BOOST_CHECK(cf.income == 5000);
}

BOOST_AUTO_TEST_CASE(get_cashflow_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK_THROW(system.get_cashflow("Мария", "01.04.2026", "30.04.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(create_saving_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.create_saving("Отпуск", "Мария", 45000, 1, "01.04.2026");

  const auto savings = system.get_savings("Мария", "01.04.2026");
  bool found = false;
  for (size_t i = 0; i < savings.getsize(); ++i)
  {
    if (savings[i].name == "Отпуск") found = true;
  }
  BOOST_CHECK(found);
}

BOOST_AUTO_TEST_CASE(create_saving_duplicate_throws_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.create_saving("Отпуск", "Мария", 45000, 1, "01.04.2026");
  BOOST_CHECK_THROW(system.create_saving("Отпуск", "Мария", 45000, 1, "01.04.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(create_saving_profile_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK_THROW(system.create_saving("Отпуск", "Мария", 45000, 1, "01.04.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(get_savings_date_filter_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.create_saving("Отпуск", "Мария", 45000, 1, "01.05.2026");

  const auto before = system.get_savings("Мария", "01.04.2026");
  const auto after  = system.get_savings("Мария", "01.05.2026");

  BOOST_CHECK(before.getsize() == 1);
  BOOST_CHECK(after.getsize() == 2);
}

BOOST_AUTO_TEST_CASE(finish_saving_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.create_saving("Отпуск", "Мария", 45000, 1, "01.04.2026");
  system.finish_saving("Мария", "Отпуск", "30.06.2026");

  const auto savings = system.get_savings("Мария", "30.06.2026");
  bool found = false;
  for (size_t i = 0; i < savings.getsize(); ++i)
  {
    if (savings[i].name == "Отпуск") found = true;
  }
  BOOST_CHECK(!found);
}

BOOST_AUTO_TEST_CASE(finish_saving_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  BOOST_CHECK_THROW(system.finish_saving("Мария", "Отпуск", "30.06.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(finish_saving_default_throws_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  BOOST_CHECK_THROW(system.finish_saving("Мария", "default", "30.06.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(calculate_goal_date_achievable_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 10000, "01.04.2026");
  system.add_income("Мария", 10000, "01.05.2026");
  system.add_income("Мария", 10000, "01.06.2026");
  system.create_saving("Отпуск", "Мария", 25000, 0, "01.04.2026");

  const std::string result = system.calculate_goal_date("Мария", "Отпуск", "01.04.2026", "31.12.2026");
  BOOST_CHECK(result != "не достигнута");
}

BOOST_AUTO_TEST_CASE(calculate_goal_date_not_achievable_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 1000, "01.04.2026");
  system.create_saving("Отпуск", "Мария", 1000000, 0, "01.04.2026");

  const std::string result = system.calculate_goal_date("Мария", "Отпуск", "01.04.2026", "30.04.2026");
  BOOST_CHECK(result == "не достигнута");
}

BOOST_AUTO_TEST_CASE(calculate_goal_date_priority_order_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 10000, "01.04.2026");
  system.add_income("Мария", 10000, "01.05.2026");
  system.add_income("Мария", 10000, "01.06.2026");
  system.create_saving("Высокий", "Мария", 15000, 0, "01.04.2026");
  system.create_saving("Низкий",  "Мария", 15000, 1, "01.04.2026");

  const std::string high = system.calculate_goal_date("Мария", "Высокий", "01.04.2026", "31.12.2026");
  const std::string low  = system.calculate_goal_date("Мария", "Низкий",  "01.04.2026", "31.12.2026");

  BOOST_CHECK(high != "не достигнута");
  BOOST_CHECK(ulanova::parse_date(high) <= ulanova::parse_date(low));
}

BOOST_AUTO_TEST_CASE(calculate_goal_date_profile_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  BOOST_CHECK_THROW(system.calculate_goal_date("Мария", "Отпуск", "01.04.2026", "31.12.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(calculate_goal_date_saving_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  BOOST_CHECK_THROW(system.calculate_goal_date("Мария", "Отпуск", "01.04.2026", "31.12.2026"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(recommend_priority_returns_valid_date_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  system.add_income("Мария", 10000, "01.04.2026");
  system.add_income("Мария", 10000, "01.05.2026");
  system.add_income("Мария", 10000, "01.06.2026");
  system.create_saving("Отпуск", "Мария", 20000, 5, "01.04.2026");

  int priority = 5;
  const std::string date = system.recommend_priority("Мария", "Отпуск", "01.04.2026", "31.12.2026", priority);

  BOOST_CHECK(date != "не достигнута");
  BOOST_CHECK(priority <= 5);
}

BOOST_AUTO_TEST_CASE(recommend_priority_profile_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  int priority = 0;
  BOOST_CHECK_THROW(system.recommend_priority("Мария", "Отпуск", "01.04.2026", "31.12.2026", priority), std::logic_error);
}

BOOST_AUTO_TEST_CASE(recommend_priority_saving_not_found_throws_test)
{
  ulanova::FinanceSystem system;
  system.create_profile("Мария");
  int priority = 0;
  BOOST_CHECK_THROW(system.recommend_priority("Мария", "Отпуск", "01.04.2026", "31.12.2026", priority), std::logic_error);
}

