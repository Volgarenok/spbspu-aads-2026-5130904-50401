#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(DefConstructorTest)
{
  sedov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  sedov::List< int > list1;
  list1.pushBack(1);
  sedov::List< int > list2(list1);
  list1.pushBack(2);
  BOOST_CHECK_EQUAL(list2.size(), 1);
  list2.pushBack(3);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  sedov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  sedov::List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK(list1.begin() == list1.end());
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list2.pushBack(3);
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK_EQUAL(list2.size(), 3);
}

BOOST_AUTO_TEST_CASE(DestructEmptyListTest)
{
  sedov::List< int > list;
}

BOOST_AUTO_TEST_CASE(DestructSingleElemTest)
{
  sedov::List< int > list;
  list.pushBack(1);
  BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_CASE(DestructMultElemTest)
{
  sedov::List< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.pushBack(i);
  }
  BOOST_CHECK_EQUAL(list.size(), 10);
}

BOOST_AUTO_TEST_CASE(DestructAfterCopyTest)
{
  sedov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  sedov::List< int > list2(list1);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(list2.size(), 2);
}

BOOST_AUTO_TEST_CASE(DestructAfterMoveTest)
{
  sedov::List< int > list1;
  list1.pushBack(52);
  sedov::List< int > list2(std::move(list1));
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK_EQUAL(list2.size(), 1);
}

BOOST_AUTO_TEST_CASE(CopyAssignTest)
{
  sedov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  sedov::List< int > list2;
  list2.pushBack(3);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(FrontAndBackTest)
{
  sedov::List< int > list;
  list.pushBack(1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
  list.pushBack(2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 2);
  list.pushFront(0);
  BOOST_CHECK_EQUAL(list.front(), 0);
  BOOST_CHECK_EQUAL(list.back(), 2);
}

BOOST_AUTO_TEST_CASE(PushFrontTest)
{
  sedov::List< int > list;
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
  list.pushFront(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  BOOST_CHECK_EQUAL(list.back(), 1);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(PushBackTest)
{
  sedov::List< int > list;
  list.pushBack(1);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
  list.pushBack(2);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK(it == list.end());
  list.clear();
  list.pushBack(4);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 4);
}

BOOST_AUTO_TEST_CASE(InsertTest)
{
  sedov::List< int > list;
  auto it = list.insert(list.end(), 1);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
  it = list.insert(list.begin(), 2);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  auto pos = list.begin();
  ++pos;
  it = list.insert(pos, 3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(*it, 3);
  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK_EQUAL(*check, 1);
}

BOOST_AUTO_TEST_CASE(PopFrontTest)
{
  sedov::List< int > list;
  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.pushBack(1);
  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.popFront();
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(PopBackTest)
{
  sedov::List< int > list;
  list.popBack();
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.pushBack(1);
  list.popBack();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.popBack();
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 2);
}

BOOST_AUTO_TEST_CASE(EraseTest)
{
  sedov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  auto pos = list.begin();
  ++pos;
  auto next = list.erase(pos);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*next, 3);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  next = list.erase(list.begin());
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*next, 3);
  BOOST_CHECK_EQUAL(list.front(), 3);
  next = list.erase(list.begin());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(next == list.end());
  next = list.erase(list.end());
  BOOST_CHECK(next == list.end());
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  sedov::List< int > list;
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.pushBack(4);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 4);
}

BOOST_AUTO_TEST_CASE(SizeTest)
{
  sedov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  list.pushBack(1);
  list.pushBack(2);
  BOOST_CHECK_EQUAL(list.size(), 2);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  sedov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  sedov::List< int > list2;
  list2.pushBack(3);
  list1.swap(list2);
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK_EQUAL(list1.front(), 3);
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(list2.front(), 1);
  BOOST_CHECK_EQUAL(list2.back(), 2);
}

BOOST_AUTO_TEST_CASE(ConstIterTest)
{
  sedov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  const sedov::List< int > & constList = list;
  auto cit = constList.cbegin();
  BOOST_CHECK_EQUAL(*cit, 1);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 2);
  ++cit;
  BOOST_CHECK(cit == constList.cend());
  BOOST_CHECK(constList.cbegin() != constList.cend());
  auto cit2 = constList.cbegin();
  BOOST_CHECK_EQUAL(*cit2, 1);
  sedov::List< int > empty;
  BOOST_CHECK(empty.cbegin() == empty.cend());
}

