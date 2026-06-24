#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>
#include "hash-table.hpp"

namespace {
  struct IntHash {
    size_t operator()(int value) const
    {
      return static_cast< size_t >(value);
    }
  };

  struct BadHash {
    size_t operator()(int) const
    {
      return 0;
    }
  };

  struct IntEqual {
    bool operator()(int lhs, int rhs) const
    {
      return lhs == rhs;
    }
  };
}

BOOST_AUTO_TEST_CASE(add_and_get_value)
{
  lukashevich::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");
  table.add(2, "two");

  BOOST_TEST(table.size() == 2);
  BOOST_TEST(table.has(1));
  BOOST_TEST(table.has(2));
  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");
}

BOOST_AUTO_TEST_CASE(duplicate_key_is_error)
{
  lukashevich::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");

  BOOST_CHECK_THROW(table.add(1, "again"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(drop_removes_value)
{
  lukashevich::HashTable< int, std::string, IntHash, IntEqual > table(5, 2);

  table.add(1, "one");

  BOOST_TEST(table.erase(1) == "one");
  BOOST_TEST(!table.has(1));
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(overflow_bucket_is_used)
{
  lukashevich::HashTable< int, std::string, BadHash, IntEqual > table(1, 2);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.overflowSize() == 1);
  BOOST_TEST(table.has(3));
  BOOST_TEST(table.at(3) == "three");
}

BOOST_AUTO_TEST_CASE(overflow_bucket_limit_is_checked)
{
  lukashevich::HashTable< int, std::string, BadHash, IntEqual > table(1, 1);

  table.add(1, "one");
  table.add(2, "two");

  BOOST_CHECK_THROW(table.add(3, "three"), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(rehash_preserves_values)
{
  lukashevich::HashTable< int, std::string, BadHash, IntEqual > table(1, 3);

  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  table.rehash(7, 2);

  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.at(1) == "one");
  BOOST_TEST(table.at(2) == "two");
  BOOST_TEST(table.at(3) == "three");
}
