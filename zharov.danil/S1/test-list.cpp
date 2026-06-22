#define BOOST_TEST_MODULE S1
#include <functional>
#include <boost/test/included/unit_test.hpp>
#include <list.hpp>

BOOST_AUTO_TEST_CASE(DefaultListConstructorTest)
{
  zharov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(CopyListConstructorTest)
{
  zharov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);
  BOOST_CHECK_EQUAL(list1.front(), 1);
  BOOST_CHECK_EQUAL(list1.back(), 3);
  zharov::List< int > list2 = zharov::List< int >(list1);
  zharov::LIter< int > it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(MoveListConstructorTest)
{
  zharov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);
  BOOST_CHECK_EQUAL(list1.front(), 1);
  BOOST_CHECK_EQUAL(list1.back(), 3);
  zharov::List< int > list2 = zharov::List< int >(std::move(list1));
  zharov::LIter< int > it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(DestructorTest)
{
  zharov::List< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.pushBack(i);
  }
  BOOST_CHECK_EQUAL(list.size(), 10);
}

BOOST_AUTO_TEST_CASE(CopyOperatorTest)
{
  zharov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(3);
  BOOST_CHECK_EQUAL(list1.front(), 1);
  BOOST_CHECK_EQUAL(list1.back(), 3);
  zharov::List< int > list2;
  list2.pushBack(11);
  list2.pushBack(33);
  BOOST_CHECK_EQUAL(list2.front(), 11);
  BOOST_CHECK_EQUAL(list2.back(), 33);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.front(), 1);
  BOOST_CHECK_EQUAL(list2.back(), 3);
}

