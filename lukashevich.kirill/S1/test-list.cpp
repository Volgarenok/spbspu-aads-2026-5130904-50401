#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTestSuite)

BOOST_AUTO_TEST_CASE(CreationOfList)
{
  lukashevich::List< int > l;
  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.size() == 0);
  BOOST_CHECK(l.begin() == l.end());
}

BOOST_AUTO_TEST_CASE(PushBackAndFrontBack)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 3);
  BOOST_CHECK(l.size() == 3);
  BOOST_CHECK(!l.empty());
}

BOOST_AUTO_TEST_CASE(PushFront)
{
  lukashevich::List< int > l;
  l.pushFront(2);
  l.pushFront(1);
  l.pushFront(0);

  BOOST_CHECK(l.front() == 0);
  BOOST_CHECK(l.back() == 2);
  BOOST_CHECK(l.size() == 3);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  l.popFront();

  BOOST_CHECK(l.front() == 2);
  BOOST_CHECK(l.back() == 3);
  BOOST_CHECK(l.size() == 2);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  l.popBack();

  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 2);
  BOOST_CHECK(l.size() == 2);
}

BOOST_AUTO_TEST_CASE(ClearSizeEmpty)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  BOOST_CHECK(l.size() == 3);
  BOOST_CHECK(!l.empty());

  l.clear();

  BOOST_CHECK(l.size() == 0);
  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.begin() == l.end());
}

BOOST_AUTO_TEST_CASE(IteratorWalk)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  lukashevich::LIter< int > it = l.begin();

  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(it == l.end());
}

BOOST_AUTO_TEST_CASE(ConstIteratorWalk)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  const lukashevich::List< int >& cl = l;
  lukashevich::LCIter< int > it = cl.begin();

  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(it == cl.end());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  lukashevich::List< int > copy(l);
  copy.pushBack(4);

  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 3);
  BOOST_CHECK(l.size() == 3);

  BOOST_CHECK(copy.front() == 1);
  BOOST_CHECK(copy.back() == 4);
  BOOST_CHECK(copy.size() == 4);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  lukashevich::List< int > copy;
  copy = l;
  copy.popFront();

  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 3);
  BOOST_CHECK(l.size() == 3);

  BOOST_CHECK(copy.front() == 2);
  BOOST_CHECK(copy.back() == 3);
  BOOST_CHECK(copy.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  lukashevich::List< int > moved(std::move(l));

  BOOST_CHECK(moved.front() == 1);
  BOOST_CHECK(moved.back() == 3);
  BOOST_CHECK(moved.size() == 3);

  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.size() == 0);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  lukashevich::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  lukashevich::List< int > moved;
  moved = std::move(l);

  BOOST_CHECK(moved.front() == 1);
  BOOST_CHECK(moved.back() == 3);
  BOOST_CHECK(moved.size() == 3);

  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
