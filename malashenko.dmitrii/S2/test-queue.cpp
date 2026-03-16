#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(Queue_test_suite)
BOOST_AUTO_TEST_CASE(constructors_and_operators)
{
  malashenko::Queue< int > q;
  BOOST_CHECK(q.empty());
  q.push(1);
  malashenko::Queue< int > qCopy(q);
  BOOST_CHECK(qCopy.size() == 1);
  BOOST_CHECK(qCopy.back() == 1);
  BOOST_CHECK(qCopy.front() == 1);

  qCopy.push(2);
  malashenko::Queue< int > qMove(std::move(qCopy));
  BOOST_CHECK(qMove.size() == 2);
  BOOST_CHECK(qMove.back() == 2);
  BOOST_CHECK(qMove.front() == 1);
  qMove.push(3);
  qMove.push(4);

  malashenko::Queue< int > qCopyOp = qMove;
  BOOST_CHECK(qCopyOp.size() == 4);
  BOOST_CHECK(qCopyOp.back() == 4);
  BOOST_CHECK(qCopyOp.front() == 1);

  malashenko::Queue< int > qMoveOp = std::move(qCopyOp);
  BOOST_CHECK(qMoveOp.size() == 4);
  BOOST_CHECK(qMoveOp.back() == 4);
  BOOST_CHECK(qMoveOp.front() == 1);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  malashenko::Queue< int > q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);

  q.push(1);

  BOOST_CHECK(!q.empty());
  BOOST_CHECK(q.size() == 1);
}

BOOST_AUTO_TEST_CASE(push_pop_front_back)
{
  malashenko::Queue< int > q;
  q.push(1);
  BOOST_CHECK(q.back() == 1);

  q.push(2);
  BOOST_CHECK(q.back() == 2);

  q.push(3);
  BOOST_CHECK(q.back() == 3);

  q.push(4);
  BOOST_CHECK(q.back() == 4);


  q.pop();
  BOOST_CHECK(q.front() == 2);

  q.pop();
  BOOST_CHECK(q.front() == 3);

  q.pop();
  BOOST_CHECK(q.front() == 4);
}

BOOST_AUTO_TEST_CASE(swap)
{
  malashenko::Queue< int > q1;
  q1.push(1);

  malashenko::Queue< int > q2;
  q2.push(9);

  q2.swap(q1);

  BOOST_CHECK(q1.front() == 9);
  BOOST_CHECK(q2.front() == 1);
}

BOOST_AUTO_TEST_CASE(clear)
{
  malashenko::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);
  q.push(5);

  BOOST_CHECK(q.size() == 5);

  q.clear();

  BOOST_CHECK(q.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()
