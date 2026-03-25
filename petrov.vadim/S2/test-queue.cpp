#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(Queue_test_suite)

BOOST_AUTO_TEST_CASE(constructors_and_operators)
{
  petrov::Queue< int > q;
  BOOST_CHECK(q.empty());
  q.push(1);
  petrov::Queue< int > qCopy(q);
  BOOST_CHECK(qCopy.size() == 1);
  BOOST_CHECK(qCopy.front() == 1);
  qCopy.push(2);
  petrov::Queue< int > qMove(std::move(qCopy));
  BOOST_CHECK(qMove.size() == 2);
  BOOST_CHECK(qMove.front() == 1);
  qMove.push(3);
  qMove.push(4);
  petrov::Queue< int > qCopyOp = qMove;
  BOOST_CHECK(qCopyOp.size() == 4);
  BOOST_CHECK(qCopyOp.front() == 1);

  petrov::Queue< int > qMoveOp = std::move(qCopyOp);
  BOOST_CHECK(qMoveOp.size() == 4);
  BOOST_CHECK(qMoveOp.front() == 1);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  petrov::Queue< int > q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);

  q.push(1);

  BOOST_CHECK(!q.empty());
  BOOST_CHECK(q.size() == 1);
}

BOOST_AUTO_TEST_CASE(push_pop_front)
{
  petrov::Queue< int > q;
  q.push(1);
  BOOST_CHECK(q.front() == 1);

  q.push(2);
  BOOST_CHECK(q.front() == 1);

  q.push(3);
  BOOST_CHECK(q.front() == 1);

  q.push(4);
  BOOST_CHECK(q.front() == 1);


  q.pop();
  BOOST_CHECK(q.front() == 2);

  q.pop();
  BOOST_CHECK(q.front() == 3);

  q.pop();
  BOOST_CHECK(q.front() == 4);
}

BOOST_AUTO_TEST_CASE(drop)
{
  petrov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_CHECK(q.drop() == 1);
  BOOST_CHECK(q.size() == 2);
  BOOST_CHECK(q.front() == 2);

  BOOST_CHECK(q.drop() == 2);
  BOOST_CHECK(q.size() == 1);
  BOOST_CHECK(q.front() == 3);

  BOOST_CHECK(q.drop() == 3);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(clear)
{
  petrov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);
  q.push(5);

  BOOST_CHECK(q.size() == 5);

  q.clear();

  BOOST_CHECK(q.size() == 0);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()
