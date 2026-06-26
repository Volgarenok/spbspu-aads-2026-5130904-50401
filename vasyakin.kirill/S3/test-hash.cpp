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
  BOOST_CHECK_EQUAL(table.contains("key"), false);
  BOOST_CHECK_THROW(table.at("key"), std::out_of_range);
  BOOST_CHECK_EQUAL(table.erase("key"), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_has)
{
  TestMap table(16);
  table.insert(std::make_pair("one", 1));
  table.insert(std::make_pair("two", 2));
  table.insert(std::make_pair("three", 3));

  BOOST_CHECK(table.contains("one"));
  BOOST_CHECK(table.contains("two"));
  BOOST_CHECK(table.contains("three"));
  BOOST_CHECK_EQUAL(table.contains("four"), false);
}

BOOST_AUTO_TEST_CASE(test_get_value)
{
  TestMap table(16);

  table.insert(std::make_pair("key", 42));
  BOOST_CHECK_EQUAL(table.at("key"), 42);

  table.insert(std::make_pair("key", 99));
  BOOST_CHECK(table.contains("key"));
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  TestMap table(16);

  table.insert(std::make_pair("del", 100));
  size_t erased = table.erase("del");

  BOOST_CHECK_EQUAL(erased, 1);
  BOOST_CHECK_EQUAL(table.contains("del"), false);
  BOOST_CHECK_EQUAL(table.erase("del"), 0);
}

BOOST_AUTO_TEST_CASE(test_rehash_preserves_data)
{
  TestMap table(8);
  for (int i = 0; i < 10; ++i)
  {
    table.insert(std::make_pair("k" + std::to_string(i), i));
  }

  table.rehash(32);

  for (int i = 0; i < 10; ++i)
  {
    BOOST_CHECK(table.contains("k" + std::to_string(i)));
    BOOST_CHECK_EQUAL(table.at("k" + std::to_string(i)), i);
  }
}

BOOST_AUTO_TEST_CASE(test_heavy_collisions)
{
  TestMap table(2);
  for (int i = 0; i < 20; ++i)
  {
    table.insert(std::make_pair("item" + std::to_string(i), i));
  }

  for (int i = 0; i < 20; ++i)
  {
    BOOST_CHECK(table.contains("item" + std::to_string(i)));
  }
}

BOOST_AUTO_TEST_CASE(test_iteration_collects_all)
{
  TestMap table(16);
  table.insert(std::make_pair("a", 1));
  table.insert(std::make_pair("b", 2));
  table.insert(std::make_pair("c", 3));
  table.insert(std::make_pair("d", 4));

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
  table.insert(std::make_pair("x", 10));
  table.insert(std::make_pair("y", 20));

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

BOOST_AUTO_TEST_CASE(test_load_factor_values)
{
  TestMap table(10);
  BOOST_CHECK_CLOSE(table.load_factor(), 0.0f, 0.001f);

  table.insert(std::make_pair("a", 1));
  table.insert(std::make_pair("b", 2));
  BOOST_CHECK_CLOSE(table.load_factor(), 0.2f, 0.001f);

  for (int i = 0; i < 8; ++i)
  {
    table.insert(std::make_pair("c" + std::to_string(i), i));
  }

  BOOST_CHECK_CLOSE(table.load_factor(), 1.0f, 0.001f);
}

BOOST_AUTO_TEST_CASE(test_max_chain_count_values)
{
  TestMap table(2);
  BOOST_CHECK_EQUAL(table.max_chain_count(), 0);

  table.insert(std::make_pair("x", 1));
  table.insert(std::make_pair("y", 2));
  BOOST_CHECK_GE(table.max_chain_count(), 1);

  table.insert(std::make_pair("z", 3));
  table.insert(std::make_pair("w", 4));

  BOOST_CHECK_GE(table.max_chain_count(), 2);
}

BOOST_AUTO_TEST_CASE(test_auto_rehash_on_load_factor)
{
  TestMap table(4);
  table.max_load_factor(0.25f);

  table.insert(std::make_pair("k1", 1));
  BOOST_CHECK_CLOSE(table.load_factor(), 0.25f, 0.001f);

  table.insert(std::make_pair("k2", 2));
  BOOST_CHECK_CLOSE(table.load_factor(), 0.5f, 0.001f);

  table.insert(std::make_pair("k3", 3));
  BOOST_CHECK_CLOSE(table.load_factor(), 0.375f, 0.001f);
}

BOOST_AUTO_TEST_CASE(test_custom_growth_policy)
{
  TestMap table(10);

  table.insert(std::make_pair("init", 0));
  table.max_load_factor(0.05f);

  table.set_growth_policy([](size_t current)
  {
    return current + 5;
  });

  table.insert(std::make_pair("next", 1));
  BOOST_CHECK_CLOSE(table.load_factor(), 2.0f / 15.0f, 0.001f);
}

BOOST_AUTO_TEST_CASE(test_chain_length_limit_disabled_when_zero)
{
  TestMap table(4);
  table.max_chain_length_limit(0);

  for (int i = 0; i < 20; ++i)
  {
    table.insert(std::make_pair("item" + std::to_string(i), i));
  }

  BOOST_CHECK(table.contains("item19"));
}

BOOST_AUTO_TEST_SUITE_END()
