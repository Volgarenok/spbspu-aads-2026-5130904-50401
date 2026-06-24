#define BOOST_TEST_MODULE ListTests

#include <boost/test/included/unit_test.hpp>
#include <stdexcept>

#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  burukov::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  burukov::List< int > first;
  first.pushFront(3);
  first.pushFront(2);
  first.pushFront(1);

  burukov::List< int > second(first);
  BOOST_CHECK_EQUAL(second.size(), 3);
  BOOST_CHECK(!second.empty());

  auto it = second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);

  first.pushFront(0);
  BOOST_CHECK_EQUAL(second.size(), 3);
}

BOOST_AUTO_TEST_CASE(CopyConstructorEmpty)
{
  burukov::List< int > first;
  burukov::List< int > second(first);
  BOOST_CHECK(second.empty());
  BOOST_CHECK_EQUAL(second.size(), 0);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  burukov::List< int > first;
  first.pushFront(2);
  first.pushFront(1);

  burukov::List< int > second(std::move(first));
  BOOST_CHECK(first.empty());
  BOOST_CHECK_EQUAL(first.size(), 0);
  BOOST_CHECK_EQUAL(second.size(), 2);

  auto it = second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructorEmpty)
{
  burukov::List< int > first;
  burukov::List< int > second(std::move(first));
  BOOST_CHECK(second.empty());
  BOOST_CHECK_EQUAL(second.size(), 0);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  burukov::List< int > first;
  burukov::List< int > second;
  first.pushFront(3);
  first.pushFront(2);
  first.pushFront(1);

  second = first;
  BOOST_CHECK_EQUAL(second.size(), 3);

  auto it = second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(CopyAssignmentSelf)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  burukov::List< int > &b = list;
  list = b;
  BOOST_CHECK_EQUAL(list.size(), 3);

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  burukov::List< int > first;
  burukov::List< int > second;
  first.pushFront(2);
  first.pushFront(1);

  second = std::move(first);
  BOOST_CHECK(first.empty());
  BOOST_CHECK_EQUAL(second.size(), 2);

  auto it = second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentSelf)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  burukov::List< int > *ptr = &list;
  *ptr = std::move(list);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(Front)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  list.front() = 10;
  BOOST_CHECK_EQUAL(list.front(), 10);
}

BOOST_AUTO_TEST_CASE(FrontConst)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  const burukov::List< int > &constRef = list;
  BOOST_CHECK_EQUAL(constRef.front(), 1);
}

