#define BOOST_TEST_MODULE F0
#include <boost/test/included/unit_test.hpp>
#include <string>
#include "cuckooHashTable.hpp"

BOOST_AUTO_TEST_CASE(cuckoo_insert_get_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;
  table.insert("a", 1);
  table.insert("b", 2);

  BOOST_TEST(table.size() == 2);
  BOOST_TEST(table.get("a") == 1);
  BOOST_TEST(table.get("b") == 2);
}

BOOST_AUTO_TEST_CASE(cuckoo_update_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;
  table.insert("a", 1);
  table.insert("a", 10);

  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.get("a") == 10);
}

BOOST_AUTO_TEST_CASE(cuckoo_contains_erase_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;
  table.insert("a", 1);

  BOOST_TEST(table.contains("a"));
  table.erase("a");
  BOOST_TEST(!table.contains("a"));
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(cuckoo_get_missing_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;

  BOOST_CHECK_THROW(table.get("none"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(cuckoo_iterator_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;
  table.insert("a", 1);
  table.insert("b", 2);
  table.insert("c", 3);
  size_t count = 0;
  int sum = 0;
  for (zhuravleva::CuckooHashTable< std::string, int >::Iterator it = table.begin();
      it != table.end(); ++it)
  {
    ++count;
    sum += it->second;
  }

  BOOST_TEST(count == 3);
  BOOST_TEST(sum == 6);
}

BOOST_AUTO_TEST_CASE(cuckoo_const_iterator_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;
  table.insert("a", 1);
  table.insert("b", 2);
  const zhuravleva::CuckooHashTable< std::string, int >& constTable = table;
  size_t count = 0;
  for (zhuravleva::CuckooHashTable< std::string, int >::ConstIterator it =
      constTable.cbegin(); it != constTable.cend(); ++it)
  {
    ++count;
  }

  BOOST_TEST(count == 2);
}

BOOST_AUTO_TEST_CASE(cuckoo_clear_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;
  table.insert("A", 1);
  table.insert("B", 2);
  table.clear();

  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
  BOOST_CHECK(!table.contains("A"));
}

BOOST_AUTO_TEST_CASE(cuckoo_many_insert_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;
  for (size_t i = 0; i < 50; ++i)
  {
    table.insert("task" + std::to_string(i), i);
  }

  BOOST_TEST(table.size() == 50);
  BOOST_TEST(table.get("task0") == 0);
  BOOST_TEST(table.get("task25") == 25);
  BOOST_TEST(table.get("task49") == 49);
}

BOOST_AUTO_TEST_CASE(cuckoo_erase_missing_test)
{
  zhuravleva::CuckooHashTable< std::string, int > table;

  BOOST_CHECK_THROW(table.erase("A"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(cuckoo_swap_test)
{
  zhuravleva::CuckooHashTable< std::string, int > first;
  zhuravleva::CuckooHashTable< std::string, int > second;
  first.insert("A", 10);
  second.insert("B", 20);
  first.swap(second);

  BOOST_TEST(first.get("B") == 20);
  BOOST_TEST(second.get("A") == 10);
}
