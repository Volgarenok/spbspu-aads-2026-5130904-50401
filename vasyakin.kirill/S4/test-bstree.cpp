#include <boost/test/unit_test.hpp>
#include <vector>
#include <string>
#include "bstree.hpp"

std::vector< int > collect_keys(const vasyakin::BSTree< int, std::string >& tree)
{
  std::vector< int > keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back((*it).first);
  }
  return keys;
}

BOOST_AUTO_TEST_SUITE(BstTests)

BOOST_AUTO_TEST_CASE(test_construction_empty_height)
{
  vasyakin::BSTree< int, std::string > tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.height(), 0);
  BOOST_CHECK(tree.begin() == tree.end());

  tree.push(42, "answer");
  BOOST_CHECK(!tree.empty());
  BOOST_CHECK_EQUAL(tree.height(), 1);
  BOOST_CHECK_EQUAL(tree.get(42), "answer");
}

BOOST_AUTO_TEST_CASE(test_push_get_update)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  BOOST_CHECK_EQUAL(tree.get(10), "ten");
  BOOST_CHECK_EQUAL(tree.get(5), "five");
  BOOST_CHECK_EQUAL(tree.get(15), "fifteen");

  tree.push(10, "TEN_UPDATED");
  BOOST_CHECK_EQUAL(tree.get(10), "TEN_UPDATED");
  BOOST_CHECK_THROW(tree.get(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_drop_leaf)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  BOOST_CHECK_EQUAL(tree.drop(5), "five");
  BOOST_CHECK_THROW(tree.get(5), std::out_of_range);

  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 2);
  BOOST_CHECK_EQUAL(keys[0], 10);
  BOOST_CHECK_EQUAL(keys[1], 15);
}

BOOST_AUTO_TEST_CASE(test_drop_one_child)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(15, "fifteen");
  tree.push(20, "twenty");

  BOOST_CHECK_EQUAL(tree.drop(15), "fifteen");

  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 2);
  BOOST_CHECK_EQUAL(keys[0], 10);
  BOOST_CHECK_EQUAL(keys[1], 20);
}

BOOST_AUTO_TEST_CASE(test_drop_two_children)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(20, "twenty");

  BOOST_CHECK_EQUAL(tree.drop(10), "ten");

  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 3);
  BOOST_CHECK_EQUAL(keys[0], 5);
  BOOST_CHECK_EQUAL(keys[1], 15);
  BOOST_CHECK_EQUAL(keys[2], 20);

  BOOST_CHECK_EQUAL(tree.get(15), "fifteen");
}

BOOST_AUTO_TEST_CASE(test_drop_exceptions)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_THROW(tree.drop(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_iterators_forward)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(30, "30");
  tree.push(10, "10");
  tree.push(20, "20");
  tree.push(40, "40");
  tree.push(5, "5");

  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 5);
  BOOST_CHECK_EQUAL(keys[0], 5);
  BOOST_CHECK_EQUAL(keys[1], 10);
  BOOST_CHECK_EQUAL(keys[2], 20);
  BOOST_CHECK_EQUAL(keys[3], 30);
  BOOST_CHECK_EQUAL(keys[4], 40);
}

BOOST_AUTO_TEST_CASE(test_iterators_backward)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(1, "a");
  tree.push(2, "b");
  tree.push(3, "c");

  auto it = tree.begin();
  while (true)
  {
    auto next = it;
    ++next;
    if (next == tree.end())
    {
      break;
    }
    it = next;
  }

  std::vector< int > bwd;
  bwd.push_back((*it).first);
  --it;
  bwd.push_back((*it).first);
  --it;
  bwd.push_back((*it).first);

  BOOST_REQUIRE_EQUAL(bwd.size(), 3);
  BOOST_CHECK_EQUAL(bwd[0], 3);
  BOOST_CHECK_EQUAL(bwd[1], 2);
  BOOST_CHECK_EQUAL(bwd[2], 1);
}

BOOST_AUTO_TEST_CASE(test_iterators_comparison)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  auto it1 = tree.begin();
  auto it2 = tree.begin();
  auto it_end = tree.end();

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it_end);

  ++it1;
  BOOST_CHECK(it1 != it2);
}

BOOST_AUTO_TEST_CASE(test_const_iterators)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(2, "two");

  const auto& ct = tree;
  auto cit = ct.cbegin();

  BOOST_CHECK_EQUAL((*cit).first, 1);
  ++cit;
  BOOST_CHECK_EQUAL((*cit).first, 2);
  BOOST_CHECK(cit != ct.cend());
}

BOOST_AUTO_TEST_CASE(test_iterator_conversion)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(42, "answer");

  vasyakin::BSTree< int, std::string >::iterator it = tree.begin();
  vasyakin::BSTree< int, std::string >::const_iterator cit = it;

  BOOST_CHECK_EQUAL((*cit).first, 42);
}

