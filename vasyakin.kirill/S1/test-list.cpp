#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)
BOOST_AUTO_TEST_CASE(DefConstructTest)
{
  vasyakin::List< int > list;
  BOOST_CHECK_EQUAL(list.get_size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(ValConstructTest)
{
  vasyakin::List< int > list(52);
  BOOST_CHECK_EQUAL(list.get_size(), 1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK_EQUAL(*list.begin(), 52);
}

BOOST_AUTO_TEST_CASE(DestructEmptyList)
{
  vasyakin::List< int > list;
}
BOOST_AUTO_TEST_CASE(DestructSingleElem)
{
  vasyakin::List< int > list(52);
  BOOST_CHECK_EQUAL(list.get_size(), 1);
}
BOOST_AUTO_TEST_CASE(DestructMultElem)
{
  vasyakin::List< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  BOOST_CHECK_EQUAL(list.get_size(), 10);
}
BOOST_AUTO_TEST_CASE(DestructAfterCopy)
{
  vasyakin::List< int > list;
  list.push_back(1);
  list.push_back(2);
  vasyakin::List< int > copy(list);
  BOOST_CHECK_EQUAL(list.get_size(), 2);
  BOOST_CHECK_EQUAL(copy.get_size(), 2);
}
BOOST_AUTO_TEST_CASE(DestructAfterMove)
{
  vasyakin::List< int > list;
  list.push_back(52);
  vasyakin::List< int > moved(std::move(list));
  BOOST_CHECK_EQUAL(list.get_size(), 0);
  BOOST_CHECK_EQUAL(moved.get_size(), 1);
}

BOOST_AUTO_TEST_CASE(CopyConstruct)
{
  vasyakin::List< int > list;
  list.push_back(1);
  list.push_back(2);
  vasyakin::List< int > copy(list);
  list.push_back(3);
  BOOST_CHECK_EQUAL(copy.get_size(), 2);
  copy.push_back(52);
  BOOST_CHECK_EQUAL(list.get_size(), 3);
  auto it = copy.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 52);
}
BOOST_AUTO_TEST_CASE(MoveConstruct)
{
  vasyakin::List< int > list;
  list.push_back(1);
  list.push_back(2);
  vasyakin::List< int > moved(std::move(list));
  BOOST_CHECK_EQUAL(list.get_size(), 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK_EQUAL(moved.get_size(), 2);
  auto it = moved.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  moved.push_back(99);
  BOOST_CHECK_EQUAL(list.get_size(), 0);
  BOOST_CHECK_EQUAL(moved.get_size(), 3);
}

BOOST_AUTO_TEST_CASE(CopyAssign)
{
  vasyakin::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  vasyakin::List< int > list2;
  list2.push_back(10);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.get_size(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list1 = list1;
  BOOST_CHECK_EQUAL(list1.get_size(), 2);
}
BOOST_AUTO_TEST_CASE(MoveAssign)
{
  vasyakin::List< int > list;
  list.push_back(1);
  list.push_back(2);
  vasyakin::List< int > moved;
  moved = std::move(list);
  BOOST_CHECK_EQUAL(list.get_size(), 0);
  BOOST_CHECK_EQUAL(moved.get_size(), 2);
  moved = std::move(moved);
  BOOST_CHECK_EQUAL(moved.get_size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()