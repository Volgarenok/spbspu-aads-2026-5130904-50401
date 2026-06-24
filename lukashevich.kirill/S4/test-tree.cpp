#include <boost/test/unit_test.hpp>

#include <string>
#include <utility>

#include "compare.hpp"
#include "tree.hpp"

namespace
{
  using Tree = lukashevich::BSTree< int, std::string,
      lukashevich::Comparator< int > >;
}

BOOST_AUTO_TEST_CASE(insert_and_iteration)
{
  Tree tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  Tree::const_iterator iterator = tree.begin();
  BOOST_TEST(iterator->first == 1);
  ++iterator;
  BOOST_TEST(iterator->first == 2);
  ++iterator;
  BOOST_TEST(iterator->first == 3);
  ++iterator;
  BOOST_CHECK(iterator == tree.end());
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  Tree source;
  source.push(1, "one");
  source.push(2, "two");

  Tree copy(source);
  copy.at(1) = "changed";

  BOOST_TEST(source.at(1) == "one");
  BOOST_TEST(copy.at(1) == "changed");
}

BOOST_AUTO_TEST_CASE(erase_node_with_two_children)
{
  Tree tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  BOOST_TEST(tree.drop(2) == "two");
  BOOST_TEST(!tree.contains(2));
  BOOST_TEST(tree.contains(1));
  BOOST_TEST(tree.contains(3));
}

BOOST_AUTO_TEST_CASE(left_rotation_lifts_passed_node)
{
  Tree tree;
  tree.push(10, "ten");
  tree.push(20, "twenty");

  const Tree::const_iterator lifted = tree.rotateLeft(tree.find(20));

  BOOST_TEST(lifted->first == 20);
  BOOST_TEST(tree.begin()->first == 10);
  BOOST_TEST(tree.height() == 2);
}

BOOST_AUTO_TEST_CASE(right_rotation_lifts_passed_node)
{
  Tree tree;
  tree.push(20, "twenty");
  tree.push(10, "ten");

  const Tree::const_iterator lifted = tree.rotateRight(tree.find(10));

  BOOST_TEST(lifted->first == 10);
  BOOST_TEST(tree.begin()->first == 10);
  BOOST_TEST(tree.height() == 2);
}
