#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>

#include "hash_table.hpp"

BOOST_AUTO_TEST_CASE(add_and_find_test)
{
  ulanova::HashTable< int > table;

  table.add("maria", 10);
  table.add("ivan", 20);

  int* maria = table.find("maria");
  int* ivan = table.find("ivan");

  BOOST_REQUIRE(maria != nullptr);
  BOOST_REQUIRE(ivan != nullptr);
  BOOST_CHECK(*maria == 10);
  BOOST_CHECK(*ivan == 20);
}

BOOST_AUTO_TEST_CASE(has_test)
{
  ulanova::HashTable< int > table;

  table.add("maria", 10);

  BOOST_CHECK(table.has("maria"));
  BOOST_CHECK(!table.has("ivan"));
}

BOOST_AUTO_TEST_CASE(duplicate_add_throws_test)
{
  ulanova::HashTable< int > table;

  table.add("maria", 10);

  BOOST_CHECK_THROW(table.add("maria", 20), std::logic_error);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  ulanova::HashTable< int > table;

  table.add("maria", 10);
  table.drop("maria");

  BOOST_CHECK(!table.has("maria"));
  BOOST_CHECK(table.find("maria") == nullptr);
}

BOOST_AUTO_TEST_CASE(drop_missing_throws_test)
{
  ulanova::HashTable< int > table;

  BOOST_CHECK_THROW(table.drop("maria"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(iterator_skips_empty_buckets_test)
{
  ulanova::HashTable< int > table;

  table.add("first", 1);
  table.add("second", 2);
  table.add("third", 3);

  int sum = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    sum += *it;
  }

  BOOST_CHECK(sum == 6);
}

BOOST_AUTO_TEST_CASE(iterator_skips_deleted_buckets_test)
{
  ulanova::HashTable< int > table;

  table.add("first", 1);
  table.add("second", 2);
  table.add("third", 3);
  table.drop("second");

  int sum = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    sum += *it;
  }

  BOOST_CHECK(sum == 4);
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  ulanova::HashTable< int > table;

  table.add("first", 1);
  table.add("second", 2);

  const ulanova::HashTable< int >& const_table = table;
  int sum = 0;

  for (auto it = const_table.cbegin(); it != const_table.cend(); ++it)
  {
    sum += *it;
  }

  BOOST_CHECK(sum == 3);
}

BOOST_AUTO_TEST_CASE(rehash_keeps_values_test)
{
  ulanova::HashTable< int > table;

  for (int i = 0; i < 30; ++i)
  {
    table.add("key" + std::to_string(i), i);
  }

  for (int i = 0; i < 30; ++i)
  {
    const int* value = table.find("key" + std::to_string(i));

    BOOST_REQUIRE(value != nullptr);
    BOOST_CHECK(*value == i);
  }
}

