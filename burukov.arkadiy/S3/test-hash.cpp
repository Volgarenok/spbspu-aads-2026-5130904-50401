#include <boost/test/unit_test.hpp>

#include <string>

#include "hash_table.hpp"
#include "siphash.hpp"

BOOST_AUTO_TEST_SUITE(HashTableTest)

BOOST_AUTO_TEST_CASE(add_and_contains)
{
  burukov::HashTable< int, std::string > table(4);
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
  burukov::HashTable< int, int > table(4);
  table.add(42, 100);
  table.add(42, 200);
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(42), 200);
}

BOOST_AUTO_TEST_CASE(erase_key)
{
  burukov::HashTable< int, int > table(4);
  table.add(1, 10);
  table.add(2, 20);
  table.erase(1);
  BOOST_CHECK(!table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(rehash_table)
{
  burukov::HashTable< int, int > table(2);
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
  burukov::HashTable< int, int > table(4);
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

BOOST_AUTO_TEST_CASE(load_factor_value)
{
  burukov::HashTable< int, int > table(4);
  table.add(1, 1);
  table.add(2, 2);
  BOOST_CHECK_CLOSE(table.loadFactor(), 0.5, 1e-6);
}

BOOST_AUTO_TEST_CASE(no_auto_rehash_by_default)
{
  burukov::HashTable< int, int > table(2);
  for (int i = 0; i < 50; ++i)
  {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 50);
  BOOST_CHECK(table.loadFactor() > 1.0);
}

BOOST_AUTO_TEST_CASE(auto_rehash_by_load_factor)
{
  burukov::HashTable< int, int > table(4);
  table.maxLoadFactor(0.75);
  for (int i = 0; i < 100; ++i)
  {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 100);
  BOOST_CHECK(table.loadFactor() <= 0.75);
  for (int i = 0; i < 100; ++i)
  {
    BOOST_CHECK_EQUAL(table.at(i), i);
  }
}

BOOST_AUTO_TEST_CASE(auto_rehash_by_chain_length)
{
  burukov::HashTable< int, int > table(4);
  table.maxChainLength(3);
  for (int i = 0; i < 200; ++i)
  {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 200);
  BOOST_CHECK(table.longestChain() <= 3);
}

BOOST_AUTO_TEST_CASE(custom_slot_policy)
{
  burukov::HashTable< int, int > table(4);
  table.slotPolicy([](size_t before) { return before + 8; });
  table.maxChainLength(2);
  for (int i = 0; i < 60; ++i)
  {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 60);
  for (int i = 0; i < 60; ++i)
  {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(string_keys_auto_rehash)
{
  burukov::HashTable< std::string, int > table(2);
  table.maxLoadFactor(0.5);
  const char *keys[] = {"alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta"};
  for (int i = 0; i < 8; ++i)
  {
    table.add(keys[i], i);
  }
  BOOST_CHECK_EQUAL(table.size(), 8);
  BOOST_CHECK(table.loadFactor() <= 0.5);
  for (int i = 0; i < 8; ++i)
  {
    BOOST_CHECK_EQUAL(table.at(keys[i]), i);
  }
}

BOOST_AUTO_TEST_SUITE_END()
