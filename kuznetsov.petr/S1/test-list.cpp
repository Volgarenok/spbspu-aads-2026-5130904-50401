#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include <stdexcept>
#include "list.hpp"

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
  original.insert(original.end(), 1);
  original.insert(original.end(), 2);
  original.insert(original.end(), 3);

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
  original.insert(original.end(), 10);
  original.insert(original.end(), 20);

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
  list1.insert(list1.end(), 100);
  list1.insert(list1.end(), 200);

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
  list1.insert(list1.end(), 300);
  list1.insert(list1.end(), 400);

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
  list.insert(list.end(), 42);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 42);
  BOOST_CHECK_EQUAL(list.back(), 42);
}

BOOST_AUTO_TEST_CASE(InsertAtEnd)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(InsertAtBegin)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);
  list.insert(list.begin(), 1);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(InsertInMiddle)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 3);

  auto it = list.begin();
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
  list.insert(list.end(), 100);
  list.insert(list.end(), 200);

  list.front() = 999;
  BOOST_CHECK_EQUAL(list.front(), 999);
}

BOOST_AUTO_TEST_CASE(BackNonConst)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 100);
  list.insert(list.end(), 200);

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
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

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
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);

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
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

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
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  auto rit = list.rcbegin();
  BOOST_CHECK_EQUAL(*rit, 3);
  ++rit;
  BOOST_CHECK_EQUAL(*rit, 2);
  ++rit;
  BOOST_CHECK_EQUAL(*rit, 1);
}

BOOST_AUTO_TEST_CASE(IteratorDereferenceOnNull)
{
  auto list = kuz::List< int >();
  auto it = list.begin();
  BOOST_CHECK_THROW(*it, std::logic_error);
}

BOOST_AUTO_TEST_CASE(SizeAndEmpty)
{
  kuz::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.insert(list.end(), 1);
  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.insert(list.end(), 2);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DeletionSuite)

BOOST_AUTO_TEST_CASE(EraseSingleElement)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  auto it = list.begin();
  ++it;
  list.erase(it);

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(EraseOnlyElement)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 42);

  list.erase(list.begin());

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(EraseOnEmptyList)
{
  auto list = kuz::List< int >();
  BOOST_CHECK_THROW(list.erase(list.begin()), std::logic_error);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  list.popFront();

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 2);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  list.popBack();

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.back(), 2);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  list.clear();

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(MultipleEraseOperations)
{
  auto list = kuz::List< int >();
  for (int i = 1; i <= 5; ++i) {
    list.insert(list.end(), i);
  }

  list.popFront();
  list.popBack();
  list.erase(list.begin());

  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(EraseReturnsCorrectIterator)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  auto it = list.begin();
  ++it;
  auto nextIt = list.erase(it);

  BOOST_CHECK_EQUAL(*nextIt, 3);
}

BOOST_AUTO_TEST_CASE(CyclicStructureAfterOperations)
{
  auto list = kuz::List< int >();
  list.insert(list.end(), 1);
  list.insert(list.end(), 2);
  list.insert(list.end(), 3);

  list.popFront();
  list.popBack();

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 2);
  BOOST_CHECK_EQUAL(list.back(), 2);
}
BOOST_AUTO_TEST_SUITE_END()

