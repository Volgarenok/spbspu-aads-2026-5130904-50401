#include "bstree.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>

BOOST_AUTO_TEST_SUITE(bstree_suite)

BOOST_AUTO_TEST_CASE(test_create_empty_tree)
{
  hvostov::BSTree< int, std::string > tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_push_single_element)
{
  hvostov::BSTree< std::string, int > tree;
  tree.push("apple", 5);
  BOOST_CHECK(!tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.get("apple"), 5);
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_push_multiple_elements)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_push_updates_existing_key)
{
  hvostov::BSTree< std::string, int > tree;
  tree.push("key", 10);
  BOOST_CHECK_EQUAL(tree.get("key"), 10);
  tree.push("key", 20);
  BOOST_CHECK_EQUAL(tree.get("key"), 20);
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_get_modifiable_reference)
{
  hvostov::BSTree< std::string, int > tree;
  tree.push("first", 1);
  tree.get("first") = 100;
  BOOST_CHECK_EQUAL(tree.get("first"), 100);
}

BOOST_AUTO_TEST_CASE(test_get_throws_on_missing_key)
{
  hvostov::BSTree< int, std::string > tree;
  BOOST_CHECK_THROW(tree.get(42), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_get_const_on_missing_key)
{
  const hvostov::BSTree< int, std::string > tree;
  BOOST_CHECK_THROW(tree.get(42), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_drop_leaf_node)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  std::string value = tree.drop(1);
  BOOST_CHECK_EQUAL(value, "one");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.get(1), std::runtime_error);
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_drop_node_with_one_child)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(4, "four");
  std::string value = tree.drop(3);
  BOOST_CHECK_EQUAL(value, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_THROW(tree.get(3), std::runtime_error);
  BOOST_CHECK_EQUAL(tree.get(4), "four");
}

BOOST_AUTO_TEST_CASE(test_drop_node_with_two_children)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(4, "four");
  tree.push(3, "three");
  tree.push(5, "five");
  std::string value = tree.drop(4);
  BOOST_CHECK_EQUAL(value, "four");
  BOOST_CHECK_EQUAL(tree.size(), 4);
  BOOST_CHECK_THROW(tree.get(4), std::runtime_error);
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_EQUAL(tree.get(5), "five");
}

BOOST_AUTO_TEST_CASE(test_drop_root_node)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  std::string value = tree.drop(2);
  BOOST_CHECK_EQUAL(value, "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.get(2), std::runtime_error);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_drop_root_makes_tree_empty)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(42, "answer");
  tree.drop(42);
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_drop_throws_on_missing_key)
{
  hvostov::BSTree< int, std::string > tree;
  BOOST_CHECK_THROW(tree.drop(42), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_drop_all_elements)
{
  hvostov::BSTree< int, std::string > tree;
  std::vector< int > keys = {5, 3, 7, 2, 4, 6, 8};
  for (int key : keys) {
    tree.push(key, "val" + std::to_string(key));
  }
  for (int key : keys) {
    BOOST_CHECK_NO_THROW(tree.drop(key));
  }
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_height_updates_after_push)
{
  hvostov::BSTree< int, int > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
  tree.push(2, 2);
  BOOST_CHECK_EQUAL(tree.height(), 1);
  tree.push(1, 1);
  BOOST_CHECK_EQUAL(tree.height(), 2);
  tree.push(3, 3);
  BOOST_CHECK_EQUAL(tree.height(), 2);
  tree.push(0, 0);
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(test_height_updates_after_drop)
{
  hvostov::BSTree< int, int > tree;
  tree.push(2, 2);
  tree.push(1, 1);
  tree.push(3, 3);
  tree.push(0, 0);
  BOOST_CHECK_EQUAL(tree.height(), 3);
  tree.drop(0);
  BOOST_CHECK_EQUAL(tree.height(), 2);
  tree.drop(1);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
  tree.clear();
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  hvostov::BSTree< std::string, int > tree1;
  tree1.push("one", 1);
  tree1.push("two", 2);
  tree1.push("three", 3);
  hvostov::BSTree< std::string, int > tree2(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 3);
  BOOST_CHECK_EQUAL(tree2.get("one"), 1);
  BOOST_CHECK_EQUAL(tree2.get("two"), 2);
  BOOST_CHECK_EQUAL(tree2.get("three"), 3);
  BOOST_CHECK_EQUAL(tree2.height(), tree1.height());
  tree2.push("four", 4);
  BOOST_CHECK_THROW(tree1.get("four"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  hvostov::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  hvostov::BSTree< int, std::string > tree2(std::move(tree1));
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(2), "two");
  BOOST_CHECK(tree1.empty());
  BOOST_CHECK_EQUAL(tree1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  hvostov::BSTree< std::string, int > tree1;
  tree1.push("a", 1);
  tree1.push("b", 2);
  hvostov::BSTree< std::string, int > tree2;
  tree2.push("c", 3);
  tree2 = tree1;
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.get("a"), 1);
  BOOST_CHECK_EQUAL(tree2.get("b"), 2);
  BOOST_CHECK_THROW(tree2.get("c"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  hvostov::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  hvostov::BSTree< int, std::string > tree2;
  tree2.push(2, "two");
  tree2 = std::move(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_THROW(tree2.get(2), std::runtime_error);
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  hvostov::BSTree< std::string, int > tree1;
  tree1.push("first", 1);
  hvostov::BSTree< std::string, int > tree2;
  tree2.push("second", 2);
  tree1.swap(tree2);
  BOOST_CHECK_EQUAL(tree1.get("second"), 2);
  BOOST_CHECK_EQUAL(tree2.get("first"), 1);
  BOOST_CHECK_THROW(tree1.get("first"), std::runtime_error);
  BOOST_CHECK_THROW(tree2.get("second"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_rotate_left_simple)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  auto it = tree.cbegin();
  auto new_it = tree.rotateLeft(it);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  auto begin = tree.cbegin();
  BOOST_CHECK_EQUAL((*begin).data.first, 1);
}

BOOST_AUTO_TEST_CASE(test_rotate_right_simple)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  auto it = tree.cbegin();
  ++it;
  auto new_it = tree.rotateRight(it);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_left_complex)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  tree.push(3, "three");
  auto it = tree.cbegin();
  auto new_it = tree.rotateLeft(it);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_rotate_right_complex)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(2, "two");
  tree.push(1, "one");
  auto it = tree.cend();
  --it;
  auto new_it = tree.rotateRight(it);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(3, "three");
  tree.push(1, "one");
  tree.push(2, "two");
  auto it = tree.cend();
  --it;
  auto new_it = tree.rotateLargeRight(it);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(2, "two");
  auto it = tree.cbegin();
  auto new_it = tree.rotateLargeLeft(it);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(2), "two");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
}

BOOST_AUTO_TEST_CASE(test_height_after_rotations)
{
  hvostov::BSTree< int, int > tree;
  tree.push(1, 1);
  tree.push(2, 2);
  tree.push(3, 3);
  size_t height_before = tree.height();
  auto it = tree.cbegin();
  tree.rotateLeft(it);
  size_t height_after = tree.height();
  BOOST_CHECK(height_after <= height_before);
}

BOOST_AUTO_TEST_CASE(test_rotate_null_iterator)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  auto it = tree.cend();
  auto result = tree.rotateLeft(it);
  BOOST_CHECK(result == tree.cend());
  result = tree.rotateRight(it);
  BOOST_CHECK(result == tree.cend());
}

BOOST_AUTO_TEST_CASE(test_rotate_leaf_node)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  auto it = tree.cbegin();
  auto result = tree.rotateLeft(it);
  BOOST_CHECK(result == it);
}

BOOST_AUTO_TEST_CASE(test_begin_iterator)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  auto it = tree.begin();
  BOOST_CHECK_EQUAL((*it).data.first, 1);
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    keys.push_back((*it).data.first);
  }
  BOOST_CHECK_EQUAL(keys.size(), 3);
  BOOST_CHECK(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(test_const_iterator_traversal)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  std::vector< int > keys;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
    keys.push_back((*it).data.first);
  }
  BOOST_CHECK_EQUAL(keys.size(), 3);
  BOOST_CHECK(std::is_sorted(keys.begin(), keys.end()));
}

