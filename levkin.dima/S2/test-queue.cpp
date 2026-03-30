#include "queue.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

using namespace levkin;
BOOST_AUTO_TEST_CASE(queue_drop_push_test)
{
  levkin::Queue< int > a{};
  a.push(1);
  a.push(2);
  a.push(3);
  BOOST_CHECK_EQUAL(a.drop(), 1);
  BOOST_CHECK_EQUAL(a.drop(), 2);
  BOOST_CHECK_EQUAL(a.drop(), 3);
}

BOOST_AUTO_TEST_CASE(queue_pop_front_test)
{
  levkin::Queue< int > a{};
  a.push(1);
  a.push(2);
  a.push(3);
  BOOST_CHECK_EQUAL(a.front(), 1);
  BOOST_CHECK_EQUAL(a.front(), 1);
  a.pop();
  a.pop();
  BOOST_CHECK_EQUAL(a.drop(), 3);
}
