#include <boost/test/unit_test.hpp>
#include <list.hpp>
#include <liter.hpp>

BOOST_AUTO_TEST_SUITE(list_iterator_tests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  chernov::LIter< int > iter1;
  chernov::LIter< int > iter2;
  BOOST_CHECK(iter1 == iter2);
}

BOOST_AUTO_TEST_CASE(test_list_constructor)
{
  chernov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);

  chernov::LIter< int > iter = list.begin();
  BOOST_CHECK_EQUAL(*iter, 3);

  ++iter;
  BOOST_CHECK_EQUAL(*iter, 2);
}

BOOST_AUTO_TEST_CASE(test_dereference_operator)
{
  chernov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);

  chernov::LIter< int > iter = list.begin();
  BOOST_CHECK_EQUAL(*iter, 2);

  ++iter;
  BOOST_CHECK_EQUAL(*iter, 1);
}

BOOST_AUTO_TEST_CASE(test_arrow_operator)
{
  struct Point {
    int x;
    int y;
  };

  chernov::List< Point > list;
  list.pushFront({1, 2});

  chernov::LIter< Point > iter = list.begin();
  BOOST_CHECK_EQUAL(iter->x, 1);
  BOOST_CHECK_EQUAL(iter->y, 2);
}

BOOST_AUTO_TEST_CASE(test_prefix_increment_operator)
{
  chernov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);

  chernov::LIter< int > iter = list.beforeBegin();
  BOOST_CHECK_EQUAL(*(++iter), 3);

  ++iter;
  BOOST_CHECK_EQUAL(*iter, 2);

  BOOST_CHECK_EQUAL(*(++iter), 1);

  ++++iter;
  BOOST_CHECK_EQUAL(*iter, 2);
}

BOOST_AUTO_TEST_CASE(test_postfix_increment_operator)
{
  chernov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);

  chernov::LIter< int > iter = list.beforeBegin();
  iter++;
  BOOST_CHECK_EQUAL(*iter, 3);

  BOOST_CHECK_EQUAL(*(iter++), 3);
  BOOST_CHECK_EQUAL(*iter, 2);

  iter++;
  BOOST_CHECK_EQUAL(*(iter++), 1);
  BOOST_CHECK_EQUAL(*iter, 3);
}

BOOST_AUTO_TEST_CASE(test_equality_operator)
{
  chernov::List< int > list;
  BOOST_CHECK(list.begin() == list.end());

  list.pushFront(1);
  list.pushFront(2);
  chernov::LIter< int > iter = list.begin();
  ++iter;
  ++iter;
  BOOST_CHECK(iter == list.begin());
}

BOOST_AUTO_TEST_CASE(test_inequality_operator)
{
  chernov::List< int > list;
  list.pushFront(1);
  BOOST_CHECK(list.begin() != list.end());

  list.pushFront(2);
  list.pushFront(3);
  chernov::LIter< int > iter1 = list.begin();
  chernov::LIter< int > iter2 = ++list.begin();
  BOOST_CHECK(iter1 != iter2);

  ++iter1;
  BOOST_CHECK(iter1 == iter2);
  ++iter1;
  BOOST_CHECK(iter1 != iter2);
}

BOOST_AUTO_TEST_SUITE_END()
