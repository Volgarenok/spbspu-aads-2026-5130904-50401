#include <boost/test/unit_test.hpp>
#include "functions.hpp"

BOOST_AUTO_TEST_CASE(infinix_basic)
{
  auto q = zhuravleva::infToPostfix("2 + 3");
  BOOST_CHECK_EQUAL(q.front(), "2");
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), "3");
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), "+");
  q.pop();
}

BOOST_AUTO_TEST_CASE(infix_priority)
{
    auto q = zhuravleva::infToPostfix("2 + 3 * 4");
  BOOST_CHECK_EQUAL(q.front(), "2");
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), "3");
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), "4");
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), "*");
  q.pop();
  BOOST_CHECK_EQUAL(q.front(), "+");
  q.pop();
}
