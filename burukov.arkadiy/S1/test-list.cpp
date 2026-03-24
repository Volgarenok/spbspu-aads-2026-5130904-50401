#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(DefConstructorTest)
{
  burukov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  burukov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  burukov::List< int > list2(list1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  list1.pushFront(0);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  burukov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  burukov::List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK(list1.begin() == list1.end());
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list2.pushFront(0);
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK_EQUAL(list2.size(), 3);
}

BOOST_AUTO_TEST_CASE(DestructEmptyListTest)
{
  burukov::List< int > list;
}

BOOST_AUTO_TEST_CASE(DestructSingleElemTest)
{
  burukov::List< int > list;
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_CASE(DestructMultElemTest)
{
  burukov::List< int > list;
  for (int i = 0; i < 10; ++i)
  {
    list.pushFront(i);
  }
  BOOST_CHECK_EQUAL(list.size(), 10);
}

BOOST_AUTO_TEST_CASE(DestructAfterCopyTest)
{
  burukov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  burukov::List< int > list2(list1);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(list2.size(), 2);
}

BOOST_AUTO_TEST_CASE(DestructAfterMoveTest)
{
  burukov::List< int > list1;
  list1.pushFront(52);
  burukov::List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK_EQUAL(list2.size(), 1);
}

BOOST_AUTO_TEST_CASE(CopyAssignTest)
{
  burukov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  burukov::List< int > list2;
  list2.pushFront(3);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list1 = list1;
  BOOST_CHECK_EQUAL(list1.size(), 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignTest)
{
  burukov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  burukov::List< int > list2;
  list2.pushFront(99);
  list2 = std::move(list1);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list2.front(), 1);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(FrontTest)
{
  burukov::List< int > list;
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  list.pushFront(2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  const burukov::List< int > &cref = list;
  BOOST_CHECK_EQUAL(cref.front(), 2);
}

BOOST_AUTO_TEST_CASE(PushFrontTest)
{
  burukov::List< int > list;
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  list.pushFront(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(PopFrontTest)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 3);
  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(InsertAfterTest)
{
  burukov::List< int > list;
  list.pushFront(1);
  auto it = list.begin();
  auto ins = list.insertAfter(it, 3);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*ins, 3);
  list.insertAfter(it, 2);
  BOOST_CHECK_EQUAL(list.size(), 3);
  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
}

BOOST_AUTO_TEST_CASE(InsertAfterChainTest)
{
  burukov::List< int > list;
  list.pushFront(1);
  auto tail = list.begin();
  for (int i = 2; i <= 5; ++i)
  {
    tail = list.insertAfter(tail, i);
  }
  BOOST_CHECK_EQUAL(list.size(), 5);
  auto it = list.begin();
  for (int i = 1; i <= 5; ++i)
  {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(EraseAfterTest)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  auto it = list.begin();
  auto next = list.eraseAfter(it);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*next, 3);
  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  list.eraseAfter(list.begin());
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  burukov::List< int > list;
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.pushFront(4);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 4);
}

BOOST_AUTO_TEST_CASE(SizeAndEmptyTest)
{
  burukov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.empty());
  list.pushFront(1);
  list.pushFront(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK(!list.empty());
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  burukov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);
  burukov::List< int > list2;
  list2.pushFront(3);
  list1.swap(list2);
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK_EQUAL(list1.front(), 3);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list2.front(), 1);
}

BOOST_AUTO_TEST_CASE(IteratorTest)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  auto old = it++;
  BOOST_CHECK_EQUAL(*old, 1);
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(ConstIterTest)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  const burukov::List< int > &constList = list;
  auto cit = constList.cbegin();
  BOOST_CHECK_EQUAL(*cit, 1);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 2);
  ++cit;
  BOOST_CHECK(cit == constList.cend());
  BOOST_CHECK(constList.cbegin() != constList.cend());
  auto cit2 = constList.cbegin();
  BOOST_CHECK_EQUAL(*cit2, 1);
  burukov::List< int > empty;
  BOOST_CHECK(empty.cbegin() == empty.cend());
}

BOOST_AUTO_TEST_CASE(ArrowOperatorTest)
{
  burukov::List< std::pair< int, int > > list;
  list.pushFront(std::make_pair(1, 2));
  auto it = list.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second, 2);
}

BOOST_AUTO_TEST_SUITE_END()
