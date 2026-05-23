#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include <string>
BOOST_AUTO_TEST_SUITE(HashTableTest)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  madieva::HashTable< int, std::string, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK(ht.size() == 0);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK(ht.capacity() == 16);
}

BOOST_AUTO_TEST_CASE(constructor_with_capacity)
{
  madieva::HashTable< int, std::string, std::hash< int >, std::equal_to< int > > ht(32);
  BOOST_CHECK(ht.capacity() == 32);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK(ht.size() == 0);
}

BOOST_AUTO_TEST_CASE(add_and_has)
{
  madieva::HashTable< int, std::string, std::hash< int >, std::equal_to< int > > ht;
  ht.add(1, "one");
  ht.add(2, "two");

  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(!ht.has(3));
  BOOST_CHECK(ht.size() == 2);
}

BOOST_AUTO_TEST_CASE(get_key)
{
  madieva::HashTable< int, std::string, std::hash< int >, std::equal_to< int > > ht;
  ht.add(1, "one");
  std::string& val = ht.get(1);
  BOOST_CHECK(val == "one");
}

BOOST_AUTO_TEST_CASE(get_throws)
{
  madieva::HashTable< int, std::string, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK_THROW(ht.get(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(drop_key)
{
  madieva::HashTable< int, std::string, std::hash< int >, std::equal_to< int > > ht;
  ht.add(1, "one");
  ht.add(2, "two");

  std::string val = ht.drop(1);
  BOOST_CHECK(val == "one");
  BOOST_CHECK(!ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(ht.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
