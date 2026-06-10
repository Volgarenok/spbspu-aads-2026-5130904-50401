#include "stack.hpp"

#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>

BOOST_AUTO_TEST_CASE(test_push_pop_stack)
{
  ulanova::Stack<size_t> s;
  s.push(1);
  s.push(2);
  s.push(3);

  BOOST_CHECK(s.front() == 3);
  s.pop();
  BOOST_CHECK(s.front() == 2);
  s.pop();
  BOOST_CHECK(s.front() == 1);
  s.pop();
}
BOOST_AUTO_TEST_CASE(test_empty_stack)
{
  ulanova::Stack<size_t> s;
  BOOST_CHECK(s.empty());
  s.push(10);
  BOOST_CHECK(!(s.empty()));
  s.pop();
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
  BOOST_CHECK_THROW(s.pop(), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(test_top)
{
  ulanova::Stack<size_t> s;
  s.push(5);
  s.push(10);
  BOOST_CHECK(s.front() == 10);
  BOOST_CHECK(s.front() == 10);
  s.pop();
  BOOST_CHECK(s.front() == 5);
}

BOOST_AUTO_TEST_CASE(test_stack_emplace)
{
  ulanova::Stack< std::string > stack;
  stack.emplace(3, 'a');

  BOOST_CHECK(stack.front() == "aaa");
}
