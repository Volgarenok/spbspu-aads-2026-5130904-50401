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

BOOST_AUTO_TEST_CASE(insert_basic_and_duplicates)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(5, "five");
  tree.insert(20, "twenty");

  BOOST_CHECK_EQUAL(tree.size(), 3u);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(!tree.has(15));

  tree.insert(10, "six_seven");

  BOOST_CHECK_EQUAL(tree.size(), 3u);
  BOOST_CHECK_EQUAL(tree.at(10), "six_seven");
}

BOOST_AUTO_TEST_CASE(inorder_traversal_random_insertion)
{
  vasyakin::AVLTree< int, int > tree;

  std::vector< int > keys = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
  for (int k : keys)
  {
    tree.insert(k, k);
  }

  std::vector< int > sorted;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    sorted.push_back((*it).first);
  }

  std::vector< int > expected = {10, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80};
  BOOST_CHECK_EQUAL_COLLECTIONS(sorted.begin(), sorted.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(remove_leaf_and_one_child)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "root");
  tree.insert(5, "left");
  tree.insert(15, "right");
  tree.insert(7, "right_leaf");
  tree.insert(3, "left_leaf");

  BOOST_CHECK(tree.remove(3));
  BOOST_CHECK_EQUAL(tree.size(), 4u);
  BOOST_CHECK(!tree.has(3));

  BOOST_CHECK(tree.remove(5));
  BOOST_CHECK_EQUAL(tree.size(), 3u);
  BOOST_CHECK(!tree.has(5));
  BOOST_CHECK(tree.has(7));
}

BOOST_AUTO_TEST_CASE(remove_two_children)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "root");
  tree.insert(5, "left");
  tree.insert(15, "right");
  tree.insert(7, "right_leaf");
  tree.insert(3, "left_leaf");

  BOOST_CHECK(tree.remove(5));
  BOOST_CHECK_EQUAL(tree.size(), 4u);
  BOOST_CHECK(!tree.has(5));
  BOOST_CHECK(tree.has(3));
  BOOST_CHECK(tree.has(7));
}

BOOST_AUTO_TEST_CASE(remove_nonexistent_returns_false)
{
  vasyakin::AVLTree< int, int > tree;
  tree.insert(52, 52);

  BOOST_CHECK(!tree.remove(67));
  BOOST_CHECK_EQUAL(tree.size(), 1u);
}

BOOST_AUTO_TEST_CASE(empty_tree_edge_cases)
{
  vasyakin::AVLTree< int, std::string > empty;

  BOOST_CHECK(empty.empty());
  BOOST_CHECK_EQUAL(empty.size(), 0u);
  BOOST_CHECK_EQUAL(empty.begin(), empty.end());
  BOOST_CHECK(empty.cbegin() == empty.cend());

  BOOST_CHECK_THROW(empty.at(100), std::out_of_range);
  BOOST_CHECK(!empty.remove(42));
  BOOST_CHECK_NO_THROW(empty.clear());
}

BOOST_AUTO_TEST_CASE(stress_removal)
{
  vasyakin::AVLTree< int, int > tree;

  for (int i = 0; i < 100; ++i)
  {
    tree.insert(i, i);
  }

  BOOST_CHECK_EQUAL(tree.size(), 100u);

  for (int i = 99; i >= 0; --i)
  {
    BOOST_CHECK(tree.remove(i));
    BOOST_CHECK_EQUAL(tree.size(), static_cast< size_t >(i));
  }

  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(find_method_and_const_correctness)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(30, "thirty");

  auto it_found = tree.find(10);
  BOOST_REQUIRE(it_found != tree.end());
  BOOST_CHECK_EQUAL((*it_found).first, 10);
  BOOST_CHECK_EQUAL((*it_found).second, "ten");

  auto it_not_found = tree.find(67);
  BOOST_CHECK(it_not_found == tree.end());

  const vasyakin::AVLTree< int, std::string >& const_ref = tree;

  auto const_it_found = const_ref.find(10);
  BOOST_REQUIRE(const_it_found != const_ref.cend());
  BOOST_CHECK_EQUAL((*const_it_found).first, 10);
  BOOST_CHECK_EQUAL((*const_it_found).second, "ten");

  auto const_it_not_found = const_ref.find(50);
  BOOST_CHECK(const_it_not_found == const_ref.cend());
}

