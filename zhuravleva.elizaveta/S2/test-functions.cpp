#include <boost/test/unit_test.hpp>
#include "functions.hpp"

BOOST_AUTO_TEST_CASE(simple_expression)
{
  auto q = zhuravleva::infToPostfix("1 + 2");
  auto res = zhuravleva::calcPostfix(q);
  BOOST_CHECK_EQUAL(res, 3);
}

BOOST_AUTO_TEST_CASE(priority_expression)
{
  auto q = zhuravleva::infToPostfix("2 + 3 * 4");
  auto res = zhuravleva::calcPostfix(q);
  BOOST_CHECK_EQUAL(res, 14);
}

BOOST_AUTO_TEST_CASE(parentheses_expression)
{
  auto q = zhuravleva::infToPostfix("( 2 + 3 ) * 4");
  auto res = zhuravleva::calcPostfix(q);

  BOOST_CHECK_EQUAL(res, 20);
}

BOOST_AUTO_TEST_CASE(bit_not)
{
  auto q = zhuravleva::infToPostfix("! 5");
  auto res = zhuravleva::calcPostfix(q);

  BOOST_CHECK_EQUAL(res, ~5);
}

BOOST_AUTO_TEST_CASE(error_expression)
{
  BOOST_CHECK_THROW(
    zhuravleva::calcPostfix(
      zhuravleva::infToPostfix("( 1 + )")
    ),
    std::runtime_error
  );
}
