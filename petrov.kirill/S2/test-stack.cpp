#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "Stack.hpp"

BOOST_AUTO_TEST_CASE(base_comms)
{
  petrov::Stack<int> s;
  BOOST_CHECK(s.empty());
  s.push(73);
  BOOST_CHECK(s.top() == 73);
  BOOST_CHECK(!s.empty());
}

BOOST_AUTO_TEST_CASE(sizeTest)
{
  petrov::Stack<int> s;
  for (size_t i = 0; i < 1000; ++i)
  {
    s.push(73);
  }
  BOOST_CHECK(s.size() == 1000);
}

BOOST_AUTO_TEST_CASE(check_rm)
{
  petrov::Stack<int> s;
  s.push(73);

  s.pop();
  BOOST_CHECK(s.empty());
}