BOOST_AUTO_TEST_CASE(test_height_full)
{
  vasyakin::BSTree< int, std::string > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);

  tree.push(1, "1");
  tree.push(2, "2");
  tree.push(3, "3");
  BOOST_CHECK_EQUAL(tree.height(), 3);

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(tree.height(it), 3);
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 2);
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 1);

  tree.drop(1);
  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "10");
  tree.push(15, "15");
  tree.push(8, "8");
  tree.push(13, "13");
  tree.push(17, "17");
  tree.push(20, "20");

  BOOST_CHECK_EQUAL(tree.height(), 4);

  auto it_15 = tree.begin();
  ++it_15;
  ++it_15;
  ++it_15;

  BOOST_CHECK_EQUAL((*it_15).first, 15);

  tree.rotateLeft(it_15);
  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 6);
  BOOST_CHECK_EQUAL(keys[0], 8);
  BOOST_CHECK_EQUAL(keys[1], 10);
  BOOST_CHECK_EQUAL(keys[2], 13);
  BOOST_CHECK_EQUAL(keys[3], 15);
  BOOST_CHECK_EQUAL(keys[4], 17);
  BOOST_CHECK_EQUAL(keys[5], 20);

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "10");
  tree.push(8, "8");
  tree.push(15, "15");
  tree.push(9, "9");
  tree.push(7, "7");
  tree.push(6, "6");

  BOOST_CHECK_EQUAL(tree.height(), 4);

  auto it_8 = tree.begin();
  ++it_8;
  ++it_8;

  BOOST_CHECK_EQUAL((*it_8).first, 8);

  tree.rotateRight(it_8);
  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 6);
  BOOST_CHECK_EQUAL(keys[0], 6);
  BOOST_CHECK_EQUAL(keys[1], 7);
  BOOST_CHECK_EQUAL(keys[2], 8);
  BOOST_CHECK_EQUAL(keys[3], 9);
  BOOST_CHECK_EQUAL(keys[4], 10);
  BOOST_CHECK_EQUAL(keys[5], 15);

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "10");
  tree.push(7, "7");
  tree.push(15, "15");
  tree.push(12, "12");
  tree.push(11, "11");
  tree.push(13, "13");
  tree.push(16, "16");

  BOOST_CHECK_EQUAL(tree.height(), 4);

  auto it_12 = tree.begin();
  ++it_12;
  ++it_12;
  ++it_12;

  BOOST_CHECK_EQUAL((*it_12).first, 12);

  tree.rotateLargeLeft(it_12);
  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 7);
  BOOST_CHECK_EQUAL(keys[0], 7);
  BOOST_CHECK_EQUAL(keys[1], 10);
  BOOST_CHECK_EQUAL(keys[2], 11);
  BOOST_CHECK_EQUAL(keys[3], 12);
  BOOST_CHECK_EQUAL(keys[4], 13);
  BOOST_CHECK_EQUAL(keys[5], 15);
  BOOST_CHECK_EQUAL(keys[6], 16);

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "10");
  tree.push(6, "6");
  tree.push(5, "5");
  tree.push(8, "8");
  tree.push(7, "7");
  tree.push(9, "9");
  tree.push(15, "15");

  BOOST_CHECK_EQUAL(tree.height(), 4);

  auto it_8 = tree.begin();
  ++it_8;
  ++it_8;
  ++it_8;

  BOOST_CHECK_EQUAL((*it_8).first, 8);

  tree.rotateLargeRight(it_8);
  auto keys = collect_keys(tree);
  BOOST_REQUIRE_EQUAL(keys.size(), 7);
  BOOST_CHECK_EQUAL(keys[0], 5);
  BOOST_CHECK_EQUAL(keys[1], 6);
  BOOST_CHECK_EQUAL(keys[2], 7);
  BOOST_CHECK_EQUAL(keys[3], 8);
  BOOST_CHECK_EQUAL(keys[4], 9);
  BOOST_CHECK_EQUAL(keys[5], 10);
  BOOST_CHECK_EQUAL(keys[6], 15);

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(test_rotation_exceptions)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "10");
  tree.push(20, "20");

  BOOST_CHECK_THROW(tree.rotateLeft(tree.end()), std::invalid_argument);
  BOOST_CHECK_THROW(tree.rotateRight(tree.begin()), std::invalid_argument);
  BOOST_CHECK_THROW(tree.rotateLargeLeft(tree.begin()), std::invalid_argument);
  BOOST_CHECK_THROW(tree.rotateLargeRight(tree.begin()), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  vasyakin::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  tree1.push(3, "three");

  vasyakin::BSTree< int, std::string > tree2(tree1);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");

  tree2.drop(1);
  BOOST_CHECK_EQUAL(tree1.get(1), "one");
  BOOST_CHECK_THROW(tree2.get(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  vasyakin::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  vasyakin::BSTree< int, std::string > tree2;
  tree2 = tree1;

  BOOST_CHECK_EQUAL(tree2.get(2), "two");
  BOOST_CHECK_EQUAL(collect_keys(tree2).size(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  vasyakin::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  vasyakin::BSTree< int, std::string > tree2(std::move(tree1));

  BOOST_CHECK(tree1.empty());
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
  BOOST_CHECK_EQUAL(tree2.get(2), "two");
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  vasyakin::BSTree< int, std::string > tree1;
  tree1.push(1, "one");

  vasyakin::BSTree< int, std::string > tree2;
  tree2 = std::move(tree1);

  BOOST_CHECK(tree1.empty());
  BOOST_CHECK_EQUAL(tree2.get(1), "one");
}

BOOST_AUTO_TEST_CASE(test_edge_duplicates)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(5, "A");
  tree.push(5, "B");
  tree.push(5, "C");

  BOOST_CHECK_EQUAL(tree.get(5), "C");
  BOOST_CHECK_EQUAL(collect_keys(tree).size(), 1);
}

BOOST_AUTO_TEST_CASE(test_edge_empty_operations)
{
  vasyakin::BSTree< int, std::string > tree;

  BOOST_CHECK_THROW(tree.drop(1), std::out_of_range);
  BOOST_CHECK_THROW(tree.get(1), std::out_of_range);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(test_stress_large_tree)
{
  vasyakin::BSTree< int, std::string > tree;

  for (int i = 0; i < 1000; ++i)
  {
    tree.push(i, std::to_string(i));
  }

  BOOST_CHECK_EQUAL(tree.height(), 1000);

  size_t count = 0;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 1000);

  for (int i = 0; i < 1000; ++i)
  {
    tree.drop(i);
  }

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
