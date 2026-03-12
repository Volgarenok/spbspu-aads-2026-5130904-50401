#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

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