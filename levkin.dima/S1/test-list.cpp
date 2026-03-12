#include "list.hpp"
#include <boost/test/unit_test.hpp>

using namespace levkin;
BOOST_AUTO_TEST_CASE(constructor_test)
{
  List< int > l(3);
  BOOST_CHECK(l.begin() != l.end());
  BOOST_CHECK(*(l.begin()) == 3);
}

BOOST_AUTO_TEST_CASE(iter_test)
{
  List< int > l;
  l.insertBack(1);
  l.insertBack(2);
  l.insertBack(3);

  auto it2 = l.begin();
  BOOST_CHECK_EQUAL(*it2, 1);
  BOOST_CHECK_EQUAL(*(++it2), 2);
  BOOST_CHECK_EQUAL(*(++it2), 3);
  BOOST_CHECK(++it2 == l.end());
  BOOST_CHECK_EQUAL(*(--it2), 3);
  BOOST_CHECK_EQUAL(*(--it2), 2);
  BOOST_CHECK_EQUAL(*(--it2), 1);
}

BOOST_AUTO_TEST_CASE(comp_iters)
{
  List< int > l;
  l.insertBack(10);

  auto it1 = l.begin();  
  auto it2 = l.cbegin();
  
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK_EQUAL(*it1, *it2);
}


BOOST_AUTO_TEST_CASE(citer_test)
{
  List< int > l;
  l.insertBack(1);
  l.insertBack(2);
  l.insertBack(3);

  auto it2 = l.cbegin();
  BOOST_CHECK_EQUAL(*it2, 1);
  BOOST_CHECK_EQUAL(*(++it2), 2);
  BOOST_CHECK_EQUAL(*(++it2), 3);
  BOOST_CHECK(++it2 == l.end());
  BOOST_CHECK_EQUAL(*(--it2), 3);
  BOOST_CHECK_EQUAL(*(--it2), 2);
  BOOST_CHECK_EQUAL(*(--it2), 1);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  List< int > l;
  l.insertBack(1);
  l.insertBack(2);
  l.insertBack(3);

  l.erase(++l.begin());

  auto it = l.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 3);

  l.clear();
  BOOST_CHECK(l.begin() == l.end());
}
