#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include "bstree.hpp"

std::vector< int > get_keys(const vasyakin::BSTree< int, std::string >& tree)
{
  std::vector< int > res;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    res.push_back((*it).first);
  }

  return res;
}

BOOST_AUTO_TEST_SUITE()

BOOST_AUTO_TEST_CASE(test_empty_height)
{
  vasyakin::BSTree< int, std::string > tree;

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.height(), 0);

  tree.push(10, "ten");
  tree.push(20, "twenty");

  BOOST_CHECK(!tree.empty());
  BOOST_CHECK_EQUAL(tree.height(), 2);

  auto it_min = tree.begin();
  BOOST_CHECK_EQUAL(tree.height(it_min), 2);
}

BOOST_AUTO_TEST_CASE(test_push_get_drop)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(50, "50");
  tree.push(30, "30");
  tree.push(70, "70");
  tree.push(20, "20");
  tree.push(40, "40");

  BOOST_CHECK_EQUAL(tree.get(30), "30");
  BOOST_CHECK_THROW(tree.get(999), std::out_of_range);

  tree.push(30, "30_UPDATED");
  BOOST_CHECK_EQUAL(tree.get(30), "30_UPDATED");

  BOOST_CHECK_EQUAL(tree.drop(20), "20");
  BOOST_CHECK_EQUAL(tree.height(), 3);

  BOOST_CHECK_EQUAL(tree.drop(40), "40");

  tree.push(40, "40_back");
  tree.push(60, "60");

  std::string val = tree.drop(50);
  BOOST_CHECK_EQUAL(val, "50");

  auto keys = get_keys(tree);

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), 
    (std::vector< int >{30, 40, 60, 70}).begin(), 
    (std::vector< int >{30, 40, 60, 70}).end());

  BOOST_CHECK_THROW(tree.drop(111), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(3, "c");
  tree.push(1, "a");
  tree.push(2, "b");

  auto it = tree.begin();

  BOOST_CHECK_EQUAL((*it).first, 1);
  ++it;

  BOOST_CHECK_EQUAL((*it).first, 2);
  ++it;

  BOOST_CHECK_EQUAL((*it).first, 3);
  ++it;

  BOOST_CHECK(it == tree.end());

  --it;
  BOOST_CHECK_EQUAL((*it).first, 3);

  --it;
  BOOST_CHECK_EQUAL((*it).first, 2);

  const auto& ct = tree;
  auto cit = ct.cbegin();
  BOOST_CHECK_EQUAL((*cit).first, 1);
}

BOOST_AUTO_TEST_CASE(test_rule_of_five)
{
  vasyakin::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  vasyakin::BSTree< int, std::string > tree2(tree1);
  BOOST_CHECK_EQUAL(tree2.get(1), "one");

  tree2.drop(1);
  BOOST_CHECK_EQUAL(tree1.get(1), "one");

  vasyakin::BSTree< int, std::string > tree3;

  tree3 = tree1;
  BOOST_CHECK_EQUAL(tree3.get(2), "two");

  vasyakin::BSTree< int, std::string > tree4(std::move(tree3));

  BOOST_CHECK(tree3.empty());
  BOOST_CHECK_EQUAL(tree4.get(2), "two");

  vasyakin::BSTree< int, std::string > tree5;

  tree5 = std::move(tree4);
  BOOST_CHECK(tree4.empty());
  BOOST_CHECK_EQUAL(tree5.get(1), "one");
}

BOOST_AUTO_TEST_CASE(rotate_simple)
{
  vasyakin::BSTree< int, std::string > tree;
  tree.push(10, "10");
  tree.push(20, "20");
  
  auto it_rise = tree.begin();
  ++it_rise;
  tree.rotateLeft(it_rise);

  BOOST_CHECK_EQUAL(tree.height(), 2);
  BOOST_CHECK_EQUAL((*tree.begin()).first, 10);
  BOOST_CHECK_EQUAL(tree.get(20), "20");

  auto it_left = tree.begin();
  tree.rotateRight(it_left);
  BOOST_CHECK_EQUAL((*tree.begin()).first, 10);
}

BOOST_AUTO_TEST_CASE(rotate_large)
{
  vasyakin::BSTree< int, std::string > tree1;
  tree1.push(50, "50");
  tree1.push(30, "30");
  tree1.push(40, "40");

  auto it_mid = tree1.begin();
  ++it_mid;

  tree1.rotateLargeLeft(it_mid);
  auto k1 = get_keys(tree1);

  BOOST_CHECK_EQUAL_COLLECTIONS(k1.begin(), k1.end(), 
    (std::vector< int >{30, 40, 50}).begin(), 
    (std::vector< int >{30, 40, 50}).end());

  vasyakin::BSTree< int, std::string > tree2;
  tree2.push(30, "30");
  tree2.push(50, "50");
  tree2.push(40, "40");
  
  auto it_mid2 = tree2.begin();
  ++it_mid2;

  tree2.rotateLargeRight(it_mid2);
  auto k2 = get_keys(tree2);

  BOOST_CHECK_EQUAL_COLLECTIONS(k2.begin(), k2.end(), 
    (std::vector< int >{30, 40, 50}).begin(), 
    (std::vector< int >{30, 40, 50}).end());
}

BOOST_AUTO_TEST_SUITE_END()
