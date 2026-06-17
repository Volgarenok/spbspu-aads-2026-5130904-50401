#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <string>
#include <vector>
#include "AVLTree.hpp"

BOOST_AUTO_TEST_SUITE(AVLTreeTests)

BOOST_AUTO_TEST_CASE(constructor_and_basic_insertion)
{
  vasyakin::AVLTree< int, std::string > tree;

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0u);
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());

  tree.insert(52, "Alblak");

  BOOST_CHECK(!tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 1u);
  BOOST_CHECK_EQUAL(tree.at(52), "Alblak");
}

BOOST_AUTO_TEST_CASE(copy_and_move_semantics)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(5, "five");

  vasyakin::AVLTree< int, std::string > yatree(tree);

  BOOST_CHECK_EQUAL(yatree.size(), tree.size());
  BOOST_CHECK(yatree.has(10));
  BOOST_CHECK(yatree.has(20));
  BOOST_CHECK(yatree.has(5));

  yatree.remove(10);

  BOOST_CHECK_EQUAL(yatree.size(), 2u);
  BOOST_CHECK(yatree.has(20));
  BOOST_CHECK(yatree.has(5));

  BOOST_CHECK_EQUAL(tree.size(), 3u);
  BOOST_CHECK(tree.has(10));

  vasyakin::AVLTree< int, std::string > assigned;
  assigned = tree;

  BOOST_CHECK_EQUAL(assigned.size(), tree.size());
  BOOST_CHECK(assigned.has(10));
  BOOST_CHECK(assigned.has(20));
  BOOST_CHECK(assigned.has(5));

  assigned.remove(5);

  BOOST_CHECK_EQUAL(assigned.size(), 2u);
  BOOST_CHECK_EQUAL(tree.size(), 3u);
  BOOST_CHECK(tree.has(5));

  vasyakin::AVLTree< int, std::string > moved(std::move(tree));

  BOOST_CHECK_EQUAL(moved.size(), 3u);
  BOOST_CHECK_EQUAL(tree.size(), 0u);
  BOOST_CHECK(moved.has(10));
  BOOST_CHECK(moved.has(20));
  BOOST_CHECK(moved.has(5));
  BOOST_CHECK(tree.empty());

  vasyakin::AVLTree< int, std::string > move_assigned;
  move_assigned = std::move(moved);

  BOOST_CHECK_EQUAL(move_assigned.size(), 3u);
  BOOST_CHECK_EQUAL(moved.size(), 0u);
  BOOST_CHECK(move_assigned.has(10));
  BOOST_CHECK(move_assigned.has(20));
  BOOST_CHECK(move_assigned.has(5));
  BOOST_CHECK(moved.empty());
}

BOOST_AUTO_TEST_CASE(self_assignment_safety)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(1, "one");
  tree.insert(2, "two");

  tree = tree;

  BOOST_CHECK_EQUAL(tree.size(), 2u);
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));

  tree = std::move(tree);

  BOOST_CHECK_EQUAL(tree.size(), 2u);
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
}

BOOST_AUTO_TEST_CASE(destructor_and_clear_safety)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(1, "one");
  tree.insert(2, "two");
  tree.insert(3, "three");

  tree.clear();
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0u);
  BOOST_CHECK(tree.begin() == tree.end());

  BOOST_CHECK_NO_THROW(tree.clear());
  BOOST_CHECK(tree.empty());

  {
    vasyakin::AVLTree< int, std::string > scoped_tree;
    scoped_tree.insert(10, "ten");
    scoped_tree.insert(20, "twenty");
    BOOST_CHECK_EQUAL(scoped_tree.size(), 2u);
  }

  {
    vasyakin::AVLTree< int, int > empty_tree;
  }
}
