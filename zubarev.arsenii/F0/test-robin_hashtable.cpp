#include "robin_hashtable.hpp"
#include <my_hashfunc/my_equal.hpp>
#include <my_hashfunc/my_siphash.hpp>

#include <boost/test/unit_test.hpp>
#include <string>

BOOST_AUTO_TEST_SUITE(robin_hashtable_tests)

using Table = zubarev::RobinHashTable< std::string, size_t, zubarev::SipHash, zubarev::Equaler< std::string > >;

using Hash = zubarev::SipHash;
using Equal = zubarev::Equaler< std::string >;

BOOST_AUTO_TEST_CASE(default_constructor)
{
  Table h;

  BOOST_CHECK(h.begin() == h.end());
  BOOST_CHECK(h.empty());
  BOOST_CHECK(h.size() == 0);
}

BOOST_AUTO_TEST_CASE(default_constructor_capacity)
{
  Table h(32);

  BOOST_CHECK(h.capacity() == 32);
  BOOST_CHECK(h.empty());
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

  BOOST_CHECK(copy.size() == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  Table a;

  a.add("apple", 1);
  a.add("banana", 2);

  Table moved(std::move(a));

  BOOST_CHECK(moved.has("apple"));
  BOOST_CHECK(moved.has("banana"));

  BOOST_CHECK(moved.at("apple") == 1);
  BOOST_CHECK(moved.at("banana") == 2);

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  Table a;

  a.add("apple", 100);

  Table b;

  b = a;

  BOOST_CHECK(b.has("apple"));
  BOOST_CHECK(b.at("apple") == 100);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  Table a;

  a.add("apple", 100);

  Table b;

  b = std::move(a);

  BOOST_CHECK(b.has("apple"));
  BOOST_CHECK(b.at("apple") == 100);

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(operator_square_brackets_insert)
{
  Table h;

  h["hello"] = 123;

  BOOST_CHECK(h.has("hello"));
  BOOST_CHECK(h["hello"] == 123);
}

BOOST_AUTO_TEST_CASE(operator_square_brackets_update)
{
  Table h;

  h["key"] = 10;
  h["key"] = 50;

  BOOST_CHECK(h.size() == 1);
  BOOST_CHECK(h.at("key") == 50);
}

BOOST_AUTO_TEST_CASE(at_non_const)
{
  Table h;

  h.add("test", 42);

  BOOST_CHECK(h.at("test") == 42);

  h.at("test") = 100;

  BOOST_CHECK(h.at("test") == 100);
}

BOOST_AUTO_TEST_CASE(at_const)
{
  Table h;

  h.add("value", 77);

  const Table& ref = h;

  BOOST_CHECK(ref.at("value") == 77);
}

BOOST_AUTO_TEST_CASE(add)
{
  Table h;

  h.add("one", 1);

  BOOST_CHECK(h.has("one"));
  BOOST_CHECK(h.size() == 1);
}

BOOST_AUTO_TEST_CASE(add_many_elements)
{
  Table h;

  for (size_t i = 0; i < 1000; i++) {
    h.add("key" + std::to_string(i), i);
  }

  BOOST_CHECK(h.size() == 1000);

  for (size_t i = 0; i < 1000; i++) {
    BOOST_CHECK(h.has("key" + std::to_string(i)));
    BOOST_CHECK(h.at("key" + std::to_string(i)) == i);
  }
}

BOOST_AUTO_TEST_CASE(drop)
{
  Table h;

  h.add("apple", 123);

  size_t value = h.drop("apple");

  BOOST_CHECK(value == 123);
  BOOST_CHECK(!h.has("apple"));
  BOOST_CHECK(h.empty());
}

BOOST_AUTO_TEST_CASE(drop_with_collision_chain)
{
  Table h(8);

  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);

  BOOST_CHECK(h.has("a"));
  BOOST_CHECK(h.has("b"));
  BOOST_CHECK(h.has("c"));

  h.drop("b");

  BOOST_CHECK(!h.has("b"));

  BOOST_CHECK(h.has("a"));
  BOOST_CHECK(h.has("c"));
}

BOOST_AUTO_TEST_CASE(has)
{
  Table h;

  h.add("apple", 1);

  BOOST_CHECK(h.has("apple"));
  BOOST_CHECK(!h.has("banana"));
}

BOOST_AUTO_TEST_CASE(rehash)
{
  Table h;

  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);

  h.rehash(128);

  BOOST_CHECK(h.capacity() == 128);

  BOOST_CHECK(h.has("a"));
  BOOST_CHECK(h.has("b"));
  BOOST_CHECK(h.has("c"));

  BOOST_CHECK(h.at("a") == 1);
  BOOST_CHECK(h.at("b") == 2);
  BOOST_CHECK(h.at("c") == 3);
}

BOOST_AUTO_TEST_CASE(auto_rehash_by_load_factor)
{
  Table h(8);

  for (size_t i = 0; i < 20; i++) {
    h.add("number" + std::to_string(i), i);
  }

  BOOST_CHECK(h.size() == 20);

  for (size_t i = 0; i < 20; i++) {
    BOOST_CHECK(h.has("number" + std::to_string(i)));
  }
}

BOOST_AUTO_TEST_CASE(begin_end_empty)
{
  Table h;

  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_CASE(begin_end_non_empty)
{
  Table h;

  h.add("hello", 5);

  BOOST_CHECK(h.begin() != h.end());
}

BOOST_AUTO_TEST_CASE(const_begin_end)
{
  Table h;

  h.add("hello", 5);

  const Table& ref = h;

  BOOST_CHECK(ref.begin() != ref.end());
}

BOOST_AUTO_TEST_CASE(cbegin_cend)
{
  Table h;

  h.add("hello", 5);

  BOOST_CHECK(h.cbegin() != h.cend());
}

BOOST_AUTO_TEST_CASE(iterator_traversal)
{
  Table h;

  h.add("a", 1);
  h.add("b", 2);
  h.add("c", 3);

  size_t count = 0;

  for (auto it = h.begin(); it != h.end(); ++it) {
    count++;
  }

  BOOST_CHECK(count == 3);
}

BOOST_AUTO_TEST_CASE(swap)
{
  Table a;

  a.add("x", 100);

  Table b;

  b.swap(a);

  BOOST_CHECK(b.has("x"));
  BOOST_CHECK(b.at("x") == 100);

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(load_factor)
{
  Table h(100);

  h.add("a", 1);
  h.add("b", 2);

  BOOST_CHECK_CLOSE(h.load_factor(), 0.02, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
