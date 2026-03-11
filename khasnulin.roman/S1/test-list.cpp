#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "list.hpp"

BOOST_AUTO_TEST_SUITE(list_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_create_empty_list)
{
  BiList< int > list;

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_element)
{
  BiList< int > list;
  list.push_back(1);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
}

BOOST_AUTO_TEST_CASE(test_front_and_begin_iter_value_equal)
{
  BiList< int > list;
  list.push_back(1);

  LIter< int > it = list.begin();

  BOOST_CHECK_EQUAL(list.front(), *it);
}

BOOST_AUTO_TEST_CASE(test_iters_moving)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);

  LIter< int > it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  --it;
  BOOST_CHECK_EQUAL(*it, 2);
  --it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_list_copy_constructor)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  BiList< int > listCP(list);

  BOOST_CHECK_EQUAL(listCP.front(), 1);
  BOOST_CHECK_EQUAL(listCP.back(), 2);
}

BOOST_AUTO_TEST_CASE(test_list_move_constructor)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  BiList< int > listCP(std::move(list));

  BOOST_CHECK_EQUAL(listCP.front(), 1);
  BOOST_CHECK_EQUAL(listCP.back(), 2);

  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_list_self_assignment)
{
  BiList< int > list;
  list.push_back(1);

  list = list;

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
