#include "queue.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(queue_tests)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  zubarev::Queue< size_t > q;

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(push_test)
{
  zubarev::Queue< size_t > q;

  q.push(10);
  BOOST_CHECK(q.top() == 10);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  zubarev::Queue< size_t > q;
  q.push(1);
  q.push(2);

  q.drop();
  BOOST_CHECK(q.top() == 2);
  q.drop();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  zubarev::Queue< size_t > q;

  q.push(1);
  q.push(2);
  q.push(3);

  zubarev::Queue< size_t > copy(q);

  while (!q.empty() && !copy.empty()) {
    BOOST_CHECK(q.top() == copy.top());
    q.drop();
    copy.drop();
  }

  BOOST_CHECK(q.empty() && copy.empty());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  zubarev::Queue< size_t > q;

  q.push(1);
  q.push(2);

  zubarev::Queue< size_t > moved(std::move(q));

  BOOST_CHECK(q.empty());

  BOOST_CHECK(moved.top() == 1);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  zubarev::Queue< size_t > q1;
  q1.push(1);
  q1.push(2);

  zubarev::Queue< size_t > q2;
  q2 = q1;

  while (!q1.empty() && !q2.empty()) {
    BOOST_CHECK(q1.top() == q2.top());
    q1.drop();
    q2.drop();
  }

  BOOST_CHECK(q1.empty() && q2.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  zubarev::Queue< size_t > q1;
  q1.push(10);
  q1.push(20);

  zubarev::Queue< size_t > q2;
  q2 = std::move(q1);

  BOOST_CHECK(q1.empty());

  BOOST_CHECK(q2.top() == 10);
}

BOOST_AUTO_TEST_CASE(top_test)
{
  zubarev::Queue< size_t > q;

  q.push(1);
  q.push(2);

  BOOST_CHECK(q.top() == 1);
}

BOOST_AUTO_TEST_CASE(last_test)
{
  zubarev::Queue< size_t > q;

  q.push(1);
  q.push(2);

  BOOST_CHECK(q.last() == 2);
}

BOOST_AUTO_TEST_CASE(size_test)
{
  const size_t sizeOfQueue = 3;
  zubarev::Queue< size_t > q;
  for (size_t i = 0; i < sizeOfQueue; ++i) {
    q.push(i);
  }

  BOOST_CHECK(q.size() == sizeOfQueue);
}

BOOST_AUTO_TEST_SUITE_END()
