#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include <functional>
#include <initializer_list>
#include <list.hpp>
#include <stdexcept>

namespace kuz = kuznetsov;
BOOST_AUTO_TEST_SUITE(SpecialMethodsSuite)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  auto list = kuz::List< int >();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  auto original = kuz::List< int >();
  original.insert(original.cend(), 1);
  original.insert(original.cend(), 2);
  original.insert(original.cend(), 3);

  kuz::List< int > copy(original);

  BOOST_CHECK_EQUAL(copy.size(), 3);
  BOOST_CHECK(!copy.empty());

  auto it = copy.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(CopyConstructorEmpty)
{
  auto original = kuz::List< int >();
  kuz::List< int > copy(original);

  BOOST_CHECK(copy.empty());
  BOOST_CHECK_EQUAL(copy.size(), 0);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  auto original = kuz::List< int >();
  original.insert(original.cend(), 10);
  original.insert(original.cend(), 20);

  kuz::List< int > moved(std::move(original));

  BOOST_CHECK_EQUAL(moved.size(), 2);
  BOOST_CHECK(original.empty());
  BOOST_CHECK_EQUAL(original.size(), 0);

  auto it = moved.cbegin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(CopyAssignmentOperator)
{
  auto list1 = kuz::List< int >();
  list1.insert(list1.cend(), 100);
  list1.insert(list1.cend(), 200);

  auto list2 = kuz::List< int >();
  list2 = list1;

  BOOST_CHECK_EQUAL(list2.size(), 2);
  auto it = list2.cbegin();
  BOOST_CHECK_EQUAL(*it, 100);
  ++it;
  BOOST_CHECK_EQUAL(*it, 200);

  BOOST_CHECK_EQUAL(list1.size(), 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentOperator)
{
  auto list1 = kuz::List< int >();
  list1.insert(list1.cend(), 300);
  list1.insert(list1.cend(), 400);

  auto list2 = kuz::List< int >();
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK(list1.empty());

  auto it = list2.cbegin();
  BOOST_CHECK_EQUAL(*it, 300);
  ++it;
  BOOST_CHECK_EQUAL(*it, 400);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AddGetSuite)

BOOST_AUTO_TEST_CASE(InsertIntoEmptyList)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 42);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 42);
  BOOST_CHECK_EQUAL(list.back(), 42);
}

BOOST_AUTO_TEST_CASE(InsertAtEnd)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(InsertAtBegin)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);
  list.insert(list.cbegin(), 1);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(InsertInMiddle)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 3);

  auto it = list.cbegin();
  ++it;
  list.insert(it, 2);

  BOOST_CHECK_EQUAL(list.size(), 3);

  auto cit = list.cbegin();
  BOOST_CHECK_EQUAL(*cit, 1);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 2);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 3);
}

BOOST_AUTO_TEST_CASE(FrontNonConst)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 100);
  list.insert(list.cend(), 200);

  list.front() = 999;
  BOOST_CHECK_EQUAL(list.front(), 999);
}

BOOST_AUTO_TEST_CASE(BackNonConst)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 100);
  list.insert(list.cend(), 200);

  list.back() = 888;
  BOOST_CHECK_EQUAL(list.back(), 888);
}

BOOST_AUTO_TEST_CASE(FrontBackOnEmptyList)
{
  auto list = kuz::List< int >();
  BOOST_CHECK_THROW(list.front(), std::logic_error);
  BOOST_CHECK_THROW(list.back(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(IteratorDecrement)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  auto it = list.begin();
  --it;
  BOOST_CHECK_EQUAL(*it, 3);
  --it;
  BOOST_CHECK_EQUAL(*it, 2);
  --it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(IteratorComparison)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);

  auto it1 = list.begin();
  auto it2 = list.begin();
  auto it3 = list.begin();
  it3++;
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
}

BOOST_AUTO_TEST_CASE(ReverseIterators)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  auto rit = list.rbegin();
  BOOST_CHECK_EQUAL(*rit, 3);
  ++rit;
  BOOST_CHECK_EQUAL(*rit, 2);
  ++rit;
  BOOST_CHECK_EQUAL(*rit, 1);
}

BOOST_AUTO_TEST_CASE(ConstReverseIterators)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  auto rit = list.rcbegin();
  BOOST_CHECK_EQUAL(*rit, 3);
  ++rit;
  BOOST_CHECK_EQUAL(*rit, 2);
  ++rit;
  BOOST_CHECK_EQUAL(*rit, 1);
}

