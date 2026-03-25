#include "stack.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  zubarev::Stack< size_t > s;

  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(push_test)
{
  zubarev::Stack< size_t > s;

  s.push(10);
  BOOST_CHECK(s.top() == 10);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  zubarev::Stack< size_t > s;
  s.push(1);
  s.push(2);

  s.drop();
  BOOST_CHECK(s.top() == 1);
  s.drop();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  zubarev::Stack< size_t > s;

  s.push(1);
  s.push(2);
  s.push(3);

  zubarev::Stack< size_t > copy(s);

  while (!s.empty() && !copy.empty()) {
    BOOST_CHECK(s.top() == copy.top());
    s.drop();
    copy.drop();
  }

  BOOST_CHECK(s.empty() && copy.empty());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  zubarev::Stack< size_t > s;

  s.push(1);
  s.push(2);

  zubarev::Stack< size_t > moved(std::move(s));

  BOOST_CHECK(s.empty());

  BOOST_CHECK(moved.top() == 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  zubarev::Stack< size_t > s1;
  s1.push(1);
  s1.push(2);

  zubarev::Stack< size_t > s2;
  s2 = s1;

  while (!s1.empty() && !s2.empty()) {
    BOOST_CHECK(s1.top() == s2.top());
    s1.drop();
    s2.drop();
  }

  BOOST_CHECK(s1.empty() && s2.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  zubarev::Stack< size_t > s1;
  s1.push(10);
  s1.push(20);

  zubarev::Stack< size_t > s2;
  s2 = std::move(s1);

  BOOST_CHECK(s1.empty());

  BOOST_CHECK(s2.top() == 20);
}

BOOST_AUTO_TEST_CASE(top_test)
{
  zubarev::Stack< size_t > s;

  s.push(1);
  s.push(2);

  BOOST_CHECK(s.top() == 2);
}

BOOST_AUTO_TEST_CASE(size_test)
{
  const size_t sizeOfQueue = 3;
  zubarev::Stack< size_t > s;
  for (size_t i = 0; i < sizeOfQueue; ++i) {
    s.push(i);
  }

  BOOST_CHECK(s.size() == sizeOfQueue);
}

BOOST_AUTO_TEST_SUITE_END()
