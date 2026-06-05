#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(listTesting)
BOOST_AUTO_TEST_CASE(defaultConstruct)
{
  donkeev::List< int > list;
  BOOST_TEST(list.size() == 0);
}BOOST_AUTO_TEST_CASE(anySpecificElements)
{
  donkeev::List< int > list{2, 4};
  BOOST_TEST(list.size() == 2);
  BOOST_TEST((*(list.begin()) == 4 && *(list.begin()++) == 4));
}
BOOST_AUTO_TEST_CASE(anotherListConsrtuct)
{
  donkeev::List< int > firstList{2, 4};
  donkeev::List< int > secondList{firstList};
  BOOST_TEST(secondList.size() == 2);
  BOOST_TEST((*(secondList.begin()) == 4 && *(secondList.begin()++) == 4));
}
BOOST_AUTO_TEST_CASE(listDestructor)
{
  donkeev::List< int > list{2, 4};
  list.~List();
  BOOST_TEST(list.size() == 0);
  BOOST_CHECK(list.begin() == nullptr);
}
BOOST_AUTO_TEST_CASE(beginMethod)
{
  donkeev::List< int > list{2, 4};
  BOOST_TEST((*(list.begin()) == 4 && *(list.cbegin()) == 4));
}
BOOST_AUTO_TEST_CASE(pushAfterMethod)
{
  donkeev::List< int > list{2, 4};
  list.pushAfter(++list.begin(), 3);
  BOOST_TEST(*(list.begin() + 2) == 3);
}
BOOST_AUTO_TEST_CASE(pushFrontMethod)
{
  donkeev::List< int > list{2, 4};
  list.pushFront(3);
  BOOST_TEST(*(list.begin()) == 3);
}
BOOST_AUTO_TEST_CASE(pushBackMethod)
{
  donkeev::List< int > list{2, 4};
  list.pushBack(3);
  BOOST_TEST(*(list.begin() + 2) == 3);
}
BOOST_AUTO_TEST_CASE(poopFrontMethod)
{
  donkeev::List< int > list{2, 4};
  list.popFront();
  BOOST_TEST((*(list.begin()) == 4 && list.size() == 1));
}
BOOST_AUTO_TEST_CASE(cutAfterMethod)
{
  donkeev::List< int > list{2, 4};
  donkeev::LIter< int > iter{list.begin()};
  list.cutAfter(iter);
  BOOST_TEST((*(list.begin()) == 4 && list.size() == 1));
}
BOOST_AUTO_TEST_CASE(clearAllMethod)
{
  donkeev::List< int > list{2, 4};
  list.clearAll();
  BOOST_CHECK(list.begin() == nullptr);
}
BOOST_AUTO_TEST_CASE(isEmptyMethod)
{
  donkeev::List< int > list{};
  BOOST_CHECK(list.isEmpty());
}
BOOST_AUTO_TEST_CASE(sizeMethod)
{
  donkeev::List< int > list{2, 4};
  BOOST_CHECK(list.size() == 2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(iteratorsTesting)
BOOST_AUTO_TEST_CASE(defaultIterConstruct)
{
  donkeev::LIter< int > iter;
  BOOST_CHECK(iter == nullptr);
}
BOOST_AUTO_TEST_CASE(anotherIterConstruct)
{
  donkeev::LIter< int > firstIter;
  donkeev::LIter< int > secondIter{firstIter};
  BOOST_CHECK(secondIter == nullptr);
}
BOOST_AUTO_TEST_CASE(iterConstructByList)
{
  donkeev::List< int > list{2, 3};
  donkeev::LIter< int > iter{list.begin()};
  BOOST_CHECK(*iter == 3);
}
BOOST_AUTO_TEST_CASE(iterOperatorPlus)
{
  donkeev::List< int > list{2, 3};
  donkeev::LIter< int > iter{list.begin()};
  BOOST_CHECK(*(iter + 1) == 3);
}
BOOST_AUTO_TEST_CASE(iterOperatorPlusPlus)
{
  donkeev::List< int > list{2, 3};
  donkeev::LIter< int > iter{list.begin()};
  BOOST_CHECK(*(++iter) == 3);
}
BOOST_AUTO_TEST_CASE(yaIterOperatorPlusPlus)
{
  donkeev::List< int > list{2, 3};
  donkeev::LIter< int > iter{list.begin()};
  BOOST_CHECK(*(iter++) == 3);
}
BOOST_AUTO_TEST_CASE(iterOperatorPlusEqual)
{
  donkeev::List< int > list{2, 3};
  donkeev::LIter< int > iter{list.begin()};
  iter += 1;
  BOOST_CHECK(*iter == 3);
}
BOOST_AUTO_TEST_CASE(iterArrowOperator)
{
  donkeev::List< int > list{2, 3};
  donkeev::List< donkeev::List< int > > outerList{1, list};
  donkeev::LIter< donkeev::List< int > > iter{outerList.begin()};
  BOOST_CHECK(*(iter->begin()) == 3);
}
BOOST_AUTO_TEST_CASE(iterOperatorEqualEqual)
{
  donkeev::List< int > list{2, 3};
  donkeev::LIter< int > firstIter{list.begin()};
  donkeev::LIter< int > secondIter{list.begin()};
  BOOST_CHECK(firstIter == secondIter);
}
BOOST_AUTO_TEST_CASE(iterOperatorNonEqual)
{
  donkeev::List< int > list{2, 3};
  donkeev::LIter< int > firstIter{list.begin()};
  donkeev::LIter< int > secondIter{list.begin() + 1};
  BOOST_CHECK(firstIter != secondIter);
}
BOOST_AUTO_TEST_CASE(defaultConstIterConstruct)
{
  donkeev::LCIter< int > iter;
  BOOST_CHECK(iter == nullptr);
}
BOOST_AUTO_TEST_CASE(anotherConstIterConstruct)
{
  donkeev::LCIter< int > firstIter;
  donkeev::LCIter< int > secondIter{firstIter};
  BOOST_CHECK(secondIter == nullptr);
}
BOOST_AUTO_TEST_CASE(constIterConstructByList)
{
  donkeev::List< int > list{2, 3};
  donkeev::LCIter< int > iter{list.cbegin()};
  BOOST_CHECK(*iter == 3);
}
BOOST_AUTO_TEST_CASE(constIterOperatorPlus)
{
  donkeev::List< int > list{2, 3};
  donkeev::LCIter< int > iter{list.cbegin()};
  BOOST_CHECK(*(iter + 1) == 3);
}
BOOST_AUTO_TEST_CASE(constIterOperatorPlusPlus)
{
  donkeev::List< int > list{2, 3};
  donkeev::LCIter< int > iter{list.cbegin()};
  BOOST_CHECK(*(++iter) == 3);
}
BOOST_AUTO_TEST_CASE(yaConstIterOperatorPlusPlus)
{
  donkeev::List< int > list{2, 3};
  donkeev::LCIter< int > iter{list.cbegin()};
  BOOST_CHECK(*(iter++) == 3);
}
BOOST_AUTO_TEST_CASE(constIterOperatorPlusEqual)
{
  donkeev::List< int > list{2, 3};
  donkeev::LCIter< int > iter{list.cbegin()};
  iter += 1;
  BOOST_CHECK(*iter == 3);
}
BOOST_AUTO_TEST_CASE(ConstIterArrowOperator)
{
  donkeev::List< int > list{2, 3};
  donkeev::List< donkeev::List< int > > outerList{1, list};
  donkeev::LCIter< donkeev::List< int > > iter{outerList.cbegin()};
  BOOST_CHECK(*(iter->cbegin()) == 3);
}
BOOST_AUTO_TEST_CASE(constIterOperatorEqualEqual)
{
  donkeev::List< int > list{2, 3};
  donkeev::LCIter< int > firstIter{list.cbegin()};
  donkeev::LCIter< int > secondIter{list.cbegin()};
  BOOST_CHECK(firstIter == secondIter);
}
BOOST_AUTO_TEST_CASE(ConstIterOperatorNonEqual)
{
  donkeev::List< int > list{2, 3};
  donkeev::LCIter< int > firstIter{list.cbegin()};
  donkeev::LCIter< int > secondIter{list.cbegin() + 1};
  BOOST_CHECK(firstIter != secondIter);
}
BOOST_AUTO_TEST_SUITE_END()
