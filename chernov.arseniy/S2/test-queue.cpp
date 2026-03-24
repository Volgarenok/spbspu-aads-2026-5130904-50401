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

BOOST_AUTO_TEST_SUITE(queue_capacity_tests)

BOOST_AUTO_TEST_CASE(test_front)
{
  chernov::Queue< int > queue;
  queue.push(1);
  BOOST_CHECK_EQUAL(queue.front(), 1);

  queue.push(2);
  BOOST_CHECK_EQUAL(queue.front(), 1);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 2);
}

BOOST_AUTO_TEST_CASE(test_const_front)
{
  chernov::Queue< int > queue;
  queue.push(1);

  const chernov::Queue< int > & const_queue = queue;
  BOOST_CHECK_EQUAL(const_queue.front(), 1);
}

BOOST_AUTO_TEST_CASE(test_back)
{
  chernov::Queue< int > queue;
  queue.push(1);
  BOOST_CHECK_EQUAL(queue.back(), 1);

  queue.push(2);
  BOOST_CHECK_EQUAL(queue.back(), 2);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.back(), 2);
}

BOOST_AUTO_TEST_CASE(test_const_back)
{
  chernov::Queue< int > queue;
  queue.push(1);
  queue.push(2);

  const chernov::Queue< int > & const_queue = queue;
  BOOST_CHECK_EQUAL(const_queue.back(), 2);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  chernov::Queue< int > queue;
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(1);
  BOOST_CHECK_EQUAL(queue.size(), 1);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(2);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  chernov::Queue< int > queue;
  BOOST_CHECK(queue.empty());

  queue.push(1);
  BOOST_CHECK(!queue.empty());

  queue.push(2);
  queue.clear();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queue_modifiers_tests)

BOOST_AUTO_TEST_CASE(test_push)
{
  chernov::Queue< int > queue;

  queue.push(1);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  BOOST_CHECK_EQUAL(queue.front(), 1);

  queue.push(2);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 3);
  BOOST_CHECK_EQUAL(queue.back(), 3);

  int value = 4;
  queue.push(value);
  BOOST_CHECK_EQUAL(queue.size(), 4);
  BOOST_CHECK_EQUAL(queue.back(), 4);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  chernov::Queue< int > queue;

  queue.push(1);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(2);
  queue.push(3);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 1);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 0);
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  chernov::Queue< int > queue;

  queue.clear();
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(1);
  queue.clear();
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(2);
  queue.push(3);
  queue.push(4);
  queue.clear();
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.clear();
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
