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

BOOST_AUTO_TEST_SUITE(list_iterator_tests)

BOOST_AUTO_TEST_CASE(test_before_begin_iterator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.before_begin() == list.end());

  list.push_front(1);
  list.push_front(2);
  BOOST_CHECK(list.before_begin() == list.end());
  BOOST_CHECK(++list.before_begin() == list.begin());
}

BOOST_AUTO_TEST_CASE(test_сbefore_begin_iterator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.before_begin() == list.end());

  list.push_front(1);
  list.push_front(2);
  BOOST_CHECK(list.before_begin() == list.end());
  BOOST_CHECK(++list.before_begin() == list.begin());
}

BOOST_AUTO_TEST_CASE(test_begin_iterator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.begin() == list.end());

  list.push_front(1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK_EQUAL(*list.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_cbegin_iterator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.begin() == list.end());

  list.push_front(1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK_EQUAL(*list.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_end_iterator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.begin() == list.end());

  list.push_front(1);
  list.push_front(2);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(++list.end() == list.begin());
}

BOOST_AUTO_TEST_CASE(test_cend_iterator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.begin() == list.end());

  list.push_front(1);
  list.push_front(2);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(++list.end() == list.begin());
}

BOOST_AUTO_TEST_CASE(test_loop_iterator)
{
  chernov::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  BOOST_CHECK_EQUAL(list.size(), 3);

  chernov::LIter< int > iter = list.begin();
  BOOST_CHECK_EQUAL(*iter, 3);
  ++iter;
  BOOST_CHECK_EQUAL(*iter, 2);
  ++iter;
  BOOST_CHECK_EQUAL(*iter, 1);
  ++iter;
  BOOST_CHECK_EQUAL(*iter, 3);

  BOOST_CHECK(iter == list.begin());
}

BOOST_AUTO_TEST_CASE(test_insert_after_iterator)
{
  chernov::List< int > list;
  chernov::LIter< int > iter = list.insert_after(list.before_begin(), 1);
  BOOST_CHECK(iter == list.begin());

  iter = list.insert_after(iter, 2);
  BOOST_CHECK_EQUAL(*iter, 2);

  iter = list.insert_after(iter, 3);
  BOOST_CHECK_EQUAL(*iter, 3);

  ++iter;
  BOOST_CHECK_EQUAL(*iter, 1);
}

BOOST_AUTO_TEST_CASE(test_one_erase_after_iterator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.erase_after(list.before_begin()) == list.end());

  chernov::LIter< int > last = list.insert_after(list.before_begin(), 1);
  list.push_front(2);
  list.push_front(3);

  BOOST_CHECK_EQUAL(*list.erase_after(last), 2);

  chernov::LIter< int > iter = list.insert_after(list.before_begin(), 4);
  iter = list.erase_after(iter);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*iter, 1);
  ++iter;
  BOOST_CHECK_EQUAL(*iter, 4);
}

BOOST_AUTO_TEST_CASE(test_many_erase_after_iterator)
{
  chernov::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  list.push_front(4);
  list.push_front(5);

  chernov::LIter< int > first = list.before_begin();
  chernov::LIter< int > last = list.begin();
  ++last;
  ++last;
  ++last;

  list.erase_after(first, last);
  
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
}

BOOST_AUTO_TEST_CASE(test_many_erase_after_iterator_through_fake)
{
  chernov::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  list.push_front(4);
  list.push_front(5);

  chernov::LIter< int > first = list.begin();
  ++first;
  ++first;

  chernov::LIter< int > last = list.begin();
  ++last;

  list.erase_after(first, last);
  
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(*list.begin(), 3);
  
  ++list.begin();
  BOOST_CHECK_EQUAL(*(++list.begin()), 4);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(list_capacity_tests)

BOOST_AUTO_TEST_CASE(test_first)
{
  chernov::List< int > list;
  list.push_front(1);
  BOOST_CHECK_EQUAL(list.first(), 1);

  list.push_front(2);
  list.push_front(3);
  BOOST_CHECK_EQUAL(list.first(), 3);

  list.pop_front();
  BOOST_CHECK_EQUAL(list.first(), 2);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  chernov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.push_front(1);
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.pop_front();
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.push_front(2);
  list.push_front(3);
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  chernov::List< int > list;
  BOOST_CHECK(list.empty());

  list.push_front(1);
  BOOST_CHECK(!list.empty());

  list.push_front(2);
  list.clear();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_SUITE_END()
