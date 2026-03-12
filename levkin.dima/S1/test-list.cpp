#include "list.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>

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
  BOOST_CHECK_EQUAL(*(l.begin()++++), 3);
}
