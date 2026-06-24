#include "hash_table.hpp"
#include "siphash.hpp"
#include <boost/test/unit_test.hpp>

using namespace burukov;

BOOST_AUTO_TEST_SUITE(HashTableTest)

BOOST_AUTO_TEST_CASE(add_and_contains)
{
  HashTable< int, std::string > table(4);
  table.add(1, "one");
  table.add(2, "two");
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK(!table.contains(3));
  BOOST_CHECK_EQUAL(table.at(1), "one");
  BOOST_CHECK_EQUAL(table.at(2), "two");
  BOOST_CHECK_EQUAL(table.size(), 2);
}

BOOST_AUTO_TEST_CASE(update_existing)
{
  HashTable< int, int > table(4);
  table.add(42, 100);
  table.add(42, 200);
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(42), 200);
}

BOOST_AUTO_TEST_CASE(erase_key)
{
  HashTable< int, int > table(4);
  table.add(1, 10);
  table.add(2, 20);
  table.erase(1);
  BOOST_CHECK(!table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(rehash_table)
{
  HashTable< int, int > table(2);
  for (int i = 0; i < 10; ++i)
  {
    table.add(i, i);
  }
  table.rehash(20);
  BOOST_CHECK_EQUAL(table.size(), 10);
  for (int i = 0; i < 10; ++i)
  {
    BOOST_CHECK_EQUAL(table.at(i), i);
  }
}

BOOST_AUTO_TEST_CASE(iteration)
{
  HashTable< int, int > table(4);
  for (int i = 0; i < 10; ++i)
  {
    table.add(i, i);
  }
  int sum = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    sum += it->second;
  }
  BOOST_CHECK_EQUAL(sum, 45);
}

BOOST_AUTO_TEST_SUITE_END()
