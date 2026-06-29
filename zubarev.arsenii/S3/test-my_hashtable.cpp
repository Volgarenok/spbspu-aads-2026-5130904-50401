#include <boost/test/unit_test.hpp>
#include <functional>

#include "my_hashtable.hpp"
#include "my_node_hashtable.hpp"
#include "my_siphash.hpp"

BOOST_AUTO_TEST_SUITE(my_hashtable_tests)

using Table = zubarev::HashTable< std::string, size_t, zubarev::SipHash, std::equal_to< std::string > >;
using Node = zubarev::detail::NodeHashTable< std::string, size_t >;
using Hash = zubarev::SipHash;
using Equal = std::equal_to< std::string >;
BOOST_AUTO_TEST_CASE(default_constructor)
{
  Table h;

  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_CASE(default_constructor_arg)
{
  Table h(3, 8, nullptr, nullptr, zubarev::List< Node >(), Hash(), Equal());

  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  Table a;
  a.insert("apple", 1);
  a.insert("banana", 2);

  Table copy(a);

  BOOST_CHECK(copy.contains("apple"));
  BOOST_CHECK(copy.contains("banana"));
  BOOST_CHECK(copy.at("apple") == 1);
  BOOST_CHECK(copy.at("banana") == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  Table a;
  a.insert("apple", 1);
  a.insert("banana", 2);

  Table move(std::move(a));

  BOOST_CHECK(move.contains("apple"));
  BOOST_CHECK(move.contains("banana"));
  BOOST_CHECK(move.at("apple") == 1);
  BOOST_CHECK(move.at("banana") == 2);
  BOOST_CHECK(a.begin() == a.end());
}

BOOST_AUTO_TEST_CASE(copy_assigment)
{
  Table a;
  a.insert("apple", 1);
  a.insert("banana", 2);

  Table copy;
  copy = a;
  BOOST_CHECK(copy.contains("apple"));
  BOOST_CHECK(copy.contains("banana"));
  BOOST_CHECK(copy.at("apple") == 1);
  BOOST_CHECK(copy.at("banana") == 2);
}

BOOST_AUTO_TEST_CASE(move_assigment)
{
  Table a;
  a.insert("apple", 1);
  a.insert("banana", 2);

  Table move;

  move = std::move(a);

  BOOST_CHECK(move.contains("apple"));
  BOOST_CHECK(move.contains("banana"));
  BOOST_CHECK(move.at("apple") == 1);
  BOOST_CHECK(move.at("banana") == 2);
  BOOST_CHECK(a.begin() == a.end());
}

BOOST_AUTO_TEST_CASE(operator_square_brackets_insert)
{
  Table a;
  a["apple"] = 123;

  BOOST_CHECK(a.contains("apple"));
  BOOST_CHECK(a["apple"] == 123);
}

BOOST_AUTO_TEST_CASE(at_non_const)
{
  Table a;
  a["apple"] = 123;

  BOOST_CHECK(a.contains("apple"));
  BOOST_CHECK(a.at("apple") == 123);
}

BOOST_AUTO_TEST_CASE(at_const)
{
  Table h;
  h.insert("apple", 123);
  const Table& h_const = h;
  BOOST_CHECK(h_const.at("apple") == 123);
}

BOOST_AUTO_TEST_CASE(insert)
{
  Table a;
  a.insert("apple", 123);

  BOOST_CHECK(a.contains("apple"));
  BOOST_CHECK(a.at("apple") == 123);
}
BOOST_AUTO_TEST_CASE(drop)
{
  Table a;
  a.insert("apple", 123);

  BOOST_CHECK(a.erase("apple") == 1);
  BOOST_CHECK(!a.contains("apple"));
}

BOOST_AUTO_TEST_CASE(contains)
{
  Table a;
  a.insert("apple", 123);

  BOOST_CHECK(a.contains("apple"));
  BOOST_CHECK(!a.contains("banana"));
}

BOOST_AUTO_TEST_CASE(rehash)

{
  Table h;
  h.insert("a", 1);
  h.insert("b", 2);
  h.insert("c", 3);
  h.rehash(128);

  BOOST_CHECK(h.contains("a"));
  BOOST_CHECK(h.contains("b"));
  BOOST_CHECK(h.contains("c"));

  BOOST_CHECK(h.at("a") == 1);
  BOOST_CHECK(h.at("b") == 2);
  BOOST_CHECK(h.at("c") == 3);
}

BOOST_AUTO_TEST_CASE(begin_end_non_empty)
{
  Table h;
  h.insert("a", 1);

  BOOST_CHECK(h.begin() != h.end());
}

BOOST_AUTO_TEST_CASE(const_begin_end_non_empty)
{
  Table h;
  h.insert("a", 1);
  const Table& h_const = h;

  BOOST_CHECK(h_const.begin() != h_const.end());
}

BOOST_AUTO_TEST_CASE(cbegin_cend)
{
  Table h;
  h.insert("a", 1);
  const Table& h_const = h;

  BOOST_CHECK(h_const.cbegin() != h_const.cend());
}

BOOST_AUTO_TEST_CASE(swap)
{
  Table h;
  h.insert("a", 1);
  h.insert("b", 2);
  Table h_empty;
  h.swap(h_empty);

  BOOST_CHECK(h_empty.contains("a"));
  BOOST_CHECK(h_empty.at("a") == 1);
  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_SUITE_END()
