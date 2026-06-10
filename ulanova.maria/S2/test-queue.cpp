#include "queue.hpp"

#include <boost/test/unit_test.hpp>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(test_push_pop_queue)
{
  ulanova::Queue<size_t> q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK(q.front() == 1);
  q.pop();
  BOOST_CHECK(q.front() == 2);
  q.pop();
  BOOST_CHECK(q.front() == 3);
  q.pop();
}
BOOST_AUTO_TEST_CASE(test_empty_queue)
{
  ulanova::Queue<size_t> q;
  BOOST_CHECK(q.empty());
  q.push(10);
  BOOST_CHECK(!(q.empty()));
  q.pop();
  BOOST_CHECK(q.empty());
}
BOOST_AUTO_TEST_CASE(test_clear_queue)
{
  ulanova::Queue<size_t> q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.clear();
  BOOST_CHECK(q.empty());
}
BOOST_AUTO_TEST_CASE(test_exception_queue)
{
  ulanova::Queue<size_t> q;
  BOOST_CHECK_THROW(q.pop(), std::runtime_error);
}