BOOST_AUTO_TEST_CASE(SizeAndEmpty)
{
  kuz::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.insert(list.cend(), 1);
  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.insert(list.cend(), 2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DeletionSuite)

BOOST_AUTO_TEST_CASE(EraseSingleElement)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  auto it = list.cbegin();
  ++it;
  list.erase(it);

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(EraseOnlyElement)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 42);

  list.erase(list.cbegin());

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(EraseOnEmptyList)
{
  auto list = kuz::List< int >();
  BOOST_CHECK_THROW(list.erase(list.cbegin()), std::logic_error);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  list.popFront();

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  list.popBack();

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.back(), 2);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  list.clear();

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(MultipleEraseOperations)
{
  auto list = kuz::List< int >();
  for (int i = 1; i <= 5; ++i) {
    list.insert(list.cend(), i);
  }

  list.popFront();
  list.popBack();
  list.erase(list.cbegin());

  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(EraseReturnsCorrectIterator)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  auto it = list.cbegin();
  ++it;
  auto nextIt = list.erase(it);

  BOOST_CHECK_EQUAL(*nextIt, 3);
}

BOOST_AUTO_TEST_CASE(CyclicStructureAfterOperations)
{
  auto list = kuz::List< int >();
  list.insert(list.cend(), 1);
  list.insert(list.cend(), 2);
  list.insert(list.cend(), 3);

  list.popFront();
  list.popBack();

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 2);
  BOOST_CHECK_EQUAL(list.back(), 2);
}
BOOST_AUTO_TEST_SUITE_END()

namespace kuznetsov {
  template< class T >
  List< T > makeList(std::initializer_list< T > il)
  {
    List< T > l;
    for (const auto& v: il) {
      l.insert(l.cend(), v);
    }
    return l;
  }

  template< class T >
  bool listEquals(const List< T >& l, std::initializer_list< T > expected)
  {
    if (l.size() != expected.size()) {
      return false;
    }
    if (l.empty()) {
      return true;
    }
    auto h = l.cbegin();
    auto it = h;
    auto eit = expected.begin();
    if (*it != *eit) {
      return false;
    }
    ++it;
    ++eit;
    while (it != h) {
      if (*it != *eit) {
        return false;
      }
      ++it;
      ++eit;
    }
    return true;
  }
}

BOOST_AUTO_TEST_SUITE(SpliceSuite)

BOOST_AUTO_TEST_CASE(SpliceWholeAtEnd)
{
  auto a = kuznetsov::makeList< int >({ 1, 2 });
  auto b = kuznetsov::makeList< int >({ 3, 4 });
  a.splice(a.cend(), b);
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3, 4 }));
  BOOST_CHECK(b.empty());
}

BOOST_AUTO_TEST_CASE(SpliceWholeAtBegin)
{
  auto a = kuznetsov::makeList< int >({ 3, 4 });
  auto b = kuznetsov::makeList< int >({ 1, 2 });
  a.splice(a.cbegin(), b);
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3, 4 }));
  BOOST_CHECK_EQUAL(a.front(), 1);
  BOOST_CHECK_EQUAL(a.back(), 4);
}

BOOST_AUTO_TEST_CASE(SpliceWholeInMiddle)
{
  auto a = kuznetsov::makeList< int >({ 1, 4 });
  auto b = kuznetsov::makeList< int >({ 2, 3 });
  auto pos = a.cbegin();
  ++pos;
  a.splice(pos, b);
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3, 4 }));
}

BOOST_AUTO_TEST_CASE(SpliceOneAtEnd)
{
  auto a = kuznetsov::makeList< int >({ 1, 2 });
  auto b = kuznetsov::makeList< int >({ 9, 10, 11 });
  auto bit = b.cbegin();
  ++bit;
  a.splice(a.cend(), b, bit);
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 10 }));
  BOOST_CHECK(kuznetsov::listEquals< int >(b, { 9, 11 }));
}

BOOST_AUTO_TEST_CASE(SpliceOneAtBegin)
{
  auto a = kuznetsov::makeList< int >({ 1, 2 });
  auto b = kuznetsov::makeList< int >({ 9, 10 });
  a.splice(a.cbegin(), b, b.cbegin());
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 9, 1, 2 }));
  BOOST_CHECK(kuznetsov::listEquals< int >(b, { 10 }));
}

