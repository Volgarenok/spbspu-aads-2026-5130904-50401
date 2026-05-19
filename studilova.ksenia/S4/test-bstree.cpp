#define BOOST_TEST_MODULE BSTreeTests
#include <boost/test/included/unit_test.hpp>

#include "bstree.hpp"

#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(push_get_update)
{
  studilova::BSTree< int, std::string > tree;

  BOOST_TEST(tree.empty());

  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.get(10) == "ten");
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(15) == "fifteen");

  tree.push(10, "TEN");
  BOOST_TEST(tree.get(10) == "TEN");
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  studilova::BSTree< int, std::string > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  studilova::BSTree< int, std::string > copy(tree);

  BOOST_TEST(copy.get(10) == "ten");
  BOOST_TEST(copy.get(5) == "five");
  BOOST_TEST(copy.get(15) == "fifteen");

  copy.push(10, "changed");

  BOOST_TEST(copy.get(10) == "changed");
  BOOST_TEST(tree.get(10) == "ten");
}

BOOST_AUTO_TEST_CASE(assignment_operator)
{
  studilova::BSTree< int, std::string > first;
  first.push(1, "one");
  first.push(2, "two");

  studilova::BSTree< int, std::string > second;
  second.push(10, "ten");

  second = first;

  BOOST_TEST(second.get(1) == "one");
  BOOST_TEST(second.get(2) == "two");
  BOOST_CHECK_THROW(second.get(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(iterator_inorder)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(10, "10");
  tree.push(5, "5");
  tree.push(15, "15");
  tree.push(3, "3");
  tree.push(7, "7");

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();

  BOOST_TEST((*it).first == 3);
  ++it;
  BOOST_TEST((*it).first == 5);
  ++it;
  BOOST_TEST((*it).first == 7);
  ++it;
  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 15);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(reverse_iteration)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(10, "10");
  tree.push(5, "5");
  tree.push(15, "15");

  studilova::BSTree< int, std::string >::It it = tree.end();

  --it;
  BOOST_TEST((*it).first == 15);
  --it;
  BOOST_TEST((*it).first == 10);
  --it;
  BOOST_TEST((*it).first == 5);
  BOOST_CHECK(it == tree.begin());
}

BOOST_AUTO_TEST_CASE(iterator_change_value)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(1, "one");

  studilova::BSTree< int, std::string >::It it = tree.begin();
  (*it).second = "ONE";

  BOOST_TEST(tree.get(1) == "ONE");
}

BOOST_AUTO_TEST_CASE(drop_leaf)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  BOOST_TEST(tree.drop(5) == "five");
  BOOST_CHECK_THROW(tree.get(5), std::out_of_range);

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();
  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 15);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(drop_one_child)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(10, "ten");
  tree.push(15, "fifteen");
  tree.push(20, "twenty");

  BOOST_TEST(tree.drop(15) == "fifteen");

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();
  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 20);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(drop_two_children)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(12, "twelve");
  tree.push(20, "twenty");

  BOOST_TEST(tree.drop(10) == "ten");

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();

  BOOST_TEST((*it).first == 5);
  ++it;
  BOOST_TEST((*it).first == 12);
  ++it;
  BOOST_TEST((*it).first == 15);
  ++it;
  BOOST_TEST((*it).first == 20);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(height_test)
{
  studilova::BSTree< int, std::string > tree;

  BOOST_TEST(tree.height() == 0);

  tree.push(10, "10");
  BOOST_TEST(tree.height() == 1);

  tree.push(20, "20");
  tree.push(30, "30");

  BOOST_TEST(tree.height() == 3);
}

BOOST_AUTO_TEST_CASE(rotate_left)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(10, "10");
  tree.push(20, "20");
  tree.push(15, "15");
  tree.push(30, "30");

  studilova::BSTree< int, std::string >::CIt root = tree.cbegin();
  BOOST_TEST((*root).first == 10);

  studilova::BSTree< int, std::string >::CIt new_root = tree.rotateLeft(root);
  BOOST_TEST((*new_root).first == 20);

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();

  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 15);
  ++it;
  BOOST_TEST((*it).first == 20);
  ++it;
  BOOST_TEST((*it).first == 30);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(rotate_right)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(20, "20");
  tree.push(10, "10");
  tree.push(5, "5");
  tree.push(15, "15");

  studilova::BSTree< int, std::string >::CIt root = tree.cbegin();
  ++root;
  ++root;
  BOOST_TEST((*root).first == 20);

  studilova::BSTree< int, std::string >::CIt new_root = tree.rotateRight(root);
  BOOST_TEST((*new_root).first == 10);

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();

  BOOST_TEST((*it).first == 5);
  ++it;
  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 15);
  ++it;
  BOOST_TEST((*it).first == 20);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(10, "10");
  tree.push(30, "30");
  tree.push(20, "20");

  studilova::BSTree< int, std::string >::CIt root = tree.cbegin();
  BOOST_TEST((*root).first == 10);

  studilova::BSTree< int, std::string >::CIt new_root = tree.rotateLargeLeft(root);
  BOOST_TEST((*new_root).first == 20);

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();

  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 20);
  ++it;
  BOOST_TEST((*it).first == 30);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
  studilova::BSTree< int, std::string > tree;

  tree.push(30, "30");
  tree.push(10, "10");
  tree.push(20, "20");

  studilova::BSTree< int, std::string >::CIt root = tree.cbegin();
  ++root;
  ++root;
  BOOST_TEST((*root).first == 30);

  studilova::BSTree< int, std::string >::CIt new_root = tree.rotateLargeRight(root);
  BOOST_TEST((*new_root).first == 20);

  studilova::BSTree< int, std::string >::CIt it = tree.cbegin();

  BOOST_TEST((*it).first == 10);
  ++it;
  BOOST_TEST((*it).first == 20);
  ++it;
  BOOST_TEST((*it).first == 30);
  ++it;
  BOOST_CHECK(it == tree.cend());
}

BOOST_AUTO_TEST_CASE(exceptions_test)
{
  studilova::BSTree< int, std::string > tree;

  BOOST_CHECK_THROW(tree.get(1), std::out_of_range);
  BOOST_CHECK_THROW(tree.drop(1), std::out_of_range);

  tree.push(10, "10");

  BOOST_CHECK_THROW(tree.rotateLeft(tree.cbegin()), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateRight(tree.cbegin()), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