BOOST_AUTO_TEST_CASE(test_iterator_decrement)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  auto it = tree.begin();
  it++;
  it++;
  BOOST_CHECK_EQUAL((*it).data.first, 3);
  --it;
  BOOST_CHECK_EQUAL((*it).data.first, 2);
  --it;
  BOOST_CHECK_EQUAL((*it).data.first, 1);
}

BOOST_AUTO_TEST_CASE(test_large_number_of_elements)
{
  size_t N = 1000;
  hvostov::BSTree< int, int > tree;
  for (size_t i = 0; i < N; ++i) {
    tree.push(i, i * i);
  }
  BOOST_CHECK_EQUAL(tree.size(), N);
  for (size_t i = 0; i < N; ++i) {
    BOOST_CHECK_EQUAL(tree.get(i), i * i);
  }
}

BOOST_AUTO_TEST_CASE(test_different_key_types)
{
  hvostov::BSTree< double, std::string > tree;
  tree.push(3.14, "pi");
  tree.push(2.71, "e");
  tree.push(1.61, "golden");
  BOOST_CHECK_EQUAL(tree.get(3.14), "pi");
  BOOST_CHECK_EQUAL(tree.get(2.71), "e");
  BOOST_CHECK_EQUAL(tree.get(1.61), "golden");
}

BOOST_AUTO_TEST_CASE(test_custom_comparator)
{
  hvostov::BSTree< std::string, int, std::greater< std::string > > tree;
  tree.push("c", 3);
  tree.push("a", 1);
  tree.push("b", 2);
  std::vector< std::string > keys;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
    keys.push_back((*it).data.first);
  }
  BOOST_CHECK_EQUAL(keys[0], "c");
  BOOST_CHECK_EQUAL(keys[1], "b");
  BOOST_CHECK_EQUAL(keys[2], "a");
}

BOOST_AUTO_TEST_CASE(test_empty_tree_iterators)
{
  hvostov::BSTree< int, std::string > tree;
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(test_iterator_arrow_operator)
{
  hvostov::BSTree< std::string, int > tree;
  tree.push("test", 42);
  auto it = tree.begin();
  BOOST_CHECK_EQUAL(it->data.second, 42);
  BOOST_CHECK_EQUAL(it->data.first, "test");
}

BOOST_AUTO_TEST_SUITE_END()
