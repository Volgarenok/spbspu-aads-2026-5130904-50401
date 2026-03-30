#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include "queue.hpp"
using namespace levkin;
BOOST_AUTO_TEST_CASE(constructor_test)
{
  levkin::Queue< int > a{};
  a.push(1);
  a.push(2);
  a.push(3);
  BOOST_CHECK_EQUAL(a.drop(), 1);
  BOOST_CHECK_EQUAL(a.drop(), 2);
  BOOST_CHECK_EQUAL(a.drop(), 3);
}