BOOST_AUTO_TEST_CASE(MoveOperatorTest)
{
  zharov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(3);
  BOOST_CHECK_EQUAL(list1.front(), 1);
  BOOST_CHECK_EQUAL(list1.back(), 3);
  zharov::List< int > list2;
  list2.pushBack(11);
  list2.pushBack(33);
  BOOST_CHECK_EQUAL(list2.front(), 11);
  BOOST_CHECK_EQUAL(list2.back(), 33);
  list2 = std::move(list1);
  BOOST_CHECK_EQUAL(list2.front(), 1);
  BOOST_CHECK_EQUAL(list2.back(), 3);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(BeginTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  zharov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(ConstBeginTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  const zharov::List< int > const_list(list);
  zharov::LCIter< int > it = const_list.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(EndTest)
{
  zharov::List< int > list;
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(ConstEndTest)
{
  const zharov::List< int > list;
  BOOST_CHECK(list.cbegin() == list.cbegin());
}

BOOST_AUTO_TEST_CASE(FrontTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  const zharov::List< int > list2(list);
  BOOST_CHECK_EQUAL(list2.front(), 1);
}

BOOST_AUTO_TEST_CASE(BackTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(list.back(), 3);
  const zharov::List< int > list2(list);
  BOOST_CHECK_EQUAL(list2.back(), 3);
}

BOOST_AUTO_TEST_CASE(PushFrontTest)
{
  zharov::List< int > list;
  list.pushFront(1);
  list.pushFront(3);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 3);
}

BOOST_AUTO_TEST_CASE(PushBackTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_CASE(InsertTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  zharov::LIter< int > it = list.begin();
  list.insert(++it, 2);
  it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 2);
  BOOST_CHECK_EQUAL(*(++it), 3);
  BOOST_CHECK_EQUAL(list.size(), 3);
}

BOOST_AUTO_TEST_CASE(PopFrontTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.popFront();
  BOOST_CHECK_EQUAL(list.front(), 2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(PopBackTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.popBack();
  BOOST_CHECK_EQUAL(list.back(), 2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(EraseTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  zharov::LIter< int > it = list.begin();
  list.erase(++it);
  it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 3);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(SizeTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(list.size(), 3);
}

BOOST_AUTO_TEST_CASE(SpliceAllBeforeEndTest)
{
  zharov::List< int > dest;
  dest.pushBack(1);
  dest.pushBack(5);
  zharov::List< int > src;
  src.pushBack(2);
  src.pushBack(3);
  src.pushBack(4);
  dest.splice(dest.end(), src);
  BOOST_CHECK_EQUAL(src.size(), 0);
  BOOST_CHECK_EQUAL(dest.size(), 5);
  zharov::LIter< int > it = dest.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 5);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 4);
}

BOOST_AUTO_TEST_CASE(SpliceAllBeforePosTest)
{
  zharov::List< int > dest;
  dest.pushBack(1);
  dest.pushBack(5);
  zharov::List< int > src;
  src.pushBack(2);
  src.pushBack(3);
  zharov::LIter< int > pos = dest.begin();
  ++pos;
  dest.splice(pos, src);
  BOOST_CHECK_EQUAL(src.size(), 0);
  BOOST_CHECK_EQUAL(dest.size(), 4);
  zharov::LIter< int > it = dest.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 5);
}

BOOST_AUTO_TEST_CASE(SpliceAllEmptySrcTest)
{
  zharov::List< int > dest;
  dest.pushBack(1);
  zharov::List< int > src;
  dest.splice(dest.end(), src);
  BOOST_CHECK_EQUAL(dest.size(), 1);
  BOOST_CHECK_EQUAL(src.size(), 0);
}

BOOST_AUTO_TEST_CASE(SortTest)
{
  zharov::List< int > list;
  list.pushBack(3);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(2);
  list.sort();
  zharov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 4);
}

BOOST_AUTO_TEST_CASE(SortAlreadySortedTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.sort();
  zharov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 3);
}

BOOST_AUTO_TEST_CASE(SortSingleTest)
{
  zharov::List< int > list;
  list.pushBack(42);
  list.sort();
  BOOST_CHECK_EQUAL(list.front(), 42);
}

BOOST_AUTO_TEST_CASE(PartitionTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(2);
  list.pushBack(5);
  list.pushBack(3);
  zharov::LIter< int > mid = list.partition(
    [](int x)
    {
      return x < 4;
    });
  BOOST_CHECK_EQUAL(list.size(), 5);
  BOOST_CHECK_EQUAL(*mid, 4);
  zharov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 4);
  BOOST_CHECK_EQUAL(*it++, 5);
}

BOOST_AUTO_TEST_CASE(PartitionAllTrueTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  zharov::LIter< int > mid = list.partition(
    [](int x)
    {
      return x > 0;
    });
  BOOST_CHECK(mid == list.end());
  BOOST_CHECK_EQUAL(list.size(), 3);
}

BOOST_AUTO_TEST_CASE(PartitionAllFalseTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  zharov::LIter< int > mid = list.partition(
    [](int x)
    {
      return x > 10;
    });
  BOOST_CHECK(mid == list.begin());
  BOOST_CHECK_EQUAL(list.size(), 3);
}

BOOST_AUTO_TEST_CASE(MergeTest)
{
  zharov::List< int > a;
  a.pushBack(1);
  a.pushBack(3);
  zharov::List< int > b;
  b.pushBack(2);
  b.pushBack(4);
  a.merge(b);
  BOOST_CHECK_EQUAL(b.size(), 0);
  BOOST_CHECK_EQUAL(a.size(), 4);
  zharov::LIter< int > it = a.begin();
  BOOST_CHECK_EQUAL(*it++, 1);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 4);
}

BOOST_AUTO_TEST_CASE(MergeWithCompTest)
{
  zharov::List< int > a;
  a.pushBack(3);
  a.pushBack(1);
  zharov::List< int > b;
  b.pushBack(4);
  b.pushBack(2);
  a.merge(b, std::greater< int >{});
  BOOST_CHECK_EQUAL(a.size(), 4);
  zharov::LIter< int > it = a.begin();
  BOOST_CHECK_EQUAL(*it++, 4);
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 1);
}