BOOST_AUTO_TEST_CASE(at_method_throws_on_invalid_key)
{
  vasyakin::AVLTree< int, std::string > tree;
  tree.insert(10, "ten");

  BOOST_CHECK_NO_THROW(tree.at(10));
  BOOST_CHECK_THROW(tree.at(999), std::out_of_range);

  const vasyakin::AVLTree< int, std::string > const_tree = tree;

  BOOST_CHECK_NO_THROW(const_tree.at(10));
  BOOST_CHECK_THROW(const_tree.at(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(iterator_increment_decrement)
{
  vasyakin::AVLTree< int, int > tree;

  tree.insert(10, 10);
  tree.insert(20, 20);
  tree.insert(30, 30);

  auto it = tree.begin();
  BOOST_CHECK_EQUAL((*it).first, 10);

  ++it;
  BOOST_CHECK_EQUAL((*it).first, 20);

  ++it;
  BOOST_CHECK_EQUAL((*it).first, 30);

  --it;
  BOOST_CHECK_EQUAL((*it).first, 20);

  --it;
  BOOST_CHECK_EQUAL((*it).first, 10);
}

BOOST_AUTO_TEST_CASE(move_only_types)
{
  vasyakin::AVLTree< int, std::unique_ptr< int > > tree;

  tree.insert(1, std::make_unique< int >(100));
  tree.insert(2, std::make_unique< int >(200));

  BOOST_CHECK_EQUAL(*tree.at(1), 100);
  BOOST_CHECK_EQUAL(*tree.at(2), 200);

  BOOST_CHECK(tree.remove(1));
  BOOST_CHECK_EQUAL(tree.size(), 1u);
  BOOST_CHECK_EQUAL(*tree.at(2), 200);
}

BOOST_AUTO_TEST_CASE(left_rotate)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(30, "thirty");
  tree.insert(40, "forty");

  BOOST_CHECK_EQUAL(tree.size(), 4u);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(20));
  BOOST_CHECK(tree.has(30));
  BOOST_CHECK(tree.has(40));

  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back((*it).first);
  }

  std::vector< int > expected = {10, 20, 30, 40};

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(right_rotate)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(9, "nine");
  tree.insert(8, "eight");
  tree.insert(7, "seven");

  BOOST_CHECK_EQUAL(tree.size(), 4u);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(9));
  BOOST_CHECK(tree.has(8));
  BOOST_CHECK(tree.has(7));

  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back((*it).first);
  }

  std::vector< int > expected = {7, 8, 9, 10};

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(left_large_rotate)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(8, "eight");
  tree.insert(9, "nine");

  BOOST_CHECK_EQUAL(tree.size(), 3u);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(8));
  BOOST_CHECK(tree.has(9));

  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back((*it).first);
  }

  std::vector< int > expected = {8, 9, 10};

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(right_large_rotate)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(15, "fifteen");

  BOOST_CHECK_EQUAL(tree.size(), 3u);
  BOOST_CHECK(tree.has(10));
  BOOST_CHECK(tree.has(20));
  BOOST_CHECK(tree.has(15));

  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back((*it).first);
  }

  std::vector< int > expected = {10, 15, 20};

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(rotations_maintain_balance_under_load)
{
  vasyakin::AVLTree< int, int > tree;

  for (int i = 0; i < 100; ++i)
  {
    tree.insert(i, i);
  }

  BOOST_CHECK_EQUAL(tree.size(), 100u);

  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back((*it).first);
  }

  std::vector< int > expected(100);
  for (int i = 0; i < 100; ++i)
  {
    expected[i] = i;
  }

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());

  for (int i = 0; i < 100; ++i)
  {
    BOOST_CHECK(tree.has(i));
  }
}

