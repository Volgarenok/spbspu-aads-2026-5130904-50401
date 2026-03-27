#include <boost/test/unit_test.hpp>
#include <queue.hpp>

BOOST_AUTO_TEST_CASE(QueueDefaultConstructorTest)
{
  zharov::Queue< int > queue;
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(QueueCopyConstructorTest)
{
  zharov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);
  BOOST_CHECK_EQUAL(queue1.front(), 1);
  BOOST_CHECK_EQUAL(queue1.back(), 3);
  zharov::Queue< int > queue2 = zharov::Queue<int>(queue1);
  BOOST_CHECK_EQUAL(queue2.front(), 1);
  BOOST_CHECK_EQUAL(queue2.back(), 3);
}

BOOST_AUTO_TEST_CASE(QueueMoveConstructorTest)
{
  zharov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);
  BOOST_CHECK_EQUAL(queue1.front(), 1);
  BOOST_CHECK_EQUAL(queue1.back(), 3);
  zharov::Queue< int > queue2 = zharov::Queue<int>(std::move(queue1));
  BOOST_CHECK_EQUAL(queue2.front(), 1);
  BOOST_CHECK_EQUAL(queue2.back(), 3);
  BOOST_CHECK_EQUAL(queue1.size(), 0);
}

BOOST_AUTO_TEST_CASE(QueueDestructorTest)
{
  zharov::Queue< int > queue;
  for (size_t i = 0; i < 10; ++i) {
    queue.push(i);
  }
  BOOST_CHECK_EQUAL(queue.size(), 10);
}

BOOST_AUTO_TEST_CASE(QueueCopyOperatorTest)
{
  zharov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(3);
  BOOST_CHECK_EQUAL(queue1.front(), 1);
  BOOST_CHECK_EQUAL(queue1.back(), 3);
  zharov::Queue< int > queue2;
  queue2.push(11);
  queue2.push(33);
  BOOST_CHECK_EQUAL(queue2.front(), 11);
  BOOST_CHECK_EQUAL(queue2.back(), 33);
  queue2 = queue1;
  BOOST_CHECK_EQUAL(queue2.front(), 1);
  BOOST_CHECK_EQUAL(queue2.back(), 3);
}

BOOST_AUTO_TEST_CASE(QueueMoveOperatorTest)
{
  zharov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(3);
  BOOST_CHECK_EQUAL(queue1.front(), 1);
  BOOST_CHECK_EQUAL(queue1.back(), 3);
  zharov::Queue< int > queue2;
  queue2.push(11);
  queue2.push(33);
  BOOST_CHECK_EQUAL(queue2.front(), 11);
  BOOST_CHECK_EQUAL(queue2.back(), 33);
  queue2 = std::move(queue1);
  BOOST_CHECK_EQUAL(queue2.front(), 1);
  BOOST_CHECK_EQUAL(queue2.back(), 3);
  BOOST_CHECK_EQUAL(queue1.size(), 0);
}

BOOST_AUTO_TEST_CASE(QueueFrontTest)
{
  zharov::Queue< int > queue;
  queue.push(1);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.front(), 1);
  const zharov::Queue< int > queue2(queue);
  BOOST_CHECK_EQUAL(queue2.front(), 1);
}

BOOST_AUTO_TEST_CASE(QueueBackTest)
{
  zharov::Queue< int > queue;
  queue.push(1);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.back(), 3);
  const zharov::Queue< int > queue2(queue);
  BOOST_CHECK_EQUAL(queue2.back(), 3);
}

BOOST_AUTO_TEST_CASE(QueuepushTest)
{
  zharov::Queue< int > queue;
  queue.push(1);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 2);
  BOOST_CHECK_EQUAL(queue.front(), 1);
}

BOOST_AUTO_TEST_CASE(QueueDropTest)
{
  zharov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  int n = queue.drop();
  BOOST_CHECK_EQUAL(n, 1);
  BOOST_CHECK_EQUAL(queue.front(), 2);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(QueueClearTest)
{
  zharov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 3);
  queue.clear();
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(QueueSizeTest)
{
  zharov::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 3);
}

BOOST_AUTO_TEST_CASE(QueueEmptyTest)
{
  zharov::Queue< int > queue;
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(QueueSwapTest)
{
  zharov::Queue< int > queue1;
  queue1.push(10);
  queue1.push(20);
  queue1.push(30);
  zharov::Queue< int > queue2;
  queue2.push(100);
  queue2.push(200);
  BOOST_CHECK_EQUAL(queue1.size(), 3);
  BOOST_CHECK_EQUAL(queue1.front(), 10);
  BOOST_CHECK_EQUAL(queue1.back(), 30);
  BOOST_CHECK_EQUAL(queue2.size(), 2);
  BOOST_CHECK_EQUAL(queue2.front(), 100);
  BOOST_CHECK_EQUAL(queue2.back(), 200);
  queue1.swap(queue2);
  BOOST_CHECK_EQUAL(queue1.size(), 2);
  BOOST_CHECK_EQUAL(queue1.front(), 100);
  BOOST_CHECK_EQUAL(queue1.back(), 200);
  BOOST_CHECK_EQUAL(queue2.size(), 3);
  BOOST_CHECK_EQUAL(queue2.front(), 10);
  BOOST_CHECK_EQUAL(queue2.back(), 30);
}
