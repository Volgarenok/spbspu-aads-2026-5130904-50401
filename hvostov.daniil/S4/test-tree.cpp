#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>
#include "bstree.hpp"

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
  BOOST_CHECK_EQUAL(tree.at("apple"), 5);
  BOOST_CHECK_EQUAL(tree.height(), 1);
}

BOOST_AUTO_TEST_CASE(test_push_multiple_elements)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_push_updates_existing_key)
{
  hvostov::BSTree< std::string, int > tree;
  tree.push("key", 10);
  BOOST_CHECK_EQUAL(tree.at("key"), 10);
  tree.push("key", 20);
  BOOST_CHECK_EQUAL(tree.at("key"), 20);
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_at_modifiable_reference)
{
  hvostov::BSTree< std::string, int > tree;
  tree.push("first", 1);
  tree.at("first") = 100;
  BOOST_CHECK_EQUAL(tree.at("first"), 100);
}

BOOST_AUTO_TEST_CASE(test_at_throws_on_missing_key)
{
  hvostov::BSTree< int, std::string > tree;
  BOOST_CHECK_THROW(tree.at(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_at_const_on_missing_key)
{
  const hvostov::BSTree< int, std::string > tree;
  BOOST_CHECK_THROW(tree.at(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets_creates_if_not_exists)
{
  hvostov::BSTree< int, std::string > tree;
  tree[42] = "answer";
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.at(42), "answer");
}

BOOST_AUTO_TEST_CASE(test_operator_brackets_modifies_existing)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(42, "old");
  tree[42] = "new";
  BOOST_CHECK_EQUAL(tree.at(42), "new");
  BOOST_CHECK_EQUAL(tree.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_erase_leaf_node)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  size_t erased = tree.erase(1);
  BOOST_CHECK_EQUAL(erased, 1);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.at(1), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_erase_node_with_one_child)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(4, "four");
  size_t erased = tree.erase(3);
  BOOST_CHECK_EQUAL(erased, 1);
  BOOST_CHECK_EQUAL(tree.size(), 3);
  BOOST_CHECK_THROW(tree.at(3), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.at(4), "four");
}

BOOST_AUTO_TEST_CASE(test_erase_node_with_two_children)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(4, "four");
  tree.push(3, "three");
  tree.push(5, "five");
  size_t erased = tree.erase(4);
  BOOST_CHECK_EQUAL(erased, 1);
  BOOST_CHECK_EQUAL(tree.size(), 4);
  BOOST_CHECK_THROW(tree.at(4), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.at(3), "three");
  BOOST_CHECK_EQUAL(tree.at(5), "five");
}