BOOST_AUTO_TEST_CASE(SortWithCompTest)
{
  zharov::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  list.pushBack(2);
  list.sort(std::greater< int >{});
  zharov::LIter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it++, 3);
  BOOST_CHECK_EQUAL(*it++, 2);
  BOOST_CHECK_EQUAL(*it++, 1);
}

BOOST_AUTO_TEST_CASE(SpliceRangeTest)
{
  zharov::List< int > dest;
  dest.pushBack(1);
  dest.pushBack(5);
  zharov::List< int > src;
  src.pushBack(10);
  src.pushBack(2);
  src.pushBack(3);
  src.pushBack(4);
  src.pushBack(20);
  zharov::LIter< int > first = src.begin();
  ++first;
  zharov::LIter< int > last = first;
  ++last;
  ++last;
  ++last;
  zharov::LIter< int > pos = dest.begin();
  ++pos;
  dest.splice(pos, src, first, last);
  BOOST_CHECK_EQUAL(dest.size(), 5);
  BOOST_CHECK_EQUAL(src.size(), 2);
  zharov::LIter< int > d = dest.begin();
  BOOST_CHECK_EQUAL(*d++, 1);
  BOOST_CHECK_EQUAL(*d++, 2);
  BOOST_CHECK_EQUAL(*d++, 3);
  BOOST_CHECK_EQUAL(*d++, 4);
  BOOST_CHECK_EQUAL(*d++, 5);
  zharov::LIter< int > s = src.begin();
  BOOST_CHECK_EQUAL(*s++, 10);
  BOOST_CHECK_EQUAL(*s++, 20);
}

BOOST_AUTO_TEST_CASE(SpliceRangeToEndTest)
{
  zharov::List< int > dest;
  dest.pushBack(1);
  zharov::List< int > src;
  src.pushBack(2);
  src.pushBack(3);
  src.pushBack(4);
  zharov::LIter< int > first = src.begin();
  ++first;
  dest.splice(dest.end(), src, first, src.end());
  BOOST_CHECK_EQUAL(dest.size(), 3);
  BOOST_CHECK_EQUAL(src.size(), 1);
  zharov::LIter< int > d = dest.begin();
  BOOST_CHECK_EQUAL(*d++, 1);
  BOOST_CHECK_EQUAL(*d++, 3);
  BOOST_CHECK_EQUAL(*d++, 4);
}

BOOST_AUTO_TEST_CASE(SpliceRangeEmptyTest)
{
  zharov::List< int > dest;
  dest.pushBack(1);
  zharov::List< int > src;
  src.pushBack(2);
  zharov::LIter< int > it = src.begin();
  dest.splice(dest.end(), src, it, it);
  BOOST_CHECK_EQUAL(dest.size(), 1);
  BOOST_CHECK_EQUAL(src.size(), 1);
}

BOOST_AUTO_TEST_CASE(SpliceSingleTest)
{
  zharov::List< int > dest;
  dest.pushBack(1);
  dest.pushBack(5);
  zharov::List< int > src;
  src.pushBack(10);
  src.pushBack(3);
  src.pushBack(20);
  zharov::LIter< int > it = src.begin();
  ++it;
  zharov::LIter< int > pos = dest.begin();
  ++pos;
  dest.splice(pos, src, it);
  BOOST_CHECK_EQUAL(dest.size(), 3);
  BOOST_CHECK_EQUAL(src.size(), 2);
  zharov::LIter< int > d = dest.begin();
  BOOST_CHECK_EQUAL(*d++, 1);
  BOOST_CHECK_EQUAL(*d++, 3);
  BOOST_CHECK_EQUAL(*d++, 5);
  zharov::LIter< int > s = src.begin();
  BOOST_CHECK_EQUAL(*s++, 10);
  BOOST_CHECK_EQUAL(*s++, 20);
}
