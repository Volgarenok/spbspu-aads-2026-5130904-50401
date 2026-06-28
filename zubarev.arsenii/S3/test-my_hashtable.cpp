#include <boost/test/unit_test.hpp>

#include "../common/my_hashfunc/my_siphash.hpp"
#include "my_hashtable.hpp"
#include "my_node_hashtable.hpp"
#include "../common/my_hashfunc/my_equal.hpp"



BOOST_AUTO_TEST_SUITE(my_hashtable_tests)

using Table = zubarev::HashTable< std::string, size_t, zubarev::SipHash, zubarev::Equaler< std::string > >;
using Node = zubarev::NodeHashTable< std::string, size_t >;
using Hash = zubarev::SipHash;
using Equal = zubarev::Equaler< std::string >;
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
  a.add("apple", 1);
  a.add("banana", 2);

  Table copy(a);

  BOOST_CHECK(copy.has("apple"));
  BOOST_CHECK(copy.has("banana"));
  BOOST_CHECK(copy.at("apple") == 1);
  BOOST_CHECK(copy.at("banana") == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  Table a;
  a.add("apple", 1);
  a.add("banana", 2);

  Table move(std::move(a));

  BOOST_CHECK(move.has("apple"));
  BOOST_CHECK(move.has("banana"));
  BOOST_CHECK(move.at("apple") == 1);
  BOOST_CHECK(move.at("banana") == 2);
  BOOST_CHECK(a.begin() == a.end());
}

BOOST_AUTO_TEST_CASE(copy_assigment)
{
  Table a;
  a.add("apple", 1);
  a.add("banana", 2);

  Table copy;
  copy = a;
  BOOST_CHECK(copy.has("apple"));
  BOOST_CHECK(copy.has("banana"));
  BOOST_CHECK(copy.at("apple") == 1);
  BOOST_CHECK(copy.at("banana") == 2);
}

BOOST_AUTO_TEST_CASE(move_assigment)
{
  Table a;
  a.add("apple", 1);
  a.add("banana", 2);

  Table move;

  move = std::move(a);

  BOOST_CHECK(move.has("apple"));
  BOOST_CHECK(move.has("banana"));
  BOOST_CHECK(move.at("apple") == 1);
  BOOST_CHECK(move.at("banana") == 2);
  BOOST_CHECK(a.begin() == a.end());
}

BOOST_AUTO_TEST_CASE(operator_square_brackets_insert)
{
  Table a;
  a["apple"] = 123;

  BOOST_CHECK(a.has("apple"));
  BOOST_CHECK(a["apple"] == 123);
}

BOOST_AUTO_TEST_CASE(operator_square_brackets_const)
{
  Table h;
  h.add("apple", 123);
  const Table& h_const = h;
  BOOST_CHECK(h_const["apple"] == 123);
}

BOOST_AUTO_TEST_CASE(at_non_const)
{
  Table a;
  a["apple"] = 123;

  BOOST_CHECK(a.has("apple"));
  BOOST_CHECK(a.at("apple") == 123);
}

BOOST_AUTO_TEST_CASE(at_const)
{
  Table h;
  h.add("apple", 123);
  const Table& h_const = h;
  BOOST_CHECK(h_const.at("apple") == 123);
}

BOOST_AUTO_TEST_CASE(add)
{
  Table a;
  a.add("apple", 123);

  BOOST_CHECK(a.has("apple"));
  BOOST_CHECK(a.at("apple") == 123);
}
BOOST_AUTO_TEST_CASE(drop)
{
  Table a;
  a.add("apple", 123);

  BOOST_CHECK(a.drop("apple") == 123);
  BOOST_CHECK(!a.has("apple"));
}

BOOST_AUTO_TEST_CASE(has)
{
  Table a;
  a.add("apple", 123);

  BOOST_CHECK(a.has("apple"));
  BOOST_CHECK(!a.has("banana"));
}

BOOST_AUTO_TEST_CASE(rehash)

{
  Table h;
  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);
  h.rehash(128);

  BOOST_CHECK(h.has("a"));
  BOOST_CHECK(h.has("b"));
  BOOST_CHECK(h.has("c"));

  BOOST_CHECK(h.at("a") == 1);
  BOOST_CHECK(h.at("b") == 2);
  BOOST_CHECK(h.at("c") == 3);
}

BOOST_AUTO_TEST_CASE(begin_end_non_empty)
{
  Table h;
  h.add("a", 1);

  BOOST_CHECK(h.begin() != h.end());
}

BOOST_AUTO_TEST_CASE(const_begin_end_non_empty)
{
  Table h;
  h.add("a", 1);
  const Table& h_const = h;

  BOOST_CHECK(h_const.begin() != h_const.end());
}

BOOST_AUTO_TEST_CASE(cbegin_cend)
{
  Table h;
  h.add("a", 1);
  const Table& h_const = h;

  BOOST_CHECK(h_const.cbegin() != h_const.cend());
}

BOOST_AUTO_TEST_CASE(swap)
{
  Table h;
  h.add("a", 1);
  h.add("b", 2);
  Table h_empty;
  h.swap(h_empty);

  BOOST_CHECK(h_empty.has("a"));
  BOOST_CHECK(h_empty.at("a") == 1);
  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_SUITE_END()
