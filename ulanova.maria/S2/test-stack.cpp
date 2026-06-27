#include "stack.hpp"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(test_push_pop_stack)
{
  ulanova::Stack<size_t> s;
  s.push(1);
  s.push(2);
  s.push(3);

  BOOST_CHECK(s.drop() == 3);
  BOOST_CHECK(s.drop() == 2);
  BOOST_CHECK(s.drop() == 1);
}
BOOST_AUTO_TEST_CASE(test_empty_stack)
{
  ulanova::Stack<size_t> s;
  BOOST_CHECK(s.empty());
  s.push(10);
  BOOST_CHECK(!(s.empty()));
  s.drop();
  BOOST_CHECK(s.empty());
}
BOOST_AUTO_TEST_CASE(test_clear_stack)
{
  ulanova::Stack<size_t> s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.clear();
  BOOST_CHECK(s.empty());
}
BOOST_AUTO_TEST_CASE(test_exception_stack)
{
  ulanova::Stack<size_t> s;
  BOOST_CHECK_THROW(s.drop(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(test_top)
{
  ulanova::Stack<size_t> s;
  s.push(5);
  s.push(10);
  BOOST_CHECK(s.front() == 10);
  BOOST_CHECK(s.drop() == 10);
  BOOST_CHECK(s.front() == 5);
}