BOOST_AUTO_TEST_CASE(lower_bound)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(30, "thirty");
  tree.insert(40, "forty");
  tree.insert(50, "fifty");

  auto lb1 = tree.lower_bound(20);
  BOOST_REQUIRE(lb1 != tree.end());
  BOOST_CHECK_EQUAL((*lb1).first, 20);
  BOOST_CHECK_EQUAL((*lb1).second, "twenty");

  auto lb2 = tree.lower_bound(25);
  BOOST_REQUIRE(lb2 != tree.end());
  BOOST_CHECK_EQUAL((*lb2).first, 30);
  BOOST_CHECK_EQUAL((*lb2).second, "thirty");

  auto lb3 = tree.lower_bound(5);
  BOOST_REQUIRE(lb3 != tree.end());
  BOOST_CHECK_EQUAL((*lb3).first, 10);
  BOOST_CHECK_EQUAL((*lb3).second, "ten");

  auto lb4 = tree.lower_bound(50);
  BOOST_REQUIRE(lb4 != tree.end());
  BOOST_CHECK_EQUAL((*lb4).first, 50);
  BOOST_CHECK_EQUAL((*lb4).second, "fifty");

  BOOST_CHECK(tree.lower_bound(52) == tree.end());
}

BOOST_AUTO_TEST_CASE(upper_bound)
{
  vasyakin::AVLTree< int, std::string > tree;

  tree.insert(10, "ten");
  tree.insert(20, "twenty");
  tree.insert(30, "thirty");
  tree.insert(40, "forty");
  tree.insert(50, "fifty");

  auto ub1 = tree.upper_bound(10);
  BOOST_REQUIRE(ub1 != tree.end());
  BOOST_CHECK_EQUAL((*ub1).first, 20);
  BOOST_CHECK_EQUAL((*ub1).second, "twenty");

  auto ub2 = tree.upper_bound(15);
  BOOST_REQUIRE(ub2 != tree.end());
  BOOST_CHECK_EQUAL((*ub2).first, 20);
  BOOST_CHECK_EQUAL((*ub2).second, "twenty");

  auto ub3 = tree.upper_bound(5);
  BOOST_REQUIRE(ub3 != tree.end());
  BOOST_CHECK_EQUAL((*ub3).first, 10);
  BOOST_CHECK_EQUAL((*ub3).second, "ten");

  BOOST_CHECK(tree.upper_bound(50) == tree.end());
}

BOOST_AUTO_TEST_CASE(equal_range)
{
  vasyakin::AVLTree< int, int > tree;

  for (size_t i = 1; i <= 10; ++i)
  {
    tree.insert(i * 10, i);
  }

  for (size_t key = 0; key <= 110; key += 5)
  {
    auto range = tree.equal_range(key);
    auto lb = tree.lower_bound(key);
    auto ub = tree.upper_bound(key);

    BOOST_CHECK(range.first == lb);
    BOOST_CHECK(range.second == ub);
  }
}

BOOST_AUTO_TEST_CASE(bounds_empty_tree)
{
  vasyakin::AVLTree< int, int > tree;

  BOOST_CHECK(tree.lower_bound(42) == tree.end());
  BOOST_CHECK(tree.upper_bound(42) == tree.end());

  auto range = tree.equal_range(42);
  BOOST_CHECK(range.first == tree.end());
  BOOST_CHECK(range.second == tree.end());
}

BOOST_AUTO_TEST_CASE(bounds_single_element)
{
  vasyakin::AVLTree< int, int > tree;
  tree.insert(100, 1);

  auto lb = tree.lower_bound(100);
  BOOST_REQUIRE(lb != tree.end());
  BOOST_CHECK_EQUAL((*lb).first, 100);

  auto ub = tree.upper_bound(100);
  BOOST_CHECK(ub == tree.end());

  auto range = tree.equal_range(100);
  BOOST_REQUIRE(range.first != tree.end());
  BOOST_CHECK_EQUAL((*range.first).first, 100);
  BOOST_CHECK(range.second == tree.end());
}

BOOST_AUTO_TEST_SUITE_END()
