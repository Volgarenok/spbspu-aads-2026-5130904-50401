#include <boost/test/unit_test.hpp>
#include <queue.hpp>

BOOST_AUTO_TEST_SUITE(queue_special_members_tests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  chernov::Queue< int > queue;
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  chernov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  chernov::Queue< int > queue2(queue1);
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.front(), 1);
  BOOST_CHECK_EQUAL(queue2.back(), 2);

  queue2.push(3);
  BOOST_CHECK_EQUAL(queue1.size(), 2);
  BOOST_CHECK_EQUAL(queue2.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  chernov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  chernov::Queue<int> queue2(std::move(queue1));
  BOOST_CHECK_EQUAL(queue2.size(), 2);

  BOOST_CHECK_EQUAL(queue1.size(), 0);
  BOOST_CHECK(queue1.empty());

  queue2.push(3);
  BOOST_CHECK_EQUAL(queue2.size(), 3);
  BOOST_CHECK_EQUAL(queue1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
  chernov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  queue1 = queue1;
  BOOST_CHECK_EQUAL(queue1.size(), 2);

  chernov::Queue< int > queue2;
  queue2 = queue1;
  BOOST_CHECK_EQUAL(queue2.size(), 2);

  queue2.push(3);
  BOOST_CHECK_EQUAL(queue1.size(), 2);
  BOOST_CHECK_EQUAL(queue2.size(), 3);

  queue1.clear();
  BOOST_CHECK_EQUAL(queue1.size(), 0);
  queue2 = queue1;
  BOOST_CHECK_EQUAL(queue2.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_operator)
{
  chernov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  queue1 = queue1;
  BOOST_CHECK_EQUAL(queue1.size(), 2);

  chernov::Queue< int > queue2;
  queue2 = std::move(queue1);
  BOOST_CHECK_EQUAL(queue2.size(), 2);

  BOOST_CHECK_EQUAL(queue1.size(), 0);
  BOOST_CHECK(queue1.empty());

  queue2.push(3);
  BOOST_CHECK_EQUAL(queue2.size(), 3);
  BOOST_CHECK_EQUAL(queue1.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
