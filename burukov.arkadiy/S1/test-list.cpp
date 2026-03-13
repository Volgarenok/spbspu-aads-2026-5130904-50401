#include <boost/test/unit_test.hpp>
#include <utility>
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

BOOST_AUTO_TEST_SUITE(ListCopyMove)

BOOST_AUTO_TEST_CASE(CopyConstruct)
{
  burukov::List< int > src;
  src.pushFront(3);
  src.pushFront(2);
  src.pushFront(1);
  burukov::List< int > dst(src);
  BOOST_CHECK_EQUAL(dst.size(), 3);
  burukov::LIter< int > it = dst.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  src.popFront();
  BOOST_CHECK_EQUAL(dst.size(), 3);
}

BOOST_AUTO_TEST_CASE(CopyConstructEmpty)
{
  burukov::List< int > src;
  burukov::List< int > dst(src);
  BOOST_CHECK(dst.empty());
}

BOOST_AUTO_TEST_CASE(MoveConstruct)
{
  burukov::List< int > src;
  src.pushFront(2);
  src.pushFront(1);
  burukov::List< int > dst(std::move(src));
  BOOST_CHECK_EQUAL(dst.size(), 2);
  BOOST_CHECK_EQUAL(dst.front(), 1);
  BOOST_CHECK(src.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssign)
{
  burukov::List< int > src;
  src.pushFront(2);
  src.pushFront(1);
  burukov::List< int > dst;
  dst.pushFront(99);
  dst = src;
  BOOST_CHECK_EQUAL(dst.size(), 2);
  BOOST_CHECK_EQUAL(dst.front(), 1);
}

BOOST_AUTO_TEST_CASE(CopyAssignSelf)
{
  burukov::List< int > list;
  list.pushFront(1);
  list = list;
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_CASE(MoveAssign)
{
  burukov::List< int > src;
  src.pushFront(2);
  src.pushFront(1);
  burukov::List< int > dst;
  dst.pushFront(99);
  dst = std::move(src);
  BOOST_CHECK_EQUAL(dst.size(), 2);
  BOOST_CHECK_EQUAL(dst.front(), 1);
  BOOST_CHECK(src.empty());
}

BOOST_AUTO_TEST_CASE(SwapLists)
{
  burukov::List< int > a;
  a.pushFront(2);
  a.pushFront(1);
  burukov::List< int > b;
  b.pushFront(30);
  a.swap(b);
  BOOST_CHECK_EQUAL(a.size(), 1);
  BOOST_CHECK_EQUAL(a.front(), 30);
  BOOST_CHECK_EQUAL(b.size(), 2);
  BOOST_CHECK_EQUAL(b.front(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ListIterators)

BOOST_AUTO_TEST_CASE(ConstIter)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  const burukov::List< int > &cref = list;
  burukov::LCIter< int > it = cref.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK(it == cref.cend());
}

BOOST_AUTO_TEST_CASE(ConstFront)
{
  burukov::List< int > list;
  list.pushFront(42);
  const burukov::List< int > &cref = list;
  BOOST_CHECK_EQUAL(cref.front(), 42);
}

BOOST_AUTO_TEST_CASE(PostfixIncrement)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  burukov::LIter< int > it = list.begin();
  burukov::LIter< int > old = it++;
  BOOST_CHECK_EQUAL(*old, 1);
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(ConstPostfixIncrement)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  burukov::LCIter< int > it = list.cbegin();
  burukov::LCIter< int > old = it++;
  BOOST_CHECK_EQUAL(*old, 1);
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(ArrowOperator)
{
  burukov::List< std::pair< int, int > > list;
  list.pushFront(std::make_pair(1, 2));
  burukov::LIter< std::pair< int, int > > it = list.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second, 2);
}

BOOST_AUTO_TEST_CASE(InsertAfterChain)
{
  burukov::List< int > list;
  list.pushFront(1);
  burukov::LIter< int > tail = list.begin();
  for (int i = 2; i <= 5; ++i)
  {
    tail = list.insertAfter(tail, i);
  }
  BOOST_CHECK_EQUAL(list.size(), 5);
  burukov::LIter< int > it = list.begin();
  for (int i = 1; i <= 5; ++i)
  {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }
}

BOOST_AUTO_TEST_SUITE_END()
