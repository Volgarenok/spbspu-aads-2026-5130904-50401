#include <boost/test/unit_test.hpp>
#include <sstream>
#include "taskManager.hpp"

BOOST_AUTO_TEST_CASE(manager_basic_operations_test)
{
  zhuravleva::TaskManager manager;
  manager.addTaskGlobal("task1", 5);
  manager.addTaskGlobal("task2", 10);
  manager.createList("study", 20);
  manager.addTaskToList("study", "task1", 1);
  manager.addTaskToList("study", "task2", 3);

  BOOST_TEST(manager.hasTask("task1"));
  BOOST_TEST(manager.hasList("study"));
  BOOST_TEST(manager.hasTaskInList("study", "task1"));
  BOOST_TEST(manager.getCurrentLabor("study") == 15);
  BOOST_TEST(manager.getFreeLabor("study") == 5);
}

BOOST_AUTO_TEST_CASE(manager_check_add_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.addTaskGlobal("task1", 8);
  manager.addTaskGlobal("task2", 5);
  manager.createList("work", 10);
  manager.addTaskToList("work", "task1", 1);
  manager.checkAdd("work", "task2", out);

  BOOST_TEST(out.str() == "NOT OK\n");
}

BOOST_AUTO_TEST_CASE(manager_deadline_expired_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.addTaskGlobal("task1", 5);
  manager.createList("study", 20);
  manager.addTaskToList("study", "task1", 1);
  manager.setDeadline("study", "task1", "2026-06-01");
  manager.showExpired("study", "2026-06-10", out);

  BOOST_TEST(out.str() == "task1 5 1 2026-06-01\n");
}

BOOST_AUTO_TEST_CASE(manager_completed_count_test)
{
  zhuravleva::TaskManager manager;
  manager.addTaskGlobal("task1", 5);
  manager.addTaskGlobal("task2", 5);
  manager.createList("study", 20);
  manager.addTaskToList("study", "task1", 1);
  manager.addTaskToList("study", "task2", 2);
  manager.markDone("study", "task1", "2026-06-10");

  BOOST_TEST(manager.countCompleted("study", "2026-06-01", "2026-06-30") == 1);
}

BOOST_AUTO_TEST_CASE(manager_save_load_test)
{
  zhuravleva::TaskManager manager;
  manager.addTaskGlobal("task1", 5);
  manager.addTaskGlobal("task2", 10);
  manager.createList("study", 30);
  manager.addTaskToList("study", "task1", 1);
  manager.addTaskToList("study", "task2", 2);
  manager.setDeadline("study", "task1", "2026-06-20");
  manager.markDone("study", "task2", "2026-06-15");
  std::stringstream stream;
  manager.save(stream);
  zhuravleva::TaskManager loaded;
  loaded.load(stream);

  BOOST_TEST(loaded.hasTask("task1"));
  BOOST_TEST(loaded.hasList("study"));
  BOOST_TEST(loaded.hasTaskInList("study", "task2"));
  BOOST_TEST(loaded.getCurrentLabor("study") == 15);
  BOOST_TEST(loaded.countCompleted("study", "2026-06-01", "2026-06-30") == 1);
}