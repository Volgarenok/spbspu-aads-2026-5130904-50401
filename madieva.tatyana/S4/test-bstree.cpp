#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include "tree_iterator.hpp"

BOOST_AUTO_TEST_SUITE(BSTreeRotateTest)

BOOST_AUTO_TEST_CASE(key_value)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");
  tree.push(15, "c");

  madieva::TIter< int, std::string, std::less< int > > it = tree.begin();
  BOOST_CHECK_EQUAL((*it).first,10);
  BOOST_CHECK_EQUAL((*it).second, "a");
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 15);
  BOOST_CHECK_EQUAL((*it).second, "c");
  ++it;
  BOOST_CHECK_EQUAL((*it).first, 20);
  BOOST_CHECK_EQUAL((*it).second, "b");

}

BOOST_AUTO_TEST_CASE(repeat_key_push)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(10, "b");
  tree.push(15, "c");

  madieva::TIter< int, std::string, std::less< int > > it = tree.begin();
  size_t count = 0;
  for (; it != tree.end(); ++it) {
    count++;
  }
  BOOST_CHECK_EQUAL(count, 2);
  madieva::TIter< int, std::string, std::less< int > > it1 = tree.begin();
  BOOST_CHECK_EQUAL((*it1).first, 10);
  BOOST_CHECK_EQUAL((*it1).second, "a");
  ++it1;
  BOOST_CHECK_EQUAL((*it1).first, 15);
  BOOST_CHECK_EQUAL((*it1).second, "c");

}

BOOST_AUTO_TEST_CASE(rotate_left_simple)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");
  tree.push(15, "c");

  madieva::TIter< int, std::string, std::less< int > > it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > it2 = tree.begin();
  ++it2;
  madieva::TIter< int, std::string, std::less< int > > it3 = tree.begin();
  ++it3;
  ++it3;

  auto new_root_it = tree.rotateLeft(it1);

  BOOST_CHECK_EQUAL((*it1).first, 10);
  BOOST_CHECK_EQUAL((*it2).first, 15);
  BOOST_CHECK_EQUAL((*it3).first, 20);

  BOOST_CHECK_EQUAL((*new_root_it).first, 20);

  madieva::TIter< int, std::string, std::less< int > > r_it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > r_it2 = tree.begin();
  ++r_it2;
  madieva::TIter< int, std::string, std::less< int > > r_it3 = tree.begin();
  ++r_it3;
  ++r_it3;
  BOOST_CHECK_EQUAL((*r_it1).first, 10);
  BOOST_CHECK_EQUAL((*r_it2).first, 15);
  BOOST_CHECK_EQUAL((*r_it3).first, 20);

  BOOST_CHECK(it1 == r_it1 && it2 == r_it2 && it3 == r_it3);
}

BOOST_AUTO_TEST_CASE(rotate_right_simple)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(5, "b");
  tree.push(7, "c");

  madieva::TIter< int, std::string, std::less< int > > it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > it2 = tree.begin();
  ++it2;
  madieva::TIter< int, std::string, std::less< int > > it3 = tree.begin();
  ++it3;
  ++it3;

  auto new_root_it = tree.rotateRight(it3);

  BOOST_CHECK_EQUAL((*it1).first, 5);
  BOOST_CHECK_EQUAL((*it2).first, 7);
  BOOST_CHECK_EQUAL((*it3).first, 10);

  BOOST_CHECK_EQUAL((*new_root_it).first, 5);

  madieva::TIter< int, std::string, std::less< int > > r_it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > r_it2 = tree.begin();
  ++r_it2;
  madieva::TIter< int, std::string, std::less< int > > r_it3 = tree.begin();
  ++r_it3;
  ++r_it3;
  BOOST_CHECK_EQUAL((*r_it1).first, 5);
  BOOST_CHECK_EQUAL((*r_it2).first, 7);
  BOOST_CHECK_EQUAL((*r_it3).first, 10);

  BOOST_CHECK(it1 == r_it1 && it2 == r_it2 && it3 == r_it3);
}

BOOST_AUTO_TEST_CASE(rotate_left_reference_stability)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "original");
  tree.push(20, "b");
  tree.push(15, "c");

  auto it = tree.find(10);
  (*it).second = "modified";

  tree.rotateLeft(it);

  BOOST_CHECK_EQUAL((*it).first, 10);
  BOOST_CHECK_EQUAL((*it).second, "modified");
}

BOOST_AUTO_TEST_CASE(height_test)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);

  tree.push(10, "a");
  BOOST_CHECK_EQUAL(tree.height(), 1);

  tree.push(20, "b");
  BOOST_CHECK_EQUAL(tree.height(), 2);

  tree.push(15, "c");
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(rotate_large_left)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");
  tree.push(15, "c");

  madieva::TIter< int, std::string, std::less< int > > it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > it2 = tree.begin();
  ++it2;
  madieva::TIter< int, std::string, std::less< int > > it3 = tree.begin();
  ++it3;
  ++it3;

  BOOST_CHECK_EQUAL((*it1).first, 10);
  BOOST_CHECK_EQUAL((*it2).first, 15);
  BOOST_CHECK_EQUAL((*it3).first, 20);

  auto new_root = tree.rotateLargeLeft(it1);

  BOOST_CHECK_EQUAL((*it1).first, 10);
  BOOST_CHECK_EQUAL((*it2).first, 15);
  BOOST_CHECK_EQUAL((*it3).first, 20);

  BOOST_CHECK_EQUAL((*new_root).first, 15);

  madieva::TIter< int, std::string, std::less< int > > r_it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > r_it2 = tree.begin();
  ++r_it2;
  madieva::TIter< int, std::string, std::less< int > > r_it3 = tree.begin();
  ++r_it3;
  ++r_it3;

  BOOST_CHECK_EQUAL((*r_it1).first, 10);
  BOOST_CHECK_EQUAL((*r_it2).first, 15);
  BOOST_CHECK_EQUAL((*r_it3).first, 20);

  BOOST_CHECK(it1 == r_it1 && it2 == r_it2 && it3 == r_it3);
}

BOOST_AUTO_TEST_CASE(rotate_large_right)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(20, "a");
  tree.push(10, "b");
  tree.push(15, "c");

  madieva::TIter< int, std::string, std::less< int > > it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > it2 = tree.begin();
  ++it2;
  madieva::TIter< int, std::string, std::less< int > > it3 = tree.begin();
  ++it3;
  ++it3;

  BOOST_CHECK_EQUAL((*it1).first, 10);
  BOOST_CHECK_EQUAL((*it2).first, 15);
  BOOST_CHECK_EQUAL((*it3).first, 20);

  auto new_root = tree.rotateLargeRight(it3);

  BOOST_CHECK_EQUAL((*it1).first, 10);
  BOOST_CHECK_EQUAL((*it2).first, 15);
  BOOST_CHECK_EQUAL((*it3).first, 20);

  BOOST_CHECK_EQUAL((*new_root).first, 15);

  madieva::TIter< int, std::string, std::less< int > > r_it1 = tree.begin();
  madieva::TIter< int, std::string, std::less< int > > r_it2 = tree.begin();
  ++r_it2;
  madieva::TIter< int, std::string, std::less< int > > r_it3 = tree.begin();
  ++r_it3;
  ++r_it3;

  BOOST_CHECK_EQUAL((*r_it1).first, 10);
  BOOST_CHECK_EQUAL((*r_it2).first, 15);
  BOOST_CHECK_EQUAL((*r_it3).first, 20);

  BOOST_CHECK(it1 == r_it1 && it2 == r_it2 && it3 == r_it3);
}

BOOST_AUTO_TEST_CASE(rotate_left_height)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");
  tree.push(15, "c");

  BOOST_CHECK_EQUAL(tree.height(), 3);

  auto it = tree.find(10);
  tree.rotateLeft(it);

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(rotate_right_height)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(20, "a");
  tree.push(10, "b");
  tree.push(15, "c");

  BOOST_CHECK_EQUAL(tree.height(), 3);

  auto it = tree.find(20);
  tree.rotateRight(it);

  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(rotate_large_left_height)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");
  tree.push(15, "c");

  BOOST_CHECK_EQUAL(tree.height(), 3);

  auto it = tree.find(10);
  tree.rotateLargeLeft(it);

  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(rotate_large_right_height)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(20, "a");
  tree.push(10, "b");
  tree.push(15, "c");

  BOOST_CHECK_EQUAL(tree.height(), 3);

  auto it = tree.find(20);
  tree.rotateLargeRight(it);

  BOOST_CHECK_EQUAL(tree.height(), 2);
}

