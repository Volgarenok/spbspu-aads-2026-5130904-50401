#include "my_compare.hpp"
#include "my_node_tree.hpp"
#include "my_tree.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(my_tree_tests)

using Table = zubarev::BSTree< std::string, size_t, zubarev::Comparator< std::string > >;
using Node = zubarev::BSTreeNode< std::string, size_t >;

using Compare = zubarev::Comparator< std::string >;
BOOST_AUTO_TEST_CASE(default_constructor)
{
  Table h;

  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  Table a;
  a.push("apple", 1);
  a.push("banana", 2);

  Table copy(a);

  BOOST_CHECK(copy.contains("apple"));
  BOOST_CHECK(copy.contains("banana"));
  BOOST_CHECK(copy.at("apple") == 1);
  BOOST_CHECK(copy.at("banana") == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  Table a;
  a.push("apple", 1);
  a.push("banana", 2);

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
  a.push("apple", 1);
  a.push("banana", 2);

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
  a.push("apple", 1);
  a.push("banana", 2);

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

BOOST_AUTO_TEST_CASE(operator_square_brackets_const)
{
  Table h;
  h.push("apple", 123);
  const Table& h_const = h;
  BOOST_CHECK(h_const["apple"] == 123);
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
  h.push("apple", 123);
  const Table& h_const = h;
  BOOST_CHECK(h_const.at("apple") == 123);
}

BOOST_AUTO_TEST_CASE(push)
{
  Table a;
  a.push("apple", 123);

  BOOST_CHECK(a.contains("apple"));
  BOOST_CHECK(a.at("apple") == 123);
}
BOOST_AUTO_TEST_CASE(drop)
{
  Table a;
  a.push("apple", 123);

  BOOST_CHECK(a.drop("apple") == 123);
  BOOST_CHECK(!a.contains("apple"));
}

BOOST_AUTO_TEST_CASE(contains)
{
  Table a;
  a.push("apple", 123);

  BOOST_CHECK(a.contains("apple"));
  BOOST_CHECK(!a.contains("banana"));
}

BOOST_AUTO_TEST_CASE(begin_end_non_empty)
{
  Table h;
  h.push("a", 1);

  BOOST_CHECK(h.begin() != h.end());
}

BOOST_AUTO_TEST_CASE(const_begin_end_non_empty)
{
  Table h;
  h.push("a", 1);
  const Table& h_const = h;

  BOOST_CHECK(h_const.begin() != h_const.end());
}

BOOST_AUTO_TEST_CASE(cbegin_cend)
{
  Table h;
  h.push("a", 1);
  const Table& h_const = h;

  BOOST_CHECK(h_const.cbegin() != h_const.cend());
}

BOOST_AUTO_TEST_CASE(swap)
{
  Table h;
  h.push("a", 1);
  h.push("b", 2);
  Table h_empty;
  h.swap(h_empty);

  BOOST_CHECK(h_empty.contains("a"));
  BOOST_CHECK(h_empty.at("a") == 1);
  BOOST_CHECK(h.begin() == h.end());
}

BOOST_AUTO_TEST_CASE(height)
{
  Table a;
  a.push("apple", 1);
  a.push("banana", 2);
  BOOST_CHECK(a.height() == 2);
}

BOOST_AUTO_TEST_CASE(drop_root_with_two_children)
{
  Table t;
  t.push("b", 2);
  t.push("a", 1);
  t.push("c", 3);

  auto val = t.drop("b");

  BOOST_CHECK(val == 2);
  BOOST_CHECK(!t.contains("b"));
  BOOST_CHECK(t.contains("a"));
  BOOST_CHECK(t.contains("c"));
}

BOOST_AUTO_TEST_CASE(drop_leaf_node)
{
  Table t;
  t.push("a", 1);
  t.push("b", 2);

  auto val = t.drop("b");

  BOOST_CHECK(val == 2);
  BOOST_CHECK(!t.contains("b"));
  BOOST_CHECK(t.contains("a"));
}

BOOST_AUTO_TEST_CASE(drop_one_child)
{
  Table t;
  t.push("b", 2);
  t.push("a", 1);

  auto val = t.drop("b");

  BOOST_CHECK(val == 2);
  BOOST_CHECK(!t.contains("b"));
  BOOST_CHECK(t.contains("a"));
}
BOOST_AUTO_TEST_CASE(height_linear_tree)
{
  Table t;
  t.push("a", 1);
  t.push("b", 2);
  t.push("c", 3);
  t.push("d", 4);

  BOOST_CHECK(t.height() == 4);
}
BOOST_AUTO_TEST_CASE(height_balanced)
{
  Table t;
  t.push("b", 2);
  t.push("a", 1);
  t.push("c", 3);

  BOOST_CHECK(t.height() == 2);
}

BOOST_AUTO_TEST_SUITE_END()
