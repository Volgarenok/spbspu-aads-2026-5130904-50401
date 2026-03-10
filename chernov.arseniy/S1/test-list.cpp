#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(list_special_members_tests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  chernov::List< int > list;

  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  chernov::List< int > * list = new chernov::List< int >;
  list->push_front(1);
  list->push_front(2);
  list->push_front(3);

  BOOST_REQUIRE_EQUAL(list->size(), 3);
  delete list;
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  chernov::List< int > list1;
  list1.push_front(1);
  list1.push_front(2);

  chernov::List< int > list2(list1);
  BOOST_CHECK(list1.begin() != list2.begin());
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 2);
  
  list2.push_front(3);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(list2.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  chernov::List< int > list1;
  list1.push_front(1);
  list1.push_front(2);
  
  chernov::List<int> list2(std::move(list1));
  
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 2);
  
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK(list1.empty());
  
  list2.push_front(3);
  BOOST_CHECK_EQUAL(list2.size(), 3);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
  chernov::List< int > list1;
  list1.push_front(1);
  list1.push_front(2);

  list1 = list1;
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(*list1.begin(), 2);

  chernov::List< int > list2;
  list2 = list1;

  BOOST_CHECK(list1.begin() != list2.begin());
  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 2);
  
  list2.push_front(3);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(list2.size(), 3);

  list1.clear();
  BOOST_CHECK_EQUAL(list1.size(), 0);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_operator)
{
  chernov::List< int > list1;
  list1.push_front(1);
  list1.push_front(2);

  list1 = list1;
  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK_EQUAL(*list1.begin(), 2);

  chernov::List< int > list2;
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 2);
  
  BOOST_CHECK_EQUAL(list1.size(), 0);
  BOOST_CHECK(list1.empty());
  
  list2.push_front(3);
  BOOST_CHECK_EQUAL(list2.size(), 3);
  BOOST_CHECK_EQUAL(list1.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
