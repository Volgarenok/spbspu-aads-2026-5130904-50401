#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "bstree.hpp"

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.height() == 0);
  BOOST_TEST(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(push_get_contains_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(2, "two");
  tree.push(7, "seven");

  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(2));
  BOOST_TEST(tree.contains(7));
  BOOST_TEST(!tree.contains(10));
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(2) == "two");
  BOOST_TEST(tree.get(7) == "seven");
}

BOOST_AUTO_TEST_CASE(push_updates_value_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(1, "one");
  tree.push(1, "new");
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(1) == "new");
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  zhuravleva::BSTree< int, std::string > first;
  first.push(1, "one");
  first.push(2, "two");
  zhuravleva::BSTree< int, std::string > second(first);
  first.drop(1);
  BOOST_TEST(first.size() == 1);
  BOOST_TEST(second.size() == 2);
  BOOST_TEST(!first.contains(1));
  BOOST_TEST(second.contains(1));
  BOOST_TEST(second.get(1) == "one");
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  zhuravleva::BSTree< int, std::string > first;
  first.push(1, "one");
  first.push(2, "two");
  zhuravleva::BSTree< int, std::string > second(std::move(first));
  BOOST_TEST(second.size() == 2);
  BOOST_TEST(second.get(1) == "one");
  BOOST_TEST(second.get(2) == "two");
  BOOST_TEST(first.size() == 0);
  BOOST_TEST(first.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  zhuravleva::BSTree< int, std::string > first;
  first.push(1, "one");
  first.push(2, "two");
  zhuravleva::BSTree< int, std::string > second;
  second.push(10, "ten");
  second = first;
  first.drop(1);
  BOOST_TEST(first.size() == 1);
  BOOST_TEST(second.size() == 2);
  BOOST_TEST(second.contains(1));
  BOOST_TEST(second.get(1) == "one");
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  zhuravleva::BSTree< int, std::string > first;
  first.push(1, "one");
  first.push(2, "two");
  zhuravleva::BSTree< int, std::string > second;
  second.push(10, "ten");
  second = std::move(first);

  BOOST_TEST(second.size() == 2);
  BOOST_TEST(second.get(1) == "one");
  BOOST_TEST(second.get(2) == "two");
  BOOST_TEST(first.size() == 0);
}

BOOST_AUTO_TEST_CASE(iterator_order_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(2, "two");
  tree.push(9, "nine");
  tree.push(13, "thirteen");
  int keys[] = {2, 5, 9, 10, 13, 15};
  std::string values[] = {"two", "five", "nine", "ten", "thirteen", "fifteen"};

  size_t i = 0;
  for (zhuravleva::BSTree< int, std::string >::constIterator it = tree.cbegin();
       it != tree.cend();
       ++it)
  {
    BOOST_TEST(it->first == keys[i]);
    BOOST_TEST(it->second == values[i]);
    BOOST_TEST((*it).first == keys[i]);
    BOOST_TEST((*it).second == values[i]);
    ++i;
  }

  BOOST_TEST(i == 6);
}

BOOST_AUTO_TEST_CASE(height_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(2, "two");
  tree.push(9, "nine");
  tree.push(13, "thirteen");
  BOOST_TEST(tree.height() == 3);
  int heights[] = {1, 2, 1, 3, 1, 2};

  size_t i = 0;
  for (zhuravleva::BSTree< int, std::string >::constIterator it = tree.cbegin();
       it != tree.cend();
       ++it)
  {
    BOOST_TEST(tree.height(it) == heights[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(drop_leaf_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(2, "two");
  tree.push(7, "seven");
  tree.drop(2);
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(!tree.contains(2));
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(7));
}

BOOST_AUTO_TEST_CASE(drop_node_with_one_child_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(2, "two");
  tree.push(1, "one");
  tree.drop(2);

  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(!tree.contains(2));
  BOOST_TEST(tree.contains(1));
  BOOST_TEST(tree.get(1) == "one");
}

BOOST_AUTO_TEST_CASE(drop_node_with_two_children_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(2, "two");
  tree.push(7, "seven");
  tree.push(6, "six");
  tree.push(8, "eight");
  tree.drop(7);

  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(!tree.contains(7));
  BOOST_TEST(tree.contains(6));
  BOOST_TEST(tree.contains(8));
}

BOOST_AUTO_TEST_CASE(drop_missing_key_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(1, "one");
  BOOST_CHECK_THROW(tree.drop(10), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(left_rotate_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(15, "fifteen");
  tree.push(6, "six");
  tree.push(13, "thirteen");
  tree.push(18, "eighteen");
  tree.rotateLeft(tree.find(15));

  BOOST_TEST(tree.height() == 3);
  int keys[] = {6, 10, 13, 15, 18};

  size_t i = 0;
  for (zhuravleva::BSTree< int, std::string >::constIterator it = tree.cbegin();
       it != tree.cend();
       ++it)
  {
    BOOST_TEST(it->first == keys[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(right_rotate_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(15, "fifteen");
  tree.push(10, "ten");
  tree.push(18, "eighteen");
  tree.push(6, "six");
  tree.push(13, "thirteen");
  tree.rotateRight(tree.find(10));
  BOOST_TEST(tree.height() == 3);
  int keys[] = {6, 10, 13, 15, 18};

  size_t i = 0;
  for (zhuravleva::BSTree< int, std::string >::constIterator it = tree.cbegin();
       it != tree.cend();
       ++it)
  {
    BOOST_TEST(it->first == keys[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(large_left_rotate_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(15, "fifteen");
  tree.push(6, "six");
  tree.push(12, "twelve");
  tree.push(18, "eighteen");
  tree.push(11, "eleven");
  tree.push(13, "thirteen");
  tree.rotateLargeLeft(tree.find(12));
  BOOST_TEST(tree.height() == 3);
  int keys[] = {6, 10, 11, 12, 13, 15, 18};

  size_t i = 0;
  for (zhuravleva::BSTree< int, std::string >::constIterator it = tree.cbegin();
       it != tree.cend();
       ++it)
  {
    BOOST_TEST(it->first == keys[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(large_right_rotate_test)
{
  zhuravleva::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(3, "three");
  tree.push(15, "fifteen");
  tree.push(1, "one");
  tree.push(7, "seven");
  tree.push(5, "five");
  tree.push(8, "eight");
  tree.rotateLargeRight(tree.find(7));
  BOOST_TEST(tree.height() == 3);
  int keys[] = {1, 3, 5, 7, 8, 10, 15};

  size_t i = 0;
  for (zhuravleva::BSTree< int, std::string >::constIterator it = tree.cbegin();
       it != tree.cend();
       ++it)
  {
    BOOST_TEST(it->first == keys[i]);
    ++i;
  }
}
