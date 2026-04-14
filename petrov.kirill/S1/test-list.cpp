#define BOOST_TEST_MODULE ListDeepTest
#include <boost/test/included/unit_test.hpp>
#include <string>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListDetailedTests)

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  petrov::List<int> l;
  l.push_back(100);

  const petrov::List<int>& cl = l;
  petrov::LIter<int> it = cl.begin();

  BOOST_CHECK_EQUAL(*it, 100);
  BOOST_CHECK(it == cl.begin());
  BOOST_CHECK(it != cl.end());
}

BOOST_AUTO_TEST_CASE(test_push_front_and_order)
{
  petrov::List<int> l;
  l.push_back(2);
  l.push_back(3);
  l.push_front(1);

  petrov::LIter<int> it = l.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  petrov::List<int> l;
  l.push_back(10);
  l.push_back(20);

  l.pop_back();
  BOOST_CHECK_EQUAL(*l.begin(), 10);

  l.pop_back();
  BOOST_CHECK(l.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  petrov::List<int> l;
  l.push_back(10);
  l.push_back(20);

  l.pop_front();
  BOOST_CHECK_EQUAL(*l.begin(), 20);

  l.pop_front();
  BOOST_CHECK(l.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  petrov::List<int> l;
  l.push_back(1);
  l.push_back(2);

  l = l; 

  petrov::LIter<int> it = l.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK(!l.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_assignment_operator)
{
  petrov::List<int> l1;
  l1.push_back(10);
  l1.push_back(20);

  petrov::List<int> l2;
  l2.push_back(30);

  l2 = l1;

  BOOST_CHECK_EQUAL(*l2.begin(), 10);
  l1.clear();
  BOOST_CHECK_EQUAL(*l2.begin(), 10);
}

BOOST_AUTO_TEST_CASE(test_iterator_decrement)
{
  petrov::List<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  petrov::LIter<int> it = l.end();
  --it;
  BOOST_CHECK_EQUAL(*it, 3);
  it--;
  BOOST_CHECK_EQUAL(*it, 2);
  --it;
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK(it == l.begin());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  petrov::List<int> l1;
  l1.push_back(1);
  petrov::List<int> l2;
  l2.push_back(2);
  l2.push_back(3);
  std::swap(l1, l2);
  BOOST_CHECK_EQUAL(*l1.begin(), 2);
  BOOST_CHECK_EQUAL(*l2.begin(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
