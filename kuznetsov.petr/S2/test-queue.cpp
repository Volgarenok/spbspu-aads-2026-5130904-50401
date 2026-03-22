#include <boost/test/unit_test.hpp>
#include "../common/includes/queue.hpp"

BOOST_AUTO_TEST_CASE(EmptyQueueTest)
{
  kuznetsov::Queue< int > queue;
  BOOST_CHECK(queue.empty());
  BOOST_CHECK_THROW(queue.front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(AddElementToQueueTest)
{
  kuznetsov::Queue< int > queue;
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  queue.push(4);
  BOOST_CHECK_EQUAL(queue.size(), 2);
}

BOOST_AUTO_TEST_CASE(PopElementFromQueueTest)
{
  kuznetsov::Queue< int > queue;
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.size(), 1);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(getAccessToDataOfQueueTest)
{
  kuznetsov::Queue< int > queue;
  queue.push(3);
  BOOST_CHECK_EQUAL(queue.front(), 3);
  BOOST_CHECK_EQUAL(queue.back(), 3);
  queue.push(8);
  BOOST_CHECK_EQUAL(queue.front(), 3);
  BOOST_CHECK_EQUAL(queue.back(), 8);
  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 8);
}

BOOST_AUTO_TEST_CASE(CopyConstructorOfQueueTest)
{
  kuznetsov::Queue< int > queue1;
  queue1.push(3);
  queue1.push(8);
  kuznetsov::Queue< int > queue2(queue1);
  BOOST_CHECK_EQUAL(queue1.size(), queue2.size());
  BOOST_CHECK_EQUAL(queue1.front(), queue2.front());
  queue1.pop();
  BOOST_CHECK(queue1.size() != queue2.size());
  BOOST_CHECK(queue1.front() != queue2.front());
}

BOOST_AUTO_TEST_CASE(MoveConstructorOfQueueTest)
{
  kuznetsov::Queue< int > queue1;
  queue1.push(3);
  queue1.push(8);
  kuznetsov::Queue< int > queue2(std::move(queue1));
  BOOST_CHECK(queue1.empty());
  BOOST_CHECK(2 == queue2.size());
  BOOST_CHECK(3 == queue2.front());
  BOOST_CHECK(8 == queue2.back());
}

BOOST_AUTO_TEST_CASE(CopyAssignmentOperatorOfQueueTest)
{
  kuznetsov::Queue< int > queue1;
  queue1.push(3);
  queue1.push(8);
  kuznetsov::Queue< int > queue2;
  queue2 = queue1;
  BOOST_CHECK_EQUAL(queue1.size(), queue2.size());
  BOOST_CHECK_EQUAL(queue1.front(), queue2.front());
  queue1.pop();
  BOOST_CHECK(queue1.size() != queue2.size());
  BOOST_CHECK(queue1.front() != queue2.front());
}

BOOST_AUTO_TEST_CASE(MoveAssignmentOperatorOfQueueTest)
{
  kuznetsov::Queue< int > queue1;
  queue1.push(3);
  queue1.push(8);
  kuznetsov::Queue< int > queue2;
  queue2 = std::move(queue1);
  BOOST_CHECK(queue1.empty());
  BOOST_CHECK(2 == queue2.size());
  BOOST_CHECK(3 == queue2.front());
  BOOST_CHECK(8 == queue2.back());
}

