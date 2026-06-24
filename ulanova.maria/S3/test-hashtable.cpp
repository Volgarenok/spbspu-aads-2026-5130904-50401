#include "hash_table.hpp"
#include "hash_functions.hpp"
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_CASE(test_hash_add_has_find)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  BOOST_TEST_CHECK(table.isEmpty());
  BOOST_TEST_CHECK(table.getsize() == 0);

  table.add("a", 10);
  table.add("b", 20);

  BOOST_TEST_CHECK(!table.isEmpty());
  BOOST_TEST_CHECK(table.getsize() == 2);
  BOOST_TEST_CHECK(table.has("a"));
  BOOST_TEST_CHECK(table.has("b"));
  BOOST_TEST_CHECK(!table.has("c"));

  BOOST_REQUIRE(table.find("a") != nullptr);
  BOOST_REQUIRE(table.find("b") != nullptr);
  BOOST_TEST_CHECK(*table.find("a") == 10);
  BOOST_TEST_CHECK(*table.find("b") == 20);
  BOOST_TEST_CHECK(table.find("c") == nullptr);
}

BOOST_AUTO_TEST_CASE(test_hash_add_updates_existing_key)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  table.add("a", 10);
  table.add("a", 99);

  BOOST_TEST_CHECK(table.getsize() == 1);
  BOOST_REQUIRE(table.find("a") != nullptr);
  BOOST_TEST_CHECK(*table.find("a") == 99);
}

BOOST_AUTO_TEST_CASE(test_hash_drop)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  table.add("a", 10);
  table.add("b", 20);

  int dropped = table.drop("a");

  BOOST_TEST_CHECK(dropped == 10);
  BOOST_TEST_CHECK(!table.has("a"));
  BOOST_TEST_CHECK(table.has("b"));
  BOOST_TEST_CHECK(table.getsize() == 1);
}

BOOST_AUTO_TEST_CASE(test_hash_drop_missing_key_throws)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  BOOST_CHECK_THROW(table.drop("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_hash_rehash)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  table.add("a", 10);
  table.add("b", 20);

  table.rehash(32);

  BOOST_TEST_CHECK(table.getcapacity() == 32);
  BOOST_TEST_CHECK(table.getsize() == 2);
  BOOST_TEST_CHECK(table.has("a"));
  BOOST_TEST_CHECK(table.has("b"));

  BOOST_REQUIRE(table.find("a") != nullptr);
  BOOST_REQUIRE(table.find("b") != nullptr);
  BOOST_TEST_CHECK(*table.find("a") == 10);
  BOOST_TEST_CHECK(*table.find("b") == 20);
}

BOOST_AUTO_TEST_CASE(test_hash_rehash_too_small_throws_and_keeps_data)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  table.add("a", 10);
  table.add("b", 20);

  BOOST_CHECK_THROW(table.rehash(1), std::invalid_argument);

  BOOST_TEST_CHECK(table.getsize() == 2);
  BOOST_TEST_CHECK(table.has("a"));
  BOOST_TEST_CHECK(table.has("b"));
}

BOOST_AUTO_TEST_CASE(test_hash_iterator_skips_empty_buckets)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  table.add("a", 10);
  table.add("b", 20);
  table.add("c", 30);

  size_t count = 0;

  for (auto it = table.begin(); it != table.end(); ++it)
  {
    BOOST_TEST_CHECK(table.has(it.key()));
    ++count;
  }

  BOOST_TEST_CHECK(count == 3);
}

BOOST_AUTO_TEST_CASE(test_hash_copy_constructor)
{
  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> table(8);

  table.add("a", 10);
  table.add("b", 20);

  ulanova::HashTable<std::string, int, ulanova::StringHash, ulanova::StringEqual> copy(table);

  BOOST_TEST_CHECK(copy.getsize() == 2);
  BOOST_TEST_CHECK(copy.has("a"));
  BOOST_TEST_CHECK(copy.has("b"));

  copy.add("a", 99);

  BOOST_REQUIRE(table.find("a") != nullptr);
  BOOST_REQUIRE(copy.find("a") != nullptr);
  BOOST_TEST_CHECK(*table.find("a") == 10);
  BOOST_TEST_CHECK(*copy.find("a") == 99);
}