BOOST_AUTO_TEST_CASE(rotate_left_node_heights)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");
  tree.push(15, "c");

  auto it10 = tree.find(10);
  auto it15 = tree.find(15);
  auto it20 = tree.find(20);

  BOOST_CHECK_EQUAL(tree.height(it10), 3);
  BOOST_CHECK_EQUAL(tree.height(it15), 1);
  BOOST_CHECK_EQUAL(tree.height(it20), 2);

  tree.rotateLeft(it10);

  BOOST_CHECK_EQUAL(tree.height(it10), 2);
  BOOST_CHECK_EQUAL(tree.height(it15), 1);
  BOOST_CHECK_EQUAL(tree.height(it20), 3);
}

BOOST_AUTO_TEST_CASE(rotate_right_node_heights)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(20, "a");
  tree.push(10, "b");
  tree.push(15, "c");

  auto it10 = tree.find(10);
  auto it15 = tree.find(15);
  auto it20 = tree.find(20);

  BOOST_CHECK_EQUAL(tree.height(it20), 3);
  BOOST_CHECK_EQUAL(tree.height(it10), 2);
  BOOST_CHECK_EQUAL(tree.height(it15), 1);

  tree.rotateRight(it20);

  BOOST_CHECK_EQUAL(tree.height(it20), 2);
  BOOST_CHECK_EQUAL(tree.height(it10), 3);
  BOOST_CHECK_EQUAL(tree.height(it15), 1);
}

BOOST_AUTO_TEST_CASE(rotate_large_left_node_heights)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");
  tree.push(15, "c");

  auto it10 = tree.find(10);
  auto it15 = tree.find(15);
  auto it20 = tree.find(20);

  BOOST_CHECK_EQUAL(tree.height(it10), 3);
  BOOST_CHECK_EQUAL(tree.height(it15), 1);
  BOOST_CHECK_EQUAL(tree.height(it20), 2);

  tree.rotateLargeLeft(it10);

  BOOST_CHECK_EQUAL(tree.height(it10), 1);
  BOOST_CHECK_EQUAL(tree.height(it15), 2);
  BOOST_CHECK_EQUAL(tree.height(it20), 1);
}

BOOST_AUTO_TEST_CASE(rotate_large_right_node_heights)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(20, "a");
  tree.push(10, "b");
  tree.push(15, "c");

  auto it10 = tree.find(10);
  auto it15 = tree.find(15);
  auto it20 = tree.find(20);

  BOOST_CHECK_EQUAL(tree.height(it20), 3);
  BOOST_CHECK_EQUAL(tree.height(it10), 2);
  BOOST_CHECK_EQUAL(tree.height(it15), 1);

  tree.rotateLargeRight(it20);

  BOOST_CHECK_EQUAL(tree.height(it20), 1);
  BOOST_CHECK_EQUAL(tree.height(it10), 1);
  BOOST_CHECK_EQUAL(tree.height(it15), 2);
}

BOOST_AUTO_TEST_CASE(rotate_left_throws_on_no_right_child)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");

  auto it = tree.find(10);
  BOOST_CHECK_THROW(tree.rotateLeft(it), std::logic_error);
}

BOOST_AUTO_TEST_CASE(rotate_right_throws_on_no_left_child)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");

  auto it = tree.find(10);
  BOOST_CHECK_THROW(tree.rotateRight(it), std::logic_error);
}

BOOST_AUTO_TEST_CASE(rotate_large_left_throws_on_wrong_structure)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(10, "a");
  tree.push(20, "b");

  auto it = tree.find(10);
  BOOST_CHECK_THROW(tree.rotateLargeLeft(it), std::logic_error);
}

BOOST_AUTO_TEST_CASE(rotate_large_right_throws_on_wrong_structure)
{
  madieva::BSTree< int, std::string, std::less< int > > tree;
  tree.push(20, "a");
  tree.push(10, "b");

  auto it = tree.find(20);
  BOOST_CHECK_THROW(tree.rotateLargeRight(it), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
