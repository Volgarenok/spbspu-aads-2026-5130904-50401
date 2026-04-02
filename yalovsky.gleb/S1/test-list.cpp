#include <boost/test/unit_test.hpp>

#include <limits>
#include <sstream>
#include <string>
#include <utility>

#include "input-output.hpp"
#include "list.hpp"

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  yalovsky::List< int > list;

  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(pushFrontAndPushBack)
{
  yalovsky::List< int > list;
  list.pushBack(2);
  list.pushFront(1);
  list.pushBack(3);

  BOOST_TEST(list.size() == 3);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 3);
  BOOST_TEST(*(list.begin() + 1) == 2);
}

BOOST_AUTO_TEST_CASE(insertAndErase)
{
  yalovsky::List< int > list;
  list.pushBack(1);
  list.pushBack(3);

  yalovsky::List< int >::iterator pos = list.begin();
  ++pos;
  list.insert(pos, 2);

  BOOST_TEST(*(list.begin() + 1) == 2);

  pos = list.begin() + 1;
  yalovsky::List< int >::iterator next = list.erase(pos);
  BOOST_TEST(*next == 3);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*(list.begin() + 1) == 3);
}

BOOST_AUTO_TEST_CASE(copyConstructorCreatesIndependentCopy)
{
  yalovsky::List< int > source;
  source.pushBack(1);
  source.pushBack(2);

  yalovsky::List< int > copy(source);
  copy.popFront();
  copy.pushBack(3);

  BOOST_TEST(source.size() == 2);
  BOOST_TEST(source.front() == 1);
  BOOST_TEST(source.back() == 2);
  BOOST_TEST(copy.front() == 2);
  BOOST_TEST(copy.back() == 3);
}

BOOST_AUTO_TEST_CASE(moveConstructorLeavesSourceValid)
{
  yalovsky::List< int > source;
  source.pushBack(10);
  source.pushBack(20);

  yalovsky::List< int > moved(std::move(source));

  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(moved.front() == 10);
  BOOST_TEST(moved.back() == 20);
  BOOST_TEST(source.empty());
  BOOST_TEST(source.begin() == source.end());
}

BOOST_AUTO_TEST_CASE(clearRemovesAllElements)
{
  yalovsky::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  list.clear();

  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.begin() == list.end());
}