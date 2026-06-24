#include "hashtable.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <functional>
#include <stdexcept>
#include <string>
using namespace levkin;
struct SimpleStringHash {
  size_t operator()(const std::string& s) const
  {
    return std::hash< std::string >{}(s);
  }
};
struct SimpleStringEqual {
  bool operator()(const std::string& a, const std::string& b) const
  {
    return a == b;
  }
};
using TestTable =
    HashTable< std::string, int, SimpleStringHash, SimpleStringEqual >;
BOOST_AUTO_TEST_CASE(ht_constructor_test)
{
  TestTable ht(8);
  BOOST_CHECK_EQUAL(ht.has("key"), false);
}
BOOST_AUTO_TEST_CASE(ht_add_and_has_test)
{
  TestTable ht(4);
  ht.add("apple", 100);
  ht.add("banana", 200);
  BOOST_CHECK(ht.has("apple"));
  BOOST_CHECK(ht.has("banana"));
  BOOST_CHECK(!ht.has("orange"));
}
BOOST_AUTO_TEST_CASE(ht_update_value_test)
{
  TestTable ht(4);
  ht.add("key1", 10);
  ht.add("key1", 20);
  BOOST_CHECK(ht.has("key1"));
  BOOST_CHECK_EQUAL(ht.drop("key1"), 20);
  BOOST_CHECK(!ht.has("key1"));
}
BOOST_AUTO_TEST_CASE(ht_overflow_bucket_test)
{
  struct CollisionHash {
    size_t operator()(const std::string&) const { return 0; }
  };
  using CollisionTable =
      HashTable< std::string, int, CollisionHash, SimpleStringEqual >;
  CollisionTable ht(4);
  ht.add("one", 1);
  ht.add("two", 2);
  ht.add("three", 3);
  ht.add("four", 4);
  BOOST_CHECK(ht.has("one"));
  BOOST_CHECK(ht.has("two"));
  BOOST_CHECK(ht.has("three"));
  BOOST_CHECK(ht.has("four"));
}
BOOST_AUTO_TEST_CASE(ht_drop_test)
{
  TestTable ht(8);
  ht.add("test", 42);
  BOOST_CHECK(ht.has("test"));
  int removed = ht.drop("test");
  BOOST_CHECK_EQUAL(removed, 42);
  BOOST_CHECK(!ht.has("test"));
  BOOST_CHECK_THROW(ht.drop("unknown"), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(ht_drop_from_overflow_test)
{
  struct CollisionHash {
    size_t operator()(const std::string&) const { return 0; }
  };
  using CollisionTable =
      HashTable< std::string, int, CollisionHash, SimpleStringEqual >;
  CollisionTable ht(4);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);
  ht.add("d", 4);
  BOOST_CHECK_EQUAL(ht.drop("d"), 4);
  BOOST_CHECK(!ht.has("d"));
  BOOST_CHECK(ht.has("a"));
}
