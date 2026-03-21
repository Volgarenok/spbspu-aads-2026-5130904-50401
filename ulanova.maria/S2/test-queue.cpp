#include "queue.hpp"
#include <boost/test/unit_test.hpp>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(test_push_pop)
{
  ulanova::Queue<size_t> q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK(q.drop() == 1);
  BOOST_CHECK(q.drop() == 2);
  BOOST_CHECK(q.drop() == 3);
}
BOOST_AUTO_TEST_CASE(test_empty)
{
  ulanova::Queue<size_t> q;
  BOOST_CHECK(q.empty());
  q.push(10);
  BOOST_CHECK(!(q.empty()));
  q.drop();
  BOOST_CHECK(q.empty());
}
BOOST_AUTO_TEST_CASE(test_clear)
{
  ulanova::Queue<size_t> q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.clear();
  BOOST_CHECK(q.empty());
}
BOOST_AUTO_TEST_CASE(test_exception)
{
  ulanova::Queue<size_t> q;
  BOOST_CHECK_THROW(q.drop(), std::runtime_error);
}