BOOST_AUTO_TEST_CASE(SpliceOneSelfMove)
{
  auto a = kuznetsov::makeList< int >({ 1, 2, 3, 4 });
  auto last = a.cbegin();
  ++last;
  ++last;
  ++last;
  a.splice(a.cbegin(), a, last);
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 4, 1, 2, 3 }));
}

BOOST_AUTO_TEST_CASE(SpliceRangeBasic)
{
  auto a = kuznetsov::makeList< int >({ 1, 5 });
  auto b = kuznetsov::makeList< int >({ 2, 3, 4, 99 });
  auto pos = a.cbegin();
  ++pos;
  auto first = b.cbegin();
  auto last = b.cbegin();
  ++last;
  ++last;
  ++last;
  a.splice(pos, b, first, last);
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3, 4, 5 }));
  BOOST_CHECK(kuznetsov::listEquals< int >(b, { 99 }));
}

BOOST_AUTO_TEST_CASE(SpliceRangeToEnd)
{
  auto a = kuznetsov::makeList< int >({ 1 });
  auto b = kuznetsov::makeList< int >({ 2, 3, 4 });
  auto first = b.cbegin();
  ++first;
  a.splice(a.cend(), b, first, b.cend());
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 3, 4 }));
  BOOST_CHECK(kuznetsov::listEquals< int >(b, { 2 }));
}

BOOST_AUTO_TEST_CASE(SpliceRangeWholeOther)
{
  auto a = kuznetsov::makeList< int >({});
  auto b = kuznetsov::makeList< int >({ 1, 2, 3 });
  a.splice(a.cend(), b, b.cbegin(), b.cend());
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3 }));
  BOOST_CHECK(b.empty());
}

BOOST_AUTO_TEST_CASE(SpliceRangeEmpty)
{
  auto a = kuznetsov::makeList< int >({ 1 });
  auto b = kuznetsov::makeList< int >({ 2, 3 });
  a.splice(a.cend(), b, b.cbegin(), b.cbegin());
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1 }));
  BOOST_CHECK(kuznetsov::listEquals< int >(b, { 2, 3 }));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MergeSuite)

BOOST_AUTO_TEST_CASE(MergeBasic)
{
  auto a = kuznetsov::makeList< int >({ 1, 3, 5, 7 });
  auto b = kuznetsov::makeList< int >({ 2, 4, 6, 8 });
  a.merge(b, std::less< int >{});
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3, 4, 5, 6, 7, 8 }));
  BOOST_CHECK(b.empty());
}

BOOST_AUTO_TEST_CASE(MergeWithEmptyOther)
{
  auto a = kuznetsov::makeList< int >({ 1, 2, 3 });
  auto b = kuznetsov::makeList< int >({});
  a.merge(b, std::less< int >{});
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3 }));
}

BOOST_AUTO_TEST_CASE(MergeRvalueOverload)
{
  auto a = kuznetsov::makeList< int >({ 1, 3 });
  a.merge(kuznetsov::makeList< int >({ 2, 4 }), std::less< int >{});
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3, 4 }));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SortSuite)

BOOST_AUTO_TEST_CASE(SortReversed)
{
  auto a = kuznetsov::makeList< int >({ 5, 4, 3, 2, 1 });
  a.sort(std::less< int >{});
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 2, 3, 4, 5 }));
}

BOOST_AUTO_TEST_CASE(SortWithDuplicates)
{
  auto a = kuznetsov::makeList< int >({ 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 });
  a.sort(std::less< int >{});
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9 }));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PartitionSuite)

BOOST_AUTO_TEST_CASE(PartitionBasic)
{
  auto a = kuznetsov::makeList< int >({ 1, 2, 3, 4, 5, 6 });
  auto it = a.partition(
      [](int x)
      {
        return x % 2 == 0;
      });
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 2, 4, 6, 1, 3, 5 }));
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(PartitionAllSatisfy)
{
  auto a = kuznetsov::makeList< int >({ 2, 4, 6 });
  auto it = a.partition(
      [](int x)
      {
        return x % 2 == 0;
      });
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 2, 4, 6 }));
  BOOST_CHECK(it == a.end());
}

BOOST_AUTO_TEST_CASE(PartitionPreservesOrderWithinGroups)
{
  auto a = kuznetsov::makeList< int >({ 1, 4, 2, 3, 6, 5 });
  auto it = a.partition(
      [](int x)
      {
        return x % 2 == 0;
      });
  BOOST_CHECK(kuznetsov::listEquals< int >(a, { 4, 2, 6, 1, 3, 5 }));
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_SUITE_END()
