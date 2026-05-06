#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include "commands.hpp"

using TestMap = vasyakin::HashTable<std::string, int, vasyakin::HMACHash, std::equal_to<std::string>>;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_empty_table)
{
  TestMap table(16);
  BOOST_CHECK_EQUAL(table.has("key"), false);
  BOOST_CHECK_THROW(table.get("key"), std::out_of_range);
  BOOST_CHECK_THROW(table.drop("key"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_add_and_has)
{
  TestMap table(16);
  table.add("one", 1);
  table.add("two", 2);
  table.add("three", 3);

  BOOST_CHECK(table.has("one"));
  BOOST_CHECK(table.has("two"));
  BOOST_CHECK(table.has("three"));
  BOOST_CHECK_EQUAL(table.has("four"), false);
}

BOOST_AUTO_TEST_CASE(test_get_value)
{
  TestMap table(16);
  table.add("key", 42);
  BOOST_CHECK_EQUAL(table.get("key"), 42);
  table.add("key", 99);
  BOOST_CHECK(table.has("key"));
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  TestMap table(16);
  table.add("del", 100);
  size_t val = table.drop("del");
  BOOST_CHECK_EQUAL(val, 100);
  BOOST_CHECK_EQUAL(table.has("del"), false);
  BOOST_CHECK_THROW(table.drop("del"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_rehash_preserves_data)
{
  TestMap table(8);
  for (int i = 0; i < 10; ++i)
  {
    table.add("k" + std::to_string(i), i);
  }

  table.rehash(32);

  for (int i = 0; i < 10; ++i)
  {
    BOOST_CHECK(table.has("k" + std::to_string(i)));
    BOOST_CHECK_EQUAL(table.get("k" + std::to_string(i)), i);
  }
}

BOOST_AUTO_TEST_CASE(test_heavy_collisions)
{
  TestMap table(2);
  for (int i = 0; i < 20; ++i)
  {
    table.add("item" + std::to_string(i), i);
  }

  for (int i = 0; i < 20; ++i)
  {
    BOOST_CHECK(table.has("item" + std::to_string(i)));
  }
}

BOOST_AUTO_TEST_CASE(test_iteration_collects_all)
{
  TestMap table(16);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.add("d", 4);

  std::vector< int > collected;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    collected.push_back(it->second);
  }

  std::sort(collected.begin(), collected.end());
  std::vector< int > expected = {1, 2, 3, 4};
  BOOST_CHECK_EQUAL_COLLECTIONS(collected.begin(), collected.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_const_iteration)
{
  TestMap table(16);
  table.add("x", 10);
  table.add("y", 20);

  const auto& ctable = table;
  int sum = 0;
  for (auto it = ctable.cbegin(); it != ctable.cend(); ++it)
  {
    sum += it->second;
  }

  BOOST_CHECK_EQUAL(sum, 30);
}

BOOST_AUTO_TEST_CASE(test_empty_iteration)
{
  TestMap table(16);
  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_CASE(test_hmac_determinism)
{
  vasyakin::HMACHash hasher("my_secret_key");
  size_t h1 = hasher("test_string");
  size_t h2 = hasher("test_string");
  
  BOOST_CHECK_EQUAL(h1, h2);
  BOOST_CHECK(h1 != hasher("another_string"));
}

BOOST_AUTO_TEST_SUITE_END()
