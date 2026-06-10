#include "postfix.hpp"

#include "boost/test/unit_test.hpp"

BOOST_AUTO_TEST_CASE(test_simple_postfix)
{
  ulanova::Queue< std::string > expr1;
  expr1.push("2");
  expr1.push("3");
  expr1.push("+");
  BOOST_TEST_CHECK(ulanova::postfix(expr1) == 5);

  ulanova::Queue< std::string > expr2;
  expr2.push("4");
  expr2.push("3");
  expr2.push("+");
  BOOST_TEST_CHECK(ulanova::postfix(expr2) == 7);

  ulanova::Queue< std::string > expr3;
  expr3.push("10");
  expr3.push("2");
  expr3.push("/");
  BOOST_TEST_CHECK(ulanova::postfix(expr3) == 5);
}
BOOST_AUTO_TEST_CASE(test_complex_postfix)
{
  ulanova::Queue< std::string > expr;
  expr.push("2");
  expr.push("3");
  expr.push("+");
  expr.push("4");
  expr.push("*");
  BOOST_TEST_CHECK(ulanova::postfix(expr) == 20);
}
BOOST_AUTO_TEST_CASE(test_xor_postfix)
{
  ulanova::Queue< std::string > expr;
  expr.push("5");
  expr.push("3");
  expr.push("^");
  BOOST_TEST_CHECK(ulanova::postfix(expr) == 6);
}
