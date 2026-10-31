#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "BSTree.hpp"

using namespace khasnulin;

BOOST_AUTO_TEST_SUITE(EmptyTreeSuite)

BOOST_AUTO_TEST_CASE(DefaultConstructorCreatesEmptyTree)
{
  BSTree< int, std::string > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.height() == 0);
  BOOST_TEST((tree.begin() == tree.end()));
  BOOST_TEST((tree.cbegin() == tree.cend()));
}

BOOST_AUTO_TEST_CASE(LookupInEmptyTree)
{
  BSTree< int, std::string > tree;
  BOOST_TEST((tree.find(42) == tree.end()));

  const auto &ctree = tree;
  BOOST_TEST((ctree.find(42) == ctree.cend()));

  BOOST_CHECK_THROW(tree.get(42), std::out_of_range);
  BOOST_CHECK_THROW(ctree.get(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(ClearAndEraseOnEmptyTree)
{
  BSTree< int, std::string > tree;
  BOOST_CHECK_NO_THROW(tree.clear());
  BOOST_CHECK_NO_THROW(tree.erase(10));
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(InsertAndLookupSuite)

BOOST_AUTO_TEST_CASE(InsertSingleAndMultipleElements)
{
  BSTree< int, std::string > tree;

  std::pair< const int, std::string > lval_pair{10, "ten"};
  tree.insert(lval_pair);
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.height() == 1);
  BOOST_TEST(tree.get(10) == "ten");

  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({13, "thirteen"});

  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(tree.height() == 3);
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(15) == "fifteen");
  BOOST_TEST(tree.get(13) == "thirteen");
}

BOOST_AUTO_TEST_CASE(InsertDuplicateKeyDoesNotOverwrite)
{
  BSTree< int, std::string > tree;
  tree.insert({10, "original"});
  tree.insert({10, "duplicate"});

  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(10) == "original");
}

BOOST_AUTO_TEST_CASE(SubscriptOperatorCreatesAndModifies)
{
  BSTree< int, std::string > tree;

  tree[20] = "twenty";
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(20) == "twenty");

  tree[20] = "modified";
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(20) == "modified");

  BOOST_TEST(tree[30] == "");
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(FindAndConstGet)
{
  BSTree< int, int > tree;
  tree.insert({10, 100});
  tree.insert({5, 50});
  tree.insert({15, 150});

  auto it = tree.find(5);
  BOOST_TEST((it != tree.end()));
  BOOST_TEST(it->first == 5);
  BOOST_TEST(it->second == 50);

  it->second = 55;
  BOOST_TEST(tree.get(5) == 55);
  tree.get(5) = 60;

  const auto &ctree = tree;
  BOOST_TEST(ctree.get(5) == 60);
  BOOST_TEST((ctree.find(999) == ctree.cend()));
  BOOST_CHECK_THROW(ctree.get(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(CustomComparatorDescendingOrder)
{
  BSTree< int, std::string, std::greater< int > > tree;
  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  tree.insert({5, "five"});

  BOOST_TEST(tree.begin()->first == 20);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IteratorsSuite)

BOOST_AUTO_TEST_CASE(ForwardAndBackwardIteration)
{
  BSTree< int, int > tree;
  std::vector< int > keys = {8, 3, 10, 1, 6, 14, 4, 7, 13};
  for (int k : keys)
  {
    tree.insert({k, k * 10});
  }

  std::vector< int > expected_asc = {1, 3, 4, 6, 7, 8, 10, 13, 14};
  std::vector< int > actual_asc;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    actual_asc.push_back(it->first);
  }
  BOOST_TEST(actual_asc == expected_asc);

  std::vector< int > expected_desc = {14, 13, 10, 8, 7, 6, 4, 3, 1};
  std::vector< int > actual_desc;
  auto it = tree.end();
  while (it != tree.begin())
  {
    --it;
    actual_desc.push_back((*it).first);
  }
  BOOST_TEST(actual_desc == expected_desc);
}

BOOST_AUTO_TEST_CASE(ConstIteratorConversionAndPostfixOps)
{
  BSTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});

  BSTree< int, std::string >::iter it = tree.begin();
  BSTree< int, std::string >::const_iter cit = it;
  BOOST_TEST((cit == it));

  auto old_it = it++;
  BOOST_TEST(old_it->first == 1);
  BOOST_TEST(it->first == 2);

  auto old_it2 = it--;
  BOOST_TEST(old_it2->first == 2);
  BOOST_TEST(it->first == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RuleOfFiveAndSwapSuite)

BOOST_AUTO_TEST_CASE(CopyConstructorMakesDeepCopy)
{
  BSTree< int, std::string > original;
  original.insert({10, "ten"});
  original.insert({5, "five"});
  original.insert({15, "fifteen"});

  BSTree< int, std::string > copy(original);

  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(copy.height() == 2);
  BOOST_TEST(copy.get(10) == "ten");
  BOOST_TEST(copy.get(5) == "five");
  BOOST_TEST(copy.get(15) == "fifteen");

  copy.get(5) = "modified_five";
  copy.insert({20, "twenty"});

  BOOST_TEST(original.size() == 3);
  BOOST_TEST(original.get(5) == "five");
  BOOST_TEST((original.find(20) == original.end()));
}

BOOST_AUTO_TEST_CASE(MoveConstructorTransfersOwnershipAndFixesFakeRootParent)
{
  BSTree< int, int > source;
  source.insert({10, 100});
  source.insert({5, 50});
  source.insert({15, 150});

  BSTree< int, int > moved(std::move(source));

  BOOST_TEST(source.empty());
  BOOST_TEST(source.size() == 0);
  BOOST_TEST((source.begin() == source.end()));

  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(moved.get(10) == 100);

  auto it = moved.end();
  --it;
  BOOST_TEST(it->first == 15);
  ++it;
  BOOST_TEST((it == moved.end()));
}

BOOST_AUTO_TEST_CASE(CopyAndMoveAssignment)
{
  BSTree< int, int > t1;
  t1.insert({1, 10});
  t1.insert({2, 20});

  BSTree< int, int > t2;
  t2.insert({100, 1000});

  t2 = t1;
  BOOST_TEST(t2.size() == 2);
  BOOST_TEST(t2.get(1) == 10);
  BOOST_TEST((t2.find(100) == t2.end()));

  BSTree< int, int > t3;
  t3 = std::move(t2);
  BOOST_TEST(t2.empty());
  BOOST_TEST(t3.size() == 2);
  BOOST_TEST(t3.get(2) == 20);
}

BOOST_AUTO_TEST_CASE(SwapExchangesTreesCorrectly)
{
  BSTree< int, std::string > t1;
  t1.insert({1, "one"});

  BSTree< int, std::string > t2;
  t2.insert({10, "ten"});
  t2.insert({20, "twenty"});

  t1.swap(t2);

  BOOST_TEST(t1.size() == 2);
  BOOST_TEST(t1.get(10) == "ten");
  BOOST_TEST((--t1.end())->first == 20);

  BOOST_TEST(t2.size() == 1);
  BOOST_TEST(t2.get(1) == "one");
  BOOST_TEST((--t2.end())->first == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(EraseSuite)

BOOST_AUTO_TEST_CASE(EraseLeafAndSingleChildNode)
{
  BSTree< int, int > tree;
  for (int k : {10, 5, 15, 2, 9, 13})
  {
    tree.insert({k, k});
  }

  tree.erase(2);
  BOOST_TEST(tree.size() == 5);
  BOOST_TEST((tree.find(2) == tree.end()));

  tree.erase(15);
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST((tree.find(15) == tree.end()));
  BOOST_TEST((tree.find(13) != tree.end()));

  std::vector< int > remaining;
  for (const auto &p : tree)
  {
    remaining.push_back(p.first);
  }
  std::vector< int > expected = {5, 9, 10, 13};
  BOOST_TEST(remaining == expected);
}

BOOST_AUTO_TEST_CASE(EraseNodeWithTwoChildrenAndIteratorStability)
{
  BSTree< int, int > tree;
  for (int k : {10, 5, 15, 2, 9, 13, 20})
  {
    tree.insert({k, k * 10});
  }

  auto it_9 = tree.find(9);
  auto it_13 = tree.find(13);
  const auto *addr_9 = &(*it_9);
  const auto *addr_13 = &(*it_13);

  auto next_it = tree.erase(tree.find(10));

  BOOST_TEST((next_it == it_13));
  BOOST_TEST(tree.size() == 6);
  BOOST_TEST((tree.find(10) == tree.end()));

  BOOST_TEST(&(*it_9) == addr_9);
  BOOST_TEST(&(*it_13) == addr_13);
  BOOST_TEST(it_9->first == 9);
  BOOST_TEST(it_13->first == 13);

  std::vector< int > actual;
  for (const auto &p : tree)
  {
    actual.push_back(p.first);
  }
  std::vector< int > expected = {2, 5, 9, 13, 15, 20};
  BOOST_TEST(actual == expected);
}

BOOST_AUTO_TEST_SUITE_END()

namespace
{
  std::vector< int > keysForward(const BSTree< int, int > &tree)
  {
    std::vector< int > keys;
    for (auto it = tree.cbegin(); it != tree.cend(); ++it)
    {
      keys.push_back(it->first);
    }
    return keys;
  }

  std::vector< int > keysBackward(const BSTree< int, int > &tree)
  {
    std::vector< int > keys;
    auto it = tree.cend();
    while (it != tree.cbegin())
    {
      --it;
      keys.push_back(it->first);
    }
    return keys;
  }

  void checkOrder(const BSTree< int, int > &tree, const std::vector< int > &expected)
  {
    BOOST_TEST(keysForward(tree) == expected);
    std::vector< int > reversed(expected.rbegin(), expected.rend());
    BOOST_TEST(keysBackward(tree) == reversed);
  }
}

BOOST_AUTO_TEST_SUITE(RotationsAndHeightSuite)

BOOST_AUTO_TEST_CASE(SubtreeHeightCalculation)
{
  BSTree< int, int > tree;
  for (int k : {1, 2, 3, 4})
  {
    tree.insert({k, k});
  }

  BOOST_TEST(tree.height() == 4);
  BOOST_TEST(tree.height(tree.find(1)) == 4);
  BOOST_TEST(tree.height(tree.find(2)) == 3);
  BOOST_TEST(tree.height(tree.find(4)) == 1);
  BOOST_TEST(tree.height(tree.end()) == 0);
}

BOOST_AUTO_TEST_CASE(SmallRotationsAtRoot)
{
  BSTree< int, int > tree;
  for (int k : {10, 20, 30})
  {
    tree.insert({k, k * 10});
  }
  BOOST_TEST(tree.height() == 3);

  auto it_10 = tree.find(10);
  auto it_20 = tree.find(20);
  const auto *addr_10 = &(*it_10);
  const auto *addr_20 = &(*it_20);

  auto res = tree.rotateLeft(it_10);
  BOOST_TEST((res == it_20));
  BOOST_TEST(tree.height() == 2);
  BOOST_TEST(tree.height(it_20) == 2);
  BOOST_TEST(&(*it_10) == addr_10);
  BOOST_TEST(&(*it_20) == addr_20);
  checkOrder(tree, {10, 20, 30});

  auto res2 = tree.rotateRight(it_20);
  BOOST_TEST((res2 == it_10));
  BOOST_TEST(tree.height() == 3);
  BOOST_TEST(tree.height(it_10) == 3);
  checkOrder(tree, {10, 20, 30});
}

BOOST_AUTO_TEST_CASE(SmallRotationBelowRoot)
{
  BSTree< int, int > tree;
  for (int k : {50, 30, 20, 10})
  {
    tree.insert({k, k});
  }
  BOOST_TEST(tree.height() == 4);

  auto res = tree.rotateRight(tree.find(30));
  BOOST_TEST(res->first == 20);
  BOOST_TEST(tree.height() == 3);
  BOOST_TEST(tree.height(tree.find(50)) == 3);
  BOOST_TEST(tree.height(res) == 2);
  checkOrder(tree, {10, 20, 30, 50});
}

BOOST_AUTO_TEST_CASE(LargeLeftRotation)
{
  BSTree< int, int > tree;
  for (int k : {10, 30, 20})
  {
    tree.insert({k, k});
  }
  BOOST_TEST(tree.height() == 3);

  auto res = tree.rotateLargeLeft(tree.find(10));
  BOOST_TEST(res->first == 20);
  BOOST_TEST(tree.height() == 2);
  BOOST_TEST(tree.height(res) == 2);
  checkOrder(tree, {10, 20, 30});
}

BOOST_AUTO_TEST_CASE(LargeRightRotation)
{
  BSTree< int, int > tree;
  for (int k : {30, 10, 20})
  {
    tree.insert({k, k});
  }
  BOOST_TEST(tree.height() == 3);

  auto res = tree.rotateLargeRight(tree.find(30));
  BOOST_TEST(res->first == 20);
  BOOST_TEST(tree.height() == 2);
  BOOST_TEST(tree.height(res) == 2);
  checkOrder(tree, {10, 20, 30});
}

BOOST_AUTO_TEST_CASE(InvalidRotationsThrowAndKeepTreeUnchanged)
{
  BSTree< int, int > tree;
  tree.insert({10, 1});
  tree.insert({5, 2});

  BOOST_CHECK_THROW(tree.rotateLeft(tree.end()), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateRight(tree.end()), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateLeft(tree.find(10)), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateLeft(tree.find(5)), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateRight(tree.find(5)), std::logic_error);

  BOOST_CHECK_THROW(tree.rotateLargeLeft(tree.find(10)), std::logic_error);
  BOOST_CHECK_THROW(tree.rotateLargeRight(tree.find(10)), std::logic_error);

  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(tree.height() == 2);
  checkOrder(tree, {5, 10});
}

BOOST_AUTO_TEST_SUITE_END()
