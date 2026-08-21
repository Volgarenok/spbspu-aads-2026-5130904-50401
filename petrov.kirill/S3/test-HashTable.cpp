#include <boost/test/unit_test.hpp>
#include <string>
#include "../common/HashTable.hpp"

using namespace petrov;

BOOST_AUTO_TEST_CASE(test_initial_state)
{
  HashTable<std::string, int> table;
  BOOST_TEST(table.isEmpty());
  BOOST_TEST(table.getSize() == 0);
  BOOST_TEST(table.getCapacity() == 16);
}

BOOST_AUTO_TEST_CASE(test_add_and_has)
{
  HashTable<std::string, int> table;
  table.add("apple", 100);
  table.add("banana", 200);

  BOOST_TEST(!table.isEmpty());
  BOOST_TEST(table.getSize() == 2);
  BOOST_TEST(table.has("apple"));
  BOOST_TEST(table.has("banana"));
  BOOST_TEST(!table.has("orange"));

  table.add("apple", 500);
  BOOST_TEST(table.getSize() == 2);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  HashTable<std::string, int> table;
  table.add("key1", 10);
  table.add("key2", 20);

  BOOST_TEST(table.drop("key1") == true);
  BOOST_TEST(!table.has("key1"));
  BOOST_TEST(table.getSize() == 1);

  BOOST_TEST(table.drop("key1") == false);
  BOOST_TEST(table.drop("non_existent") == false);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  HashTable<int, std::string> table;
  table.add(1, "one");
  table.add(2, "two");

  table.clear();
  BOOST_TEST(table.isEmpty());
  BOOST_TEST(table.getSize() == 0);
  BOOST_TEST(!table.has(1));
}

BOOST_AUTO_TEST_CASE(test_hash_copy_and_assignment)
{
  HashTable<int, int> table1;
  table1.add(5, 50);
  table1.add(6, 60);

  HashTable<int, int> table2(table1);
  BOOST_TEST(table2.getSize() == 2);
  BOOST_TEST(table2.has(5));
  BOOST_TEST(table2.has(6));

  HashTable<int, int> table3;
  table3 = table1;
  BOOST_TEST(table3.getSize() == 2);
  BOOST_TEST(table3.has(5));
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  HashTable<int, int> table1;
  table1.add(1, 10);

  HashTable<int, int> table2;
  table2.add(2, 20);
  table2.add(3, 30);

  table1.swap(table2);

  BOOST_TEST(table1.getSize() == 2);
  BOOST_TEST(table1.has(2));
  BOOST_TEST(table2.getSize() == 1);
  BOOST_TEST(table2.has(1));
}

BOOST_AUTO_TEST_CASE(test_automatic_rehash)
{
  HashTable<int, int> table(2);
  table.setMaxLoadFactor(0.5);

  size_t initial_capacity = table.getCapacity();

  table.add(10, 100);
  BOOST_TEST(table.getCapacity() == initial_capacity);

  table.add(20, 200);
  BOOST_TEST(table.getCapacity() == initial_capacity * 2);
  BOOST_TEST(table.has(10));
  BOOST_TEST(table.has(20));
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  HashTable<int, int> table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);

  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    BOOST_TEST(table.has(it->first));
    count++;
  }
  BOOST_TEST(count == 3);
}

BOOST_AUTO_TEST_CASE(test_const_iterators)
{
  HashTable<int, int> table;
  table.add(100, 1);

  const HashTable<int, int>& constTable = table;

  auto it = constTable.begin();

  BOOST_CHECK(it != constTable.end());
  BOOST_TEST(it->first == 100);
  BOOST_TEST((*it).second == 1);

  ++it;
  BOOST_CHECK(!(it != constTable.end()));
}
