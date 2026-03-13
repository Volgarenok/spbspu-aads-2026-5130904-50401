#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListBasic)

BOOST_AUTO_TEST_CASE(DefaultConstruct)
{
  burukov::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(PushFrontSingle)
{
  burukov::List< int > list;
  list.pushFront(42);
  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 42);
}

BOOST_AUTO_TEST_CASE(PushFrontOrder)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 3);
  burukov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  list.popFront();
  list.popFront();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(InsertAfter)
{
  burukov::List< int > list;
  list.pushFront(1);
  burukov::LIter< int > it = list.begin();
  burukov::LIter< int > ins = list.insertAfter(it, 2);
  list.insertAfter(ins, 3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(EraseAfter)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  burukov::LIter< int > it = list.begin();
  burukov::LIter< int > next = list.eraseAfter(it);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*next, 3);
}

BOOST_AUTO_TEST_CASE(ClearList)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(ClearEmpty)
{
  burukov::List< int > list;
  list.clear();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_SUITE_END()
