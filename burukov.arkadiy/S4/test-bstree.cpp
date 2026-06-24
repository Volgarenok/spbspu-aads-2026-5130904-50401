#include <boost/test/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "bstree.hpp"

using namespace burukov;

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(default_constructor_makes_empty_tree)
{
  BSTree< int, std::string > t;
  BOOST_CHECK(t.empty());
  BOOST_CHECK_EQUAL(t.size(), 0);
  BOOST_CHECK(t.begin() == t.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor_creates_deep_copy)
{
  BSTree< int, std::string > t1;
  t1.push(1, "one");
  t1.push(2, "two");
  BSTree< int, std::string > t2(t1);
  BOOST_CHECK_EQUAL(t2.size(), 2);
  BOOST_CHECK_EQUAL(t2.at(1), "one");
  t2.at(1) = "ONE";
  BOOST_CHECK_EQUAL(t1.at(1), "one");
}

BOOST_AUTO_TEST_CASE(move_constructor_transfers_ownership)
{
  BSTree< int, std::string > t1;
  t1.push(1, "one");
  BSTree< int, std::string > t2(std::move(t1));
  BOOST_CHECK_EQUAL(t2.size(), 1);
  BOOST_CHECK(t1.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment_creates_deep_copy)
{
  BSTree< int, std::string > t1, t2;
  t1.push(1, "one");
  t2 = t1;
  BOOST_CHECK_EQUAL(t2.size(), 1);
  BOOST_CHECK_EQUAL(t2.at(1), "one");
}

BOOST_AUTO_TEST_CASE(move_assignment_transfers_ownership)
{
  BSTree< int, std::string > t1, t2;
  t1.push(1, "one");
  t2 = std::move(t1);
  BOOST_CHECK_EQUAL(t2.size(), 1);
  BOOST_CHECK(t1.empty());
}

BOOST_AUTO_TEST_CASE(push_adds_new_elements)
{
  BSTree< int, std::string > t;
  t.push(5, "five");
  t.push(3, "three");
  t.push(7, "seven");
  BOOST_CHECK_EQUAL(t.size(), 3);
  BOOST_CHECK_EQUAL(t.at(5), "five");
  BOOST_CHECK_EQUAL(t.at(3), "three");
  BOOST_CHECK_EQUAL(t.at(7), "seven");
}

BOOST_AUTO_TEST_CASE(push_updates_existing_key)
{
  BSTree< int, std::string > t;
  t.push(42, "answer");
  t.push(42, "ANSWER");
  BOOST_CHECK_EQUAL(t.size(), 1);
  BOOST_CHECK_EQUAL(t.at(42), "ANSWER");
}

BOOST_AUTO_TEST_CASE(push_with_rvalue_moves_value)
{
  BSTree< int, std::string > t;
  std::string val = "test";
  t.push(99, std::move(val));
  BOOST_CHECK_EQUAL(t.at(99), "test");
}

BOOST_AUTO_TEST_CASE(at_returns_correct_value)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  t.push(2, "two");
  BOOST_CHECK_EQUAL(t.at(1), "one");
  BOOST_CHECK_EQUAL(t.at(2), "two");
}

BOOST_AUTO_TEST_CASE(at_allows_modification)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  t.at(1) = "ONE";
  BOOST_CHECK_EQUAL(t.at(1), "ONE");
}

BOOST_AUTO_TEST_CASE(const_at_works_on_const_object)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  const auto& ct = t;
  BOOST_CHECK_EQUAL(ct.at(1), "one");
}

BOOST_AUTO_TEST_CASE(at_throws_on_missing_key)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  BOOST_CHECK_THROW(t.at(2), std::out_of_range);
  const auto& ct = t;
  BOOST_CHECK_THROW(ct.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(hasKey_returns_true_for_existing_key)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  BOOST_CHECK(t.hasKey(1));
}

BOOST_AUTO_TEST_CASE(hasKey_returns_false_for_missing_key)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  BOOST_CHECK(!t.hasKey(2));
}

BOOST_AUTO_TEST_CASE(drop_removes_leaf_node)
{
  BSTree< int, std::string > t;
  t.push(2, "two");
  t.push(1, "one");
  t.push(3, "three");
  std::string val = t.drop(1);
  BOOST_CHECK_EQUAL(val, "one");
  BOOST_CHECK_EQUAL(t.size(), 2);
  BOOST_CHECK_THROW(t.at(1), std::out_of_range);
  BOOST_CHECK_EQUAL(t.at(2), "two");
  BOOST_CHECK_EQUAL(t.at(3), "three");
}