BOOST_AUTO_TEST_CASE(test_erase_root_node)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  size_t erased = tree.erase(2);
  BOOST_CHECK_EQUAL(erased, 1);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK_THROW(tree.at(2), std::out_of_range);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_erase_root_makes_tree_empty)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(42, "answer");
  tree.erase(42);
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_erase_returns_zero_on_missing_key)
{
  hvostov::BSTree< int, std::string > tree;
  size_t erased = tree.erase(42);
  BOOST_CHECK_EQUAL(erased, 0);
  BOOST_CHECK_EQUAL(tree.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_erase_all_elements)
{
  hvostov::BSTree< int, std::string > tree;
  std::vector< int > keys = {5, 3, 7, 2, 4, 6, 8};
  for (int key : keys) {
    tree.push(key, "val" + std::to_string(key));
  }
  for (int key : keys) {
    size_t erased = tree.erase(key);
    BOOST_CHECK_EQUAL(erased, 1);
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

BOOST_AUTO_TEST_CASE(test_height_updates_after_erase)
{
  hvostov::BSTree< int, int > tree;
  tree.push(2, 2);
  tree.push(1, 1);
  tree.push(3, 3);
  tree.push(0, 0);
  BOOST_CHECK_EQUAL(tree.height(), 3);
  tree.erase(0);
  BOOST_CHECK_EQUAL(tree.height(), 2);
  tree.erase(1);
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
  BOOST_CHECK_EQUAL(tree2.at("one"), 1);
  BOOST_CHECK_EQUAL(tree2.at("two"), 2);
  BOOST_CHECK_EQUAL(tree2.at("three"), 3);
  BOOST_CHECK_EQUAL(tree2.height(), tree1.height());
  tree2.push("four", 4);
  BOOST_CHECK_THROW(tree1.at("four"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  hvostov::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  hvostov::BSTree< int, std::string > tree2(std::move(tree1));
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_EQUAL(tree2.at(2), "two");
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
  BOOST_CHECK_EQUAL(tree2.at("a"), 1);
  BOOST_CHECK_EQUAL(tree2.at("b"), 2);
  BOOST_CHECK_THROW(tree2.at("c"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  hvostov::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  hvostov::BSTree< int, std::string > tree2;
  tree2.push(2, "two");
  tree2 = std::move(tree1);
  BOOST_CHECK_EQUAL(tree2.size(), 1);
  BOOST_CHECK_EQUAL(tree2.at(1), "one");
  BOOST_CHECK_THROW(tree2.at(2), std::out_of_range);
  BOOST_CHECK(tree1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  hvostov::BSTree< std::string, int > tree1;
  tree1.push("first", 1);
  hvostov::BSTree< std::string, int > tree2;
  tree2.push("second", 2);
  tree1.swap(tree2);
  BOOST_CHECK_EQUAL(tree1.at("second"), 2);
  BOOST_CHECK_EQUAL(tree2.at("first"), 1);
  BOOST_CHECK_THROW(tree1.at("first"), std::out_of_range);
  BOOST_CHECK_THROW(tree2.at("second"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_rotate_left_simple)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");
  auto it = tree.cbegin();
  auto new_it = tree.rotateLeft(it);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.size(), 2);
  auto begin = tree.cbegin();
  BOOST_CHECK_EQUAL((*begin).first, 1);
}

BOOST_AUTO_TEST_CASE(test_rotate_right_simple)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  auto it = tree.cbegin();
  ++it;
  auto new_it = tree.rotateRight(it);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
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
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
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
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
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
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(2, "two");
  auto it = tree.cbegin();
  auto new_it = tree.rotateLargeLeft(it);
  BOOST_CHECK_EQUAL(tree.at(1), "one");
  BOOST_CHECK_EQUAL(tree.at(2), "two");
  BOOST_CHECK_EQUAL(tree.at(3), "three");
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
  BOOST_CHECK_EQUAL((*it).first, 1);
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");
  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it) {
    keys.push_back((*it).first);
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
    keys.push_back((*it).first);
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
  ++it;
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 3);
  --it;
  BOOST_CHECK_EQUAL((*it).first, 2);
  --it;
  BOOST_CHECK_EQUAL((*it).first, 1);
}

BOOST_AUTO_TEST_CASE(test_large_number_of_elements)
{
  size_t N = 1000;
  hvostov::BSTree< int, int > tree;
  for (size_t i = 0; i < N; ++i) {
    tree.push(i, static_cast< int >(i * i));
  }
  BOOST_CHECK_EQUAL(tree.size(), N);
  for (size_t i = 0; i < N; ++i) {
    BOOST_CHECK_EQUAL(tree.at(static_cast< int >(i)), static_cast< int >(i * i));
  }
}

BOOST_AUTO_TEST_CASE(test_different_key_types)
{
  hvostov::BSTree< double, std::string > tree;
  tree.push(3.14, "pi");
  tree.push(2.71, "e");
  tree.push(1.61, "golden");
  BOOST_CHECK_EQUAL(tree.at(3.14), "pi");
  BOOST_CHECK_EQUAL(tree.at(2.71), "e");
  BOOST_CHECK_EQUAL(tree.at(1.61), "golden");
}

BOOST_AUTO_TEST_CASE(test_custom_comparator)
{
  hvostov::BSTree< std::string, int, std::greater< std::string > > tree;
  tree.push("c", 3);
  tree.push("a", 1);
  tree.push("b", 2);
  std::vector< std::string > keys;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it) {
    keys.push_back((*it).first);
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
  BOOST_CHECK_EQUAL(it->second, 42);
  BOOST_CHECK_EQUAL(it->first, "test");
}

BOOST_AUTO_TEST_CASE(test_has_method)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(!tree.has(2));
}

BOOST_AUTO_TEST_CASE(test_push_rvalue)
{
  hvostov::BSTree< int, std::string > tree;
  std::string value = "hello";
  tree.push(1, std::move(value));
  BOOST_CHECK_EQUAL(tree.at(1), "hello");
}

BOOST_AUTO_TEST_CASE(test_erase_successor_not_direct_child)
{
  hvostov::BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(6, "six");
  tree.push(8, "eight");
  size_t erased = tree.erase(5);
  BOOST_CHECK_EQUAL(erased, 1);
  BOOST_CHECK_EQUAL(tree.size(), 4);
  BOOST_CHECK_EQUAL(tree.at(6), "six");
  BOOST_CHECK_EQUAL(tree.at(7), "seven");
}

BOOST_AUTO_TEST_SUITE_END()
