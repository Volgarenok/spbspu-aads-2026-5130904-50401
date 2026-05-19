#include <boost/test/unit_test.hpp>

#include "hash_table.hpp"
#include "hmac_hash.hpp"

#include <functional>
#include <stdexcept>
#include <string>

using TestTable = studilova::HashTable<
  std::string,
  int,
  studilova::HMACHash,
  std::equal_to< std::string >
>;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(empty_table)
{
  TestTable table(16);

  BOOST_CHECK(table.isEmpty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(!table.has("a"));

  BOOST_CHECK_THROW(table.get("a"), std::out_of_range);
  BOOST_CHECK_THROW(table.drop("a"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(add_and_has)
{
  TestTable table(16);

  table.add("a", 10);
  table.add("b", 20);
  table.add("c", 30);

  BOOST_CHECK(table.has("a"));
  BOOST_CHECK(table.has("b"));
  BOOST_CHECK(table.has("c"));
  BOOST_CHECK(!table.has("d"));

  BOOST_CHECK_EQUAL(table.size(), 3);
}

BOOST_AUTO_TEST_CASE(get_and_replace)
{
  TestTable table(16);

  table.add("key", 1);
  BOOST_CHECK_EQUAL(table.get("key"), 1);

  table.add("key", 100);

  BOOST_CHECK_EQUAL(table.get("key"), 100);
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(erase_element)
{
  TestTable table(16);

  table.add("a", 10);
  table.add("b", 20);

  BOOST_CHECK(table.erase("a"));

  BOOST_CHECK(!table.has("a"));
  BOOST_CHECK(table.has("b"));

  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(erase_missing_element)
{
  TestTable table(16);

  table.add("a", 10);

  BOOST_CHECK(!table.erase("b"));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(drop_element)
{
  TestTable table(16);

  table.add("a", 42);

  BOOST_CHECK_EQUAL(table.drop("a"), 42);
  BOOST_CHECK(!table.has("a"));

  BOOST_CHECK_THROW(table.drop("a"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(rehash_preserves_values)
{
  TestTable table(8);

  table.add("a", 10);
  table.add("b", 20);
  table.add("c", 30);

  table.rehash(31);

  BOOST_CHECK_EQUAL(table.capacity(), 31);

  BOOST_CHECK_EQUAL(table.get("a"), 10);
  BOOST_CHECK_EQUAL(table.get("b"), 20);
  BOOST_CHECK_EQUAL(table.get("c"), 30);
}

BOOST_AUTO_TEST_CASE(iterator_visits_all_elements)
{
  TestTable table(16);

  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);

  int sum = 0;
  size_t count = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    auto item = *it;

    sum += item.second;
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 3);
  BOOST_CHECK_EQUAL(sum, 6);
}

BOOST_AUTO_TEST_CASE(const_iterator_works)
{
  TestTable table(16);

  table.add("x", 10);
  table.add("y", 20);

  const TestTable& constTable = table;

  int sum = 0;

  for (auto it = constTable.cbegin(); it != constTable.cend(); ++it)
  {
    auto item = *it;
    sum += item.second;
  }

  BOOST_CHECK_EQUAL(sum, 30);
}

BOOST_AUTO_TEST_CASE(empty_iteration)
{
  TestTable table(16);

  size_t count = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_CASE(hmac_hash_is_deterministic)
{
  studilova::HMACHash hasher("secret");

  size_t h1 = hasher("abc");
  size_t h2 = hasher("abc");
  size_t h3 = hasher("xyz");

  BOOST_CHECK_EQUAL(h1, h2);
  BOOST_CHECK(h1 != h3);
}

BOOST_AUTO_TEST_SUITE_END()