BOOST_AUTO_TEST_CASE(drop_removes_node_with_one_child)
{
  BSTree< int, std::string > t;
  t.push(2, "two");
  t.push(1, "one");
  t.push(3, "three");
  t.push(4, "four");
  std::string val = t.drop(3);
  BOOST_CHECK_EQUAL(val, "three");
  BOOST_CHECK_EQUAL(t.size(), 3);
  BOOST_CHECK_THROW(t.at(3), std::out_of_range);
  BOOST_CHECK_EQUAL(t.at(4), "four");
}

BOOST_AUTO_TEST_CASE(drop_removes_node_with_two_children)
{
  BSTree< int, std::string > t;
  t.push(10, "ten");
  t.push(5, "five");
  t.push(15, "fifteen");
  t.push(12, "twelve");
  t.push(18, "eighteen");
  t.drop(10);
  BOOST_CHECK_EQUAL(t.size(), 4);
  BOOST_CHECK_EQUAL(t.at(12), "twelve");
}

BOOST_AUTO_TEST_CASE(drop_removes_root_when_only_node)
{
  BSTree< int, std::string > t;
  t.push(42, "answer");
  std::string val = t.drop(42);
  BOOST_CHECK_EQUAL(val, "answer");
  BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_CASE(drop_throws_on_missing_key)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  BOOST_CHECK_THROW(t.drop(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(iterator_traverses_in_order)
{
  BSTree< int, std::string > t;
  t.push(3, "c");
  t.push(1, "a");
  t.push(2, "b");
  t.push(5, "e");
  t.push(4, "d");

  int expected[] = {1, 2, 3, 4, 5};
  int i = 0;
  for (auto it = t.begin(); it != t.end(); ++it, ++i)
  {
    BOOST_CHECK_EQUAL((*it).first, expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(iterator_moves_backward)
{
  BSTree< int, std::string > t;
  t.push(2, "b");
  t.push(1, "a");
  t.push(3, "c");

  auto it = t.begin();
  ++it;
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 3);
  --it;
  BOOST_CHECK_EQUAL((*it).first, 2);
  --it;
  BOOST_CHECK_EQUAL((*it).first, 1);
}

BOOST_AUTO_TEST_CASE(iterator_post_increment)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  t.push(2, "two");

  auto it = t.begin();
  auto it2 = it++;
  BOOST_CHECK_EQUAL((*it2).first, 1);
  BOOST_CHECK_EQUAL((*it).first, 2);
}

BOOST_AUTO_TEST_CASE(iterator_post_decrement)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  t.push(2, "two");

  auto it = t.begin();
  ++it;
  auto it2 = it--;
  BOOST_CHECK_EQUAL((*it2).first, 2);
  BOOST_CHECK_EQUAL((*it).first, 1);
}

BOOST_AUTO_TEST_CASE(height_of_empty_tree_is_zero)
{
  BSTree< int, std::string > t;
  BOOST_CHECK_EQUAL(t.height(), 0);
}

BOOST_AUTO_TEST_CASE(height_of_single_node_is_one)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  BOOST_CHECK_EQUAL(t.height(), 1);
}

BOOST_AUTO_TEST_CASE(height_of_chain_is_length)
{
  BSTree< int, std::string > t;
  t.push(1, "a");
  t.push(2, "b");
  t.push(3, "c");
  t.push(4, "d");
  BOOST_CHECK_EQUAL(t.height(), 4);
}

BOOST_AUTO_TEST_CASE(height_of_balanced_tree_is_log)
{
  BSTree< int, std::string > t;
  t.push(2, "b");
  t.push(1, "a");
  t.push(3, "c");
  BOOST_CHECK_EQUAL(t.height(), 2);
}

BOOST_AUTO_TEST_CASE(height_of_subtree)
{
  BSTree< int, std::string > t;
  t.push(5, "five");
  t.push(3, "three");
  t.push(7, "seven");
  t.push(2, "two");
  t.push(4, "four");

  auto it = t.begin();
  ++it;
  ++it;
  BOOST_CHECK_EQUAL(t.height(it), 1);
}

BOOST_AUTO_TEST_CASE(rotate_left_simple)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  t.push(2, "two");

  auto it = t.begin();
  ++it;
  auto newIt = t.rotateLeft(it);
  BOOST_CHECK_EQUAL((*newIt).first, 2);
  BOOST_CHECK_EQUAL(t.at(1), "one");
  BOOST_CHECK_EQUAL(t.at(2), "two");
}

BOOST_AUTO_TEST_CASE(rotate_right_simple)
{
  BSTree< int, std::string > t;
  t.push(2, "two");
  t.push(1, "one");

  auto it = t.begin();
  ++it;
  auto newIt = t.rotateRight(it);
  BOOST_CHECK_EQUAL((*newIt).first, 1);
}

BOOST_AUTO_TEST_CASE(rotate_left_preserves_order)
{
  BSTree< int, std::string > t;
  t.push(10, "ten");
  t.push(5, "five");
  t.push(15, "fifteen");
  t.push(12, "twelve");
  t.push(20, "twenty");

  auto it = t.begin();
  ++it;
  ++it;
  ++it;
  t.rotateLeft(it);

  int expected[] = {5, 10, 12, 15, 20};
  int i = 0;
  for (auto cit = t.begin(); cit != t.end(); ++cit, ++i)
  {
    BOOST_CHECK_EQUAL((*cit).first, expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(rotate_right_preserves_order)
{
  BSTree< int, std::string > t;
  t.push(10, "ten");
  t.push(5, "five");
  t.push(15, "fifteen");
  t.push(3, "three");
  t.push(7, "seven");

  auto it = t.begin();
  ++it;
  t.rotateRight(it);

  int expected[] = {3, 5, 7, 10, 15};
  int i = 0;
  for (auto cit = t.begin(); cit != t.end(); ++cit, ++i)
  {
    BOOST_CHECK_EQUAL((*cit).first, expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
  BSTree< int, std::string > t;
  t.push(30, "thirty");
  t.push(20, "twenty");
  t.push(40, "forty");
  t.push(10, "ten");
  t.push(25, "twenty five");
  t.push(35, "thirty five");
  t.push(50, "fifty");
  t.push(22, "twenty two");
  t.push(28, "twenty eight");

  auto it = t.begin();
  for (int i = 0; i < 6; ++i) ++it;
  t.rotateLargeLeft(it);

  int expected[] = {10, 20, 22, 25, 28, 30, 35, 40, 50};
  int i = 0;
  for (auto cit = t.begin(); cit != t.end(); ++cit, ++i)
  {
    BOOST_CHECK_EQUAL((*cit).first, expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
  BSTree< int, std::string > t;
  t.push(10, "ten");
  t.push(5, "five");
  t.push(20, "twenty");
  t.push(3, "three");
  t.push(8, "eight");
  t.push(15, "fifteen");
  t.push(25, "twenty five");
  t.push(6, "six");
  t.push(9, "nine");

  auto it = t.begin();
  ++it;
  ++it;
  t.rotateLargeRight(it);

  int expected[] = {3, 5, 6, 8, 9, 10, 15, 20, 25};
  int i = 0;
  for (auto cit = t.begin(); cit != t.end(); ++cit, ++i)
  {
    BOOST_CHECK_EQUAL((*cit).first, expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(rotate_on_fake_returns_same)
{
  BSTree< int, std::string > t;
  auto it = t.begin();
  auto newIt = t.rotateLeft(it);
  BOOST_CHECK(it == newIt);
}

BOOST_AUTO_TEST_CASE(rotate_on_leaf_returns_same)
{
  BSTree< int, std::string > t;
  t.push(2, "two");
  t.push(1, "one");

  auto it = t.begin();
  auto newIt = t.rotateLeft(it);
  BOOST_CHECK(it == newIt);
}

BOOST_AUTO_TEST_CASE(clear_removes_all_elements)
{
  BSTree< int, std::string > t;
  t.push(1, "one");
  t.push(2, "two");
  t.clear();
  BOOST_CHECK(t.empty());
  BOOST_CHECK_EQUAL(t.size(), 0);
  BOOST_CHECK_THROW(t.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(swap_exchanges_contents)
{
  BSTree< int, std::string > t1, t2;
  t1.push(1, "one");
  t2.push(2, "two");
  t2.push(3, "three");

  t1.swap(t2);
  BOOST_CHECK_EQUAL(t1.size(), 2);
  BOOST_CHECK_EQUAL(t2.size(), 1);
  BOOST_CHECK_EQUAL(t1.at(2), "two");
  BOOST_CHECK_EQUAL(t2.at(1), "one");
}

BOOST_AUTO_TEST_CASE(stress_large_tree)
{
  BSTree< int, std::string > t;
  for (int i = 0; i < 500; ++i)
  {
    t.push(i, std::to_string(i));
  }
  BOOST_CHECK_EQUAL(t.size(), 500);

  for (int i = 0; i < 500; ++i)
  {
    BOOST_CHECK_EQUAL(t.at(i), std::to_string(i));
  }

  for (int i = 0; i < 500; ++i)
  {
    t.drop(i);
  }
  BOOST_CHECK(t.empty());
}

BOOST_AUTO_TEST_SUITE_END()
