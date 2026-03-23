#include <boost/test/unit_test.hpp>
#include "queue.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(queue_push_and_drop)
{
  vasyakin::Queue< int > queue;

  queue.push(1);
  queue.push(2);
  queue.push(3);

  BOOST_CHECK_EQUAL(queue.drop(), 1);
  BOOST_CHECK_EQUAL(queue.drop(), 2);
  BOOST_CHECK_EQUAL(queue.drop(), 3);
}

BOOST_AUTO_TEST_CASE(queue_peek)
{
  vasyakin::Queue< int > queue;

  queue.push(10);
  queue.push(20);

  BOOST_CHECK_EQUAL(queue.peek(), 10);
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
  vasyakin::Queue<std::string> queue;

  queue.push("first");
  queue.push("second");

  BOOST_CHECK_EQUAL(queue.drop(), "first");
  BOOST_CHECK_EQUAL(queue.drop(), "second");
}

BOOST_AUTO_TEST_SUITE_END()
