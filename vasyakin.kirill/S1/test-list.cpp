#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)
BOOST_AUTO_TEST_CASE(DefConstructTest)
{
  vasyakin::List< int > list;
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(ValConstructTest)
{
  vasyakin::List< int > list(52);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK_EQUAL(*list.begin(), 52);
}

BOOST_AUTO_TEST_CASE(DestructEmptyListTest)
{
  vasyakin::List< int > list;
}

BOOST_AUTO_TEST_CASE(DestructSingleElemTest)
{
  vasyakin::List< int > list(52);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(DestructMultElemTest)
{
  vasyakin::List< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.pushBack(i);
  }
  BOOST_CHECK_EQUAL(list.getSize(), 10);
}

BOOST_AUTO_TEST_CASE(DestructAfterCopyTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > copy(list);
  BOOST_CHECK_EQUAL(list.getSize(), 2);
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(DestructAfterMoveTest)
{
  vasyakin::List< int > list;
  list.pushBack(52);
  vasyakin::List< int > moved(std::move(list));
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(CopyConstructTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > copy(list);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
  copy.pushBack(52);
  BOOST_CHECK_EQUAL(list.getSize(), 3);
  auto it = copy.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 52);
}

BOOST_AUTO_TEST_CASE(MoveConstructTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > moved(std::move(list));
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 2);
  auto it = moved.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  moved.pushBack(99);
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 3);
}

BOOST_AUTO_TEST_CASE(CopyAssignTest)
{
  vasyakin::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  vasyakin::List< int > list2;
  list2.pushBack(10);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.getSize(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list1 = list1;
  BOOST_CHECK_EQUAL(list1.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > moved;
  moved = std::move(list);
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(EraseTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.erase(list.getFake());
  BOOST_CHECK_EQUAL(list.getSize(), 2);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
  auto it = list.begin();
  for (size_t i = 0; i < list.getSize() - 2; ++i)
  {
    ++it;
  }
  list.erase(it.getPtr());
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
  list.erase(list.getFake());
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.erase(list.getFake());
  BOOST_CHECK_EQUAL(list.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.clear();
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.clear();
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  list.pushBack(42);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 42);
}

BOOST_AUTO_TEST_CASE(PushBackTest)
{
  vasyakin::List< int > list;
  list.pushBack(10);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 10);
  list.pushBack(20);
  list.pushBack(30);
  BOOST_CHECK_EQUAL(list.getSize(), 3);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK(it == list.end());
  list.clear();
  list.pushBack(42);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 42);
}

BOOST_AUTO_TEST_CASE(InsertTest)
{
  vasyakin::List< int > list;
  list.insert(list.getFake(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
  list.insert(list.getFake(), 0);
  BOOST_CHECK_EQUAL(*list.begin(), 0);
  auto it = list.begin();
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(list.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  vasyakin::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  vasyakin::List< int > list2;
  list2.pushBack(10);
  list1.swap(list2);
  BOOST_CHECK_EQUAL(list1.getSize(), 1);
  BOOST_CHECK_EQUAL(*list1.begin(), 10);
  BOOST_CHECK_EQUAL(list2.getSize(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(ConstIteratorTest)
{
  vasyakin::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  const vasyakin::List< int >& const_list = list;
  auto cit = const_list.cbegin();
  BOOST_CHECK_EQUAL(*cit, 10);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 20);
  ++cit;
  BOOST_CHECK(cit == const_list.cend());
  BOOST_CHECK(const_list.cbegin() != const_list.cend());
  auto cit2 = const_list.begin();
  BOOST_CHECK_EQUAL(*cit2, 10);
  vasyakin::List< int > empty;
  BOOST_CHECK(empty.cbegin() == empty.cend());
}

BOOST_AUTO_TEST_SUITE_END()
