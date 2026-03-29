#include <boost/test/unit_test.hpp>
#include <queue.hpp>

BOOST_AUTO_TEST_SUITE(queue_suite)

BOOST_AUTO_TEST_CASE(test_create_queue)
{
  hvostov::Queue< int > queue;
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_push_element)
{
  hvostov::Queue< int > queue;
  queue.push(6);
  BOOST_CHECK(!queue.empty());
  BOOST_CHECK(queue.front() == 6);
  queue.push(7);
  BOOST_CHECK(queue.front() == 6);
  BOOST_CHECK(queue.back() == 7);
  queue.push(8);
  BOOST_CHECK(queue.front() == 6);
  BOOST_CHECK(queue.back() == 8);
}

BOOST_AUTO_TEST_CASE(test_drop_element)
{
  hvostov::Queue< int > queue;
  queue.push(6);
  queue.push(7);
  queue.push(8);
  BOOST_CHECK(queue.front() == 6);
  BOOST_CHECK(queue.back() == 8);
  queue.drop();
  BOOST_CHECK(queue.front() == 7);
  BOOST_CHECK(queue.back() == 8);
  queue.drop();
  BOOST_CHECK(queue.front() == 8);
  BOOST_CHECK(queue.back() == 8);
  queue.drop();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::Queue< int > queue;
  queue.push(6);
  queue.push(7);
  queue.push(8);
  BOOST_CHECK(!queue.empty());
  queue.clear();
  BOOST_CHECK(queue.empty());
  queue.push(6);
  queue.push(7);
  BOOST_CHECK(!queue.empty());
  queue.clear();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()
