#include <boost/test/unit_test.hpp>
#include <utility>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTestSuite)

BOOST_AUTO_TEST_CASE(CreationOfList)
{
  kondrat::List< int > l;
  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.size() == 0);
  BOOST_CHECK(l.begin() == l.end());
}

BOOST_AUTO_TEST_CASE(PushBackAndFrontBack)
{
  kondrat::List< int > l;
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
  kondrat::List< int > l;
  l.pushFront(2);
  l.pushFront(1);
  l.pushFront(0);

  BOOST_CHECK(l.front() == 0);
  BOOST_CHECK(l.back() == 2);
  BOOST_CHECK(l.size() == 3);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  kondrat::List< int > l;
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
  kondrat::List< int > l;
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
  kondrat::List< int > l;
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
  kondrat::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  kondrat::LIter< int > it = l.begin();

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
  kondrat::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  const kondrat::List< int > & cl = l;
  kondrat::LCIter< int > it = cl.begin();

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
  kondrat::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  kondrat::List< int > copy(l);
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
  kondrat::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  kondrat::List< int > copy;
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
  kondrat::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  kondrat::List< int > moved(std::move(l));

  BOOST_CHECK(moved.front() == 1);
  BOOST_CHECK(moved.back() == 3);
  BOOST_CHECK(moved.size() == 3);

  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.size() == 0);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  kondrat::List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  kondrat::List< int > moved;
  moved = std::move(l);

  BOOST_CHECK(moved.front() == 1);
  BOOST_CHECK(moved.back() == 3);
  BOOST_CHECK(moved.size() == 3);

  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
