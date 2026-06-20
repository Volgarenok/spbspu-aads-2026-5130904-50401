#include "hash_table.hpp"
#include <stdexcept>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  sedov::HashTable< std::string, int > ht(16);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_size)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.empty());
  ht.add("two", 2);
  BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_contains)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);
  BOOST_CHECK(ht.contains("one"));
  BOOST_CHECK(ht.contains("two"));
  BOOST_CHECK(!ht.contains("three"));
}

BOOST_AUTO_TEST_CASE(test_at)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);
  BOOST_CHECK_EQUAL(ht.at("one"), 1);
  BOOST_CHECK_EQUAL(ht.at("two"), 2);
  ht.at("one") = 10;
  BOOST_CHECK_EQUAL(ht.at("one"), 10);
}

BOOST_AUTO_TEST_CASE(test_at_throws_for_missing_key)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  BOOST_CHECK_THROW(ht.at("two"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.erase("one");
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.contains("one"));
  BOOST_CHECK(ht.contains("two"));
}

BOOST_AUTO_TEST_CASE(test_add_update_existing)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("key", 1);
  ht.add("key", 2);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.at("key"), 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);
  ht.clear();
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK(!ht.contains("one"));
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  sedov::HashTable< std::string, int > ht1(16);
  ht1.add("one", 1);
  ht1.add("two", 2);
  sedov::HashTable< std::string, int > ht2 = ht1;
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.contains("one"));
  BOOST_CHECK(ht2.contains("two"));
  BOOST_CHECK_EQUAL(ht2.at("one"), 1);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  sedov::HashTable< std::string, int > ht1(16);
  ht1.add("one", 1);
  ht1.add("two", 2);
  sedov::HashTable< std::string, int > ht2 = std::move(ht1);
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK(ht2.contains("one"));
  BOOST_CHECK(ht2.contains("two"));
  BOOST_CHECK(ht1.empty());
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  sedov::HashTable< std::string, int > ht(4);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);
  ht.add("four", 4);
  ht.add("five", 5);
  ht.rehash(16);
  BOOST_CHECK_EQUAL(ht.size(), 5);
  BOOST_CHECK(ht.contains("one"));
  BOOST_CHECK(ht.contains("two"));
  BOOST_CHECK(ht.contains("three"));
  BOOST_CHECK(ht.contains("four"));
  BOOST_CHECK(ht.contains("five"));
}

BOOST_AUTO_TEST_CASE(test_iterator)
{
  sedov::HashTable< std::string, int > ht(16);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);
  int sum = 0;
  for (sedov::HashTable< std::string, int >::HIter it = ht.begin(); it != ht.end(); ++it)
  {
    sum += (*it).second;
  }
  BOOST_CHECK_EQUAL(sum, 6);
}

BOOST_AUTO_TEST_SUITE_END()
