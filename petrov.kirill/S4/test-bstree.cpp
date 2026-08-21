#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "bstree.hpp"
#include <iostream>

struct BSTreeFixture {
  petrov::BSTree<int, std::string> tree;

  void populate_basic_nodes() {
    tree.push(100, "Root");
    tree.push(50,  "LeftChild");
    tree.push(150, "RightChild");
  }
};

BOOST_FIXTURE_TEST_SUITE(BSTree_Operations_Suite, BSTreeFixture)

BOOST_AUTO_TEST_CASE(Verify_Empty_Tree_Behavior) {
  BOOST_TEST(tree.height() == 0);
  BOOST_TEST(tree.has(999) == false);

  BOOST_REQUIRE_THROW(tree.drop(10), std::out_of_range);
  BOOST_REQUIRE_THROW(tree.get(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Insert_New_And_Update_Existing_Nodes) {
  populate_basic_nodes();

  BOOST_TEST(tree.height() == 2);
  BOOST_TEST(tree.has(100) == true);
  BOOST_TEST(tree.get(50) == "LeftChild");

  tree.push(50, "UpdatedLeftChild");

  BOOST_TEST(tree.get(50) == "UpdatedLeftChild");
  BOOST_TEST(tree.height() == 2);
}

BOOST_AUTO_TEST_CASE(Drop_Nodes_And_Check_Integrity) {
  populate_basic_nodes();
  tree.push(25, "LeftLeft");

  std::string dropped_leaf = tree.drop(25);
  BOOST_TEST(dropped_leaf == "LeftLeft");
  BOOST_TEST(tree.has(25) == false);

  std::string dropped_root = tree.drop(100);
  BOOST_TEST(dropped_root == "Root");
  BOOST_TEST(tree.has(100) == false);

  BOOST_TEST(tree.has(50) == true);
  BOOST_TEST(tree.has(150) == true);
}

BOOST_AUTO_TEST_CASE(Tree_Rotations_Logic) {
  tree.push(10, "A");
  tree.push(20, "B");
  tree.push(30, "C");

  BOOST_TEST(tree.height() == 3);

  auto it = tree.cbegin();
  ++it;

  auto new_subtree_root = tree.rotateLeft(it);

  BOOST_TEST(new_subtree_root->key == 20);
  BOOST_TEST(tree.height() == 2);
}

BOOST_AUTO_TEST_CASE(Iterators_Sequential_Traversal) {
  populate_basic_nodes();

  auto it = tree.cbegin();

  BOOST_REQUIRE(it != tree.cend());

  BOOST_TEST(it->key == 50);

  ++it;
  BOOST_TEST(it->key == 100);

  ++it;
  BOOST_TEST(it->key == 150);

  ++it;

  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_SUITE_END()
