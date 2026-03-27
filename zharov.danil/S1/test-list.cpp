#define BOOST_TEST_MODULE S1
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
  zharov::List< int > list2 = zharov::List<int>(list1);
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
  zharov::List< int > list2 = zharov::List<int>(std::move(list1));
  zharov::LIter< int > it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(DestructorTest)
{
  zharov::List< int > list;
  for (size_t i = 0; i < 10; ++i) {
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
  zharov::LCIter< int > it = const_list.constBegin();
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
  BOOST_CHECK(list.constBegin() == list.constBegin());
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