BOOST_AUTO_TEST_CASE(FrontEmptyThrows)
{
  burukov::List< int > list;
  BOOST_CHECK_THROW(list.front(), std::logic_error);
  BOOST_CHECK_THROW(static_cast< const burukov::List< int > & >(list).front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(PushFront)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(PushFrontMove)
{
  burukov::List< std::string > list;
  std::string s = "hello";
  list.pushFront(std::move(s));
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(list.front(), "hello");
}

BOOST_AUTO_TEST_CASE(PushBack)
{
  burukov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  list.popFront();
  BOOST_CHECK_EQUAL(list.front(), 2);
  BOOST_CHECK_EQUAL(list.size(), 2);
  list.popFront();
  BOOST_CHECK_EQUAL(list.front(), 3);
  BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_CASE(PopFrontEmpty)
{
  burukov::List< int > list;
  list.popFront();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(InsertAfter)
{
  burukov::List< int > list;
  list.pushFront(1);
  auto it = list.begin();
  list.insertAfter(it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(InsertAfterMultiple)
{
  burukov::List< int > list;
  list.pushFront(1);
  auto it = list.begin();
  it = list.insertAfter(it, 2);
  it = list.insertAfter(it, 3);
  it = list.insertAfter(it, 4);
  BOOST_CHECK_EQUAL(list.size(), 4);

  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK_EQUAL(*check, 4);
}

BOOST_AUTO_TEST_CASE(InsertAfterEnd)
{
  burukov::List< int > list;
  list.pushFront(1);
  auto result = list.insertAfter(list.end(), 2);
  BOOST_CHECK(result == list.end());
  BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_CASE(EraseAfter)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  auto it = list.begin();
  list.eraseAfter(it);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(EraseAfterLast)
{
  burukov::List< int > list;
  list.pushFront(2);
  list.pushFront(1);
  auto it = list.begin();
  ++it;
  auto result = list.eraseAfter(it);
  BOOST_CHECK(result == list.end());
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(EraseAfterEnd)
{
  burukov::List< int > list;
  list.pushFront(1);
  auto result = list.eraseAfter(list.end());
  BOOST_CHECK(result == list.end());
  BOOST_CHECK_EQUAL(list.size(), 1);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(ClearEmpty)
{
  burukov::List< int > list;
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(Swap)
{
  burukov::List< int > first;
  burukov::List< int > second;
  first.pushFront(2);
  first.pushFront(1);
  second.pushFront(4);
  second.pushFront(3);
  first.swap(second);
  BOOST_CHECK_EQUAL(first.size(), 2);
  BOOST_CHECK_EQUAL(second.size(), 2);

  auto it = first.begin();
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  it = second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(IteratorIncrement)
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

BOOST_AUTO_TEST_CASE(ConstIterator)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  const burukov::List< int > &constRef = list;
  auto it = constRef.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK(it == constRef.cend());
}

BOOST_AUTO_TEST_CASE(SpliceAfterWholeList)
{
  burukov::List< int > list1;
  burukov::List< int > list2;
  list1.pushFront(3);
  list1.pushFront(2);
  list1.pushFront(1);
  list2.pushFront(6);
  list2.pushFront(5);
  list2.pushFront(4);

  list1.spliceAfter(list1.begin(), list2);

  BOOST_CHECK_EQUAL(list1.size(), 6);
  BOOST_CHECK(list2.empty());

  auto it = list1.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(SpliceAfterWholeListEmpty)
{
  burukov::List< int > list1;
  burukov::List< int > list2;
  list1.pushFront(1);
  list1.spliceAfter(list1.begin(), list2);
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE(SpliceAfterWholeListSelf)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  list.spliceAfter(list.begin(), list);
  BOOST_CHECK_EQUAL(list.size(), 3);
}

BOOST_AUTO_TEST_CASE(SpliceAfterSingleElement)
{
  burukov::List< int > list1;
  burukov::List< int > list2;
  list1.pushFront(3);
  list1.pushFront(1);
  list2.pushFront(4);
  list2.pushFront(2);

  auto it = list1.begin();
  ++it;
  auto it2 = list2.begin();

  list1.spliceAfter(it, list2, it2);

  BOOST_CHECK_EQUAL(list1.size(), 3);
  BOOST_CHECK_EQUAL(list2.size(), 1);

  auto check = list1.begin();
  BOOST_CHECK_EQUAL(*check, 1);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK_EQUAL(*check, 4);
}

BOOST_AUTO_TEST_CASE(SpliceAfterRange)
{
  burukov::List< int > list1;
  burukov::List< int > list2;
  list1.pushFront(5);
  list1.pushFront(1);
  list2.pushFront(6);
  list2.pushFront(4);
  list2.pushFront(3);
  list2.pushFront(2);

  auto pos = list1.begin();
  ++pos;
  auto first = list2.begin();
  ++first;
  auto last = list2.end();

  list1.spliceAfter(pos, list2, first, last);

  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(list2.size(), 2);

  auto it = list1.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
}

BOOST_AUTO_TEST_CASE(SpliceAfterRangeEmpty)
{
  burukov::List< int > list1;
  burukov::List< int > list2;
  list1.pushFront(1);
  list2.pushFront(2);
  auto it = list2.begin();
  list1.spliceAfter(list1.begin(), list2, it, it);
  BOOST_CHECK_EQUAL(list1.size(), 1);
  BOOST_CHECK_EQUAL(list2.size(), 1);
}

BOOST_AUTO_TEST_CASE(Sort)
{
  burukov::List< int > list;
  list.pushFront(5);
  list.pushFront(1);
  list.pushFront(3);
  list.pushFront(4);
  list.pushFront(2);
  list.sort();

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
}

BOOST_AUTO_TEST_CASE(SortDescending)
{
  burukov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);
  list.pushFront(4);
  list.pushFront(5);
  list.sort(std::greater< int >());

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(SortEmpty)
{
  burukov::List< int > list;
  list.sort();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(SortSingleElement)
{
  burukov::List< int > list;
  list.pushFront(42);
  list.sort();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 42);
}

BOOST_AUTO_TEST_CASE(Merge)
{
  burukov::List< int > first;
  burukov::List< int > second;
  first.pushFront(3);
  first.pushFront(1);
  second.pushFront(4);
  second.pushFront(2);
  first.merge(second);

  auto it = first.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  BOOST_CHECK(second.empty());
  BOOST_CHECK_EQUAL(first.size(), 4);
}

BOOST_AUTO_TEST_CASE(MergeWithComparator)
{
  burukov::List< int > first;
  burukov::List< int > second;
  first.pushFront(1);
  first.pushFront(3);
  second.pushFront(2);
  second.pushFront(4);
  first.merge(second, std::greater< int >());

  auto it = first.begin();
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(MergeEmpty)
{
  burukov::List< int > first;
  burukov::List< int > second;
  first.pushFront(1);
  first.merge(second);
  BOOST_CHECK_EQUAL(first.size(), 1);
  BOOST_CHECK(second.empty());
}

BOOST_AUTO_TEST_CASE(MergeSelf)
{
  burukov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);
  list.merge(list);
  BOOST_CHECK_EQUAL(list.size(), 3);
}

BOOST_AUTO_TEST_CASE(Partition)
{
  burukov::List< int > list;
  list.pushFront(5);
  list.pushFront(2);
  list.pushFront(4);
  list.pushFront(1);
  list.pushFront(3);
  list.partition([](int value)
    {
      return value % 2 == 0;
    });

  bool oddFound = false;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    if (*it % 2 != 0)
    {
      oddFound = true;
    }
    if (oddFound)
    {
      BOOST_CHECK(*it % 2 != 0);
    }
  }
}

BOOST_AUTO_TEST_CASE(PartitionEmpty)
{
  burukov::List< int > list;
  auto result = list.partition([](int value)
    {
      return value > 0;
    });
  BOOST_CHECK(result == list.end());
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(PartitionAllTrue)
{
  burukov::List< int > list;
  list.pushFront(5);
  list.pushFront(3);
  list.pushFront(1);
  auto result = list.partition([](int value)
    {
      return value % 2 == 1;
    });
  BOOST_CHECK(result == list.begin());

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
}

BOOST_AUTO_TEST_CASE(PartitionAllFalse)
{
  burukov::List< int > list;
  list.pushFront(4);
  list.pushFront(2);
  list.pushFront(6);
  auto result = list.partition([](int value)
    {
      return value % 2 == 1;
    });
  BOOST_CHECK(result == list.begin());

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 6);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
}

BOOST_AUTO_TEST_CASE(StressTest)
{
  burukov::List< int > list;
  for (int i = 100; i > 0; --i)
  {
    list.pushFront(i);
  }
  BOOST_CHECK_EQUAL(list.size(), 100);
  list.sort();

  auto it = list.begin();
  for (int i = 1; i <= 100; ++i, ++it)
  {
    BOOST_CHECK_EQUAL(*it, i);
  }
}

BOOST_AUTO_TEST_CASE(MultipleOperations)
{
  burukov::List< int > list;
  list.pushFront(10);
  list.pushFront(20);
  list.pushFront(30);
  list.popFront();
  BOOST_CHECK_EQUAL(list.front(), 20);
  BOOST_CHECK_EQUAL(list.size(), 2);
  list.insertAfter(list.begin(), 25);

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 25);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(ChainOperations)
{
  burukov::List< int > list1;
  burukov::List< int > list2;
  for (int i = 1; i <= 10; ++i)
  {
    list1.pushFront(i);
  }
  for (int i = 11; i <= 20; ++i)
  {
    list2.pushFront(i);
  }
  list1.sort();
  list2.sort();
  list1.merge(list2);

  auto it = list1.begin();
  for (int i = 1; i <= 20; ++i, ++it)
  {
    BOOST_CHECK_EQUAL(*it, i);
  }
}

BOOST_AUTO_TEST_SUITE_END()
