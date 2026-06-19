#include <boost/test/unit_test.hpp>
#include <sstream>
#include "commands.hpp"

BOOST_AUTO_TEST_CASE(command_add_task_global_test)
{
  zhuravleva::TaskManager manager;
  std::istringstream in("A 10");
  std::ostringstream out;
  zhuravleva::executeCommand("add_task_global", in, out, manager);

  BOOST_CHECK(manager.hasTask("A"));
}

BOOST_AUTO_TEST_CASE(command_create_list_test)
{
  zhuravleva::TaskManager manager;
  std::istringstream in("work 50");
  std::ostringstream out;
  zhuravleva::executeCommand("create_list", in, out, manager);

  BOOST_CHECK(manager.hasList("work"));
}

BOOST_AUTO_TEST_CASE(command_add_task_list_and_show_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.addTaskGlobal("A", 10);
  manager.createList("work", 50);
  std::istringstream in("work A 1");
  zhuravleva::executeCommand("add_task_list", in, out, manager);
  std::istringstream showIn("work");
  zhuravleva::executeCommand("show_list", showIn, out, manager);

  BOOST_CHECK(out.str().find("A 10 1 0") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(command_check_add_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.addTaskGlobal("A", 40);
  manager.addTaskGlobal("B", 20);
  manager.createList("work", 50);
  manager.addTaskToList("work", "A", 1);
  std::istringstream in("work B");
  zhuravleva::executeCommand("check_add", in, out, manager);

  BOOST_TEST(out.str() == "NOT OK\n");
}

BOOST_AUTO_TEST_CASE(command_invalid_test)
{
  zhuravleva::TaskManager manager;
  std::istringstream in;
  std::ostringstream out;

  BOOST_CHECK_THROW(
      zhuravleva::executeCommand("unknown", in, out, manager),
      std::logic_error);
}