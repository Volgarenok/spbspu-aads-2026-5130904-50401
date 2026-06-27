#include <boost/test/unit_test.hpp>
#include "test.hpp"
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(queue_push_and_drop)
{
  vasyakin::Queue< int > queue;

  queue.push(1);
  queue.push(2);
  queue.push(3);

  BOOST_CHECK_EQUAL(queue.front(), 1);
  queue.pop();

  BOOST_CHECK_EQUAL(queue.front(), 2);
  queue.pop();

  BOOST_CHECK_EQUAL(queue.front(), 3);
  queue.pop();
}

BOOST_AUTO_TEST_CASE(queue_front)
{
  vasyakin::Queue< int > queue;

  queue.push(10);
  queue.push(20);

  BOOST_CHECK_EQUAL(queue.front(), 10);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(queue_empty_and_size)
{
  vasyakin::Queue< int > queue;

  BOOST_CHECK(queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(1);
  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 1);
}

BOOST_AUTO_TEST_CASE(queue_with_strings)
{
  vasyakin::Queue< std::string > queue;

  queue.push("first");
  queue.push("second");

  BOOST_CHECK_EQUAL(queue.front(), "first");
  queue.pop();

  BOOST_CHECK_EQUAL(queue.front(), "second");
  queue.pop();
}

BOOST_AUTO_TEST_CASE(queue_emplace)
{
  vasyakin::Queue< TestNoDefault > queue;

  queue.emplace(67, "sixseven");
  queue.emplace(52, "ayyy");

  BOOST_CHECK_EQUAL(queue.size(), 2);
  BOOST_CHECK_EQUAL(queue.front().val_, 67);
  BOOST_CHECK_EQUAL(queue.front().str_, "sixseven");
}

BOOST_AUTO_TEST_SUITE_END()
