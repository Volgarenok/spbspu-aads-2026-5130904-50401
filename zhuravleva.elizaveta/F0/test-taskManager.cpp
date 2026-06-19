#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
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

BOOST_AUTO_TEST_CASE(manager_save_empty_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.save(out);

  BOOST_CHECK(out.str().find("TASKS 0") != std::string::npos);
  BOOST_CHECK(out.str().find("LISTS 0") != std::string::npos);
  BOOST_CHECK(out.str().find("ITEMS 0") != std::string::npos);
}


BOOST_AUTO_TEST_CASE(manager_load_empty_test)
{
  zhuravleva::TaskManager manager;
  std::istringstream in("TASKS 0\nLISTS 0\nITEMS 0\n");

  BOOST_CHECK_NO_THROW(manager.load(in));
  BOOST_CHECK(!manager.hasTask("A"));
  BOOST_CHECK(!manager.hasList("work"));
}

BOOST_AUTO_TEST_CASE(manager_load_with_data_test)
{
  zhuravleva::TaskManager manager;
  std::istringstream in(
      "TASKS 2\n"
      "A 10\n"
      "B 20\n"
      "LISTS 1\n"
      "work 50\n"
      "ITEMS 2\n"
      "work A 1 0 - -\n"
      "work B 3 1 2026-06-20 2026-06-18\n");
  manager.load(in);

  BOOST_CHECK(manager.hasTask("A"));
  BOOST_CHECK(manager.hasList("work"));
  BOOST_CHECK(manager.hasTaskInList("work", "A"));
  BOOST_CHECK(manager.hasTaskInList("work", "B"));
  BOOST_TEST(manager.getCurrentLabor("work") == 30);
  BOOST_TEST(manager.countCompleted("work", "2026-06-01", "2026-06-30") == 1);
}

BOOST_AUTO_TEST_CASE(manager_load_invalid_test)
{
  zhuravleva::TaskManager manager;
  std::istringstream in("wrong file");

  BOOST_CHECK_THROW(manager.load(in), std::logic_error);
}

BOOST_AUTO_TEST_CASE(manager_suggest_remove_one_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.addTaskGlobal("A", 40);
  manager.addTaskGlobal("B", 20);
  manager.addTaskGlobal("C", 15);
  manager.createList("work", 50);
  manager.addTaskToList("work", "A", 1);
  manager.addTaskToList("work", "C", 2);
  manager.suggestRemoveOne("work", "B", out);

  BOOST_CHECK(out.str().find("A") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(manager_suggest_remove_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.addTaskGlobal("A", 30);
  manager.addTaskGlobal("B", 25);
  manager.addTaskGlobal("C", 10);
  manager.addTaskGlobal("D", 20);
  manager.createList("work", 50);
  manager.addTaskToList("work", "A", 1);
  manager.addTaskToList("work", "B", 2);
  manager.addTaskToList("work", "C", 3);
  manager.suggestRemove("work", "D", 2, out);

  BOOST_CHECK(out.str().find("A") != std::string::npos
      || out.str().find("B") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(manager_optimize_test)
{
  zhuravleva::TaskManager manager;
  std::ostringstream out;
  manager.addTaskGlobal("A", 10);
  manager.addTaskGlobal("B", 20);
  manager.addTaskGlobal("C", 40);
  manager.createList("work", 100);
  manager.addTaskToList("work", "A", 1);
  manager.addTaskToList("work", "B", 2);
  manager.addTaskToList("work", "C", 10);
  manager.optimize("result", "work", 30);

  BOOST_CHECK(manager.hasList("result"));
  BOOST_CHECK(manager.hasTaskInList("result", "A"));
  BOOST_CHECK(manager.hasTaskInList("result", "B"));
  BOOST_CHECK(!manager.hasTaskInList("result", "C"));
  BOOST_TEST(manager.getCurrentLabor("result") == 30);
}