BOOST_AUTO_TEST_CASE(SpliceAllTest)
{
  sedov::List< int > a;
  a.pushBack(1);
  a.pushBack(2);
  sedov::List< int > b;
  b.pushBack(10);
  b.pushBack(20);
  b.pushBack(30);
  auto pos = a.begin();
  ++pos;
  a.splice(pos, b);
  BOOST_CHECK_EQUAL(a.size(), 5);
  BOOST_CHECK_EQUAL(b.size(), 0);
  auto it = a.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK(it == a.end());
}

BOOST_AUTO_TEST_CASE(SpliceSingleTest)
{
  sedov::List< int > a;
  a.pushBack(1);
  a.pushBack(3);
  sedov::List< int > b;
  b.pushBack(10);
  b.pushBack(20);
  b.pushBack(30);
  auto pos = a.begin();
  ++pos;
  auto it = b.begin();
  ++it;
  a.splice(pos, b, it);
  BOOST_CHECK_EQUAL(a.size(), 3);
  BOOST_CHECK_EQUAL(b.size(), 2);
  auto check = a.begin();
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 20);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK(check == a.end());
}

BOOST_AUTO_TEST_CASE(SpliceRangeTest)
{
  sedov::List< int > a;
  a.pushBack(1);
  a.pushBack(5);
  sedov::List< int > b;
  b.pushBack(10);
  b.pushBack(20);
  b.pushBack(30);
  b.pushBack(40);
  auto pos = a.begin();
  ++pos;
  auto first = b.begin();
  ++first;
  auto last = b.begin();
  ++last;
  ++last;
  ++last;
  a.splice(pos, b, first, last);
  BOOST_CHECK_EQUAL(a.size(), 4);
  BOOST_CHECK_EQUAL(b.size(), 2);
  auto check = a.begin();
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 20);
  ++check;
  BOOST_CHECK_EQUAL(*check, 30);
  ++check;
  BOOST_CHECK_EQUAL(*check, 5);
  ++check;
  BOOST_CHECK(check == a.end());
}

BOOST_AUTO_TEST_CASE(MergeSortedTest)
{
  sedov::List< int > a;
  a.pushBack(1);
  a.pushBack(3);
  a.pushBack(5);
  sedov::List< int > b;
  b.pushBack(2);
  b.pushBack(4);
  b.pushBack(6);
  a.merge(b);
  BOOST_CHECK_EQUAL(a.size(), 6);
  BOOST_CHECK_EQUAL(b.size(), 0);
  auto it = a.begin();
  for (int i = 1; i <= 6; ++i)
  {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }
  BOOST_CHECK(it == a.end());
}

BOOST_AUTO_TEST_CASE(SortTest)
{
  sedov::List< int > list;
  list.pushBack(5);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(2);
  list.pushBack(3);
  list.sort();
  BOOST_CHECK_EQUAL(list.size(), 5);
  auto it = list.begin();
  for (int i = 1; i <= 5; ++i)
  {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }
  BOOST_CHECK(it == list.end());
}

bool isEven(int x)
{
  return x % 2 == 0;
}

BOOST_AUTO_TEST_CASE(PartitionTest)
{
  sedov::List< int > list;
  list.pushBack(3);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(1);
  list.pushBack(5);
  list.pushBack(9);
  list.pushBack(2);
  list.pushBack(6);
  auto it = list.partition(isEven);
  BOOST_CHECK(it != list.end());
  BOOST_CHECK_EQUAL(*it, 3);
  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 4);
  ++check;
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 6);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 5);
  ++check;
  BOOST_CHECK_EQUAL(*check, 9);
  ++check;
  BOOST_CHECK(check == list.end());
}

BOOST_AUTO_TEST_CASE(PartitionAllTrueTest)
{
  sedov::List< int > list;
  list.pushBack(2);
  list.pushBack(4);
  list.pushBack(6);
  auto it = list.partition(isEven);
  BOOST_CHECK(it == list.end());
  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 4);
  ++check;
  BOOST_CHECK_EQUAL(*check, 6);
  ++check;
  BOOST_CHECK(check == list.end());
}

BOOST_AUTO_TEST_SUITE_END()
