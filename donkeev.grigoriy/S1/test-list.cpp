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
