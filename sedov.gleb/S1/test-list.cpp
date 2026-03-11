#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(DefConstructorTest)
{
  sedov::List< int > h;
  BOOST_CHECK_EQUAL(h.size(), 0);
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
  sedov::List< int > list(1);
  BOOST_CHECK_EQUAL(list.get_size(), 1);
}

BOOST_AUTO_TEST_CASE(DestructMultElemTest)
{
  sedov::List< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  BOOST_CHECK_EQUAL(list.get_size(), 10);
}

BOOST_AUTO_TEST_CASE(DestructAfterCopyTest)
{
  sedov::List< int > list1;
  list.push_back(1);
  list.push_back(2);
  sedov::List< int > list2(list);
  BOOST_CHECK_EQUAL(list1.get_size(), 2);
  BOOST_CHECK_EQUAL(list2.get_size(), 2);
}

BOOST_AUTO_TEST_CASE(DestructAfterMoveTest)
{
  sedov::List< int > list1;
  list.push_back(52);
  sedov::List< int > list2(std::move(list));
  BOOST_CHECK_EQUAL(list1.get_size(), 0);
  BOOST_CHECK_EQUAL(list2.get_size(), 1);
}

BOOST_AUTO_TEST_CASE(CopyAssignTest)
{
  sedov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  sedov::List< int > list2;
  lsit2.pushBack(3);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list1 = list1;
  BOOST_CHECK_EQUAL(list1.size(), 2);
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
  list.push_back(1);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
  list.push_back(2);
  list.push_back(3);
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
  list.push_back(4);
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
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
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
  list.push_back(1);
  list.push_back(2);
  const sedov::List< int > & const_list = list;
  auto cit = const_list.cbegin();
  BOOST_CHECK_EQUAL(*cit, 1);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 2);
  ++cit;
  BOOST_CHECK(cit == const_list.cend());
  BOOST_CHECK(const_list.cbegin() != const_list.cend());
  auto cit2 = const_list.begin();
  BOOST_CHECK_EQUAL(*cit2, 1);
  sedov::List< int > empty;
  BOOST_CHECK(empty.cbegin() == empty.cend());
}

BOOST_AUTO_TEST_SUITE_END()
