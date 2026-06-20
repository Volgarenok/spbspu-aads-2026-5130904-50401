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

BOOST_AUTO_TEST_CASE(find_returns_nullptr_for_test)
{
  ulanova::HashTable< int > table;
  BOOST_CHECK(table.find("missing") == nullptr);
}

BOOST_AUTO_TEST_CASE(drop_and_readd_test)
{
  ulanova::HashTable< int > table;
  table.add("key", 10);
  table.drop("key");
  table.add("key", 20);  // должно работать после удаления

  int* val = table.find("key");
  BOOST_REQUIRE(val != nullptr);
  BOOST_CHECK(*val == 20);
}

BOOST_AUTO_TEST_CASE(size_test)
{
  ulanova::HashTable< int > table;
  BOOST_CHECK(table.size() == 0);
  table.add("a", 1);
  BOOST_CHECK(table.size() == 1);
  table.add("b", 2);
  BOOST_CHECK(table.size() == 2);
  table.drop("a");
  BOOST_CHECK(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  ulanova::HashTable< int > table;
  BOOST_CHECK(table.empty());
  table.add("a", 1);
  BOOST_CHECK(!table.empty());
  table.drop("a");
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(value_through_find_test)
{
  ulanova::HashTable< int > table;
  table.add("key", 10);
  int* val = table.find("key");
  BOOST_REQUIRE(val != nullptr);
  *val = 99;
  BOOST_CHECK(*table.find("key") == 99);
}

BOOST_AUTO_TEST_CASE(at_found_test)
{
  ulanova::HashTable< int > table;
  table.add("key", 42);
  BOOST_CHECK(table.at("key") == 42);
}

BOOST_AUTO_TEST_CASE(at_modify_test)
{
  ulanova::HashTable< int > table;
  table.add("key", 42);
  table.at("key") = 99;
  BOOST_CHECK(table.at("key") == 99);
}

BOOST_AUTO_TEST_CASE(at_not_found_throws_test)
{
  ulanova::HashTable< int > table;
  BOOST_CHECK_THROW(table.at("missing"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(at_const_test)
{
  ulanova::HashTable< int > table;
  table.add("key", 42);
  const ulanova::HashTable< int >& ctable = table;
  BOOST_CHECK(ctable.at("key") == 42);
}


BOOST_AUTO_TEST_CASE(capacity_test)
{
  ulanova::HashTable< int > table;
  BOOST_CHECK(table.capacity() >= table.size());
  table.add("a", 1);
  BOOST_CHECK(table.capacity() >= table.size());
}

BOOST_AUTO_TEST_CASE(capacity_constructor_test)
{
  ulanova::HashTable< int > table(100);
  BOOST_CHECK(table.capacity() >= 100);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  ulanova::HashTable< int > a;
  ulanova::HashTable< int > b;
  a.add("x", 1);
  b.add("y", 2);
  a.swap(b);
  BOOST_CHECK(a.has("y") && !a.has("x"));
  BOOST_CHECK(b.has("x") && !b.has("y"));
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  ulanova::HashTable< int > original;
  original.add("a", 1);
  original.add("b", 2);

  ulanova::HashTable< int > copy(original);
  BOOST_CHECK(copy.has("a"));
  BOOST_CHECK(*copy.find("a") == 1);

  original.drop("a");
  BOOST_CHECK(copy.has("a"));
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  ulanova::HashTable< int > original;
  original.add("a", 1);

  ulanova::HashTable< int > moved(std::move(original));
  BOOST_CHECK(moved.has("a"));
  BOOST_CHECK(original.empty());
}

BOOST_AUTO_TEST_CASE(copy_assign_test)
{
  ulanova::HashTable< int > a;
  ulanova::HashTable< int > b;
  a.add("x", 10);
  b = a;
  BOOST_CHECK(b.has("x") && *b.find("x") == 10);
}

BOOST_AUTO_TEST_CASE(move_assign_test)
{
  ulanova::HashTable< int > a;
  ulanova::HashTable< int > b;
  a.add("x", 10);
  b = std::move(a);
  BOOST_CHECK(b.has("x") && a.empty());
}
