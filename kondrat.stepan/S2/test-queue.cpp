#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTestSuite)

BOOST_AUTO_TEST_CASE(CreationOfQueue)
{
  kondrat::Queue< int > q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushFront)
{
  kondrat::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_CHECK(q.front() == 1);
  BOOST_CHECK(q.size() == 3);
  BOOST_CHECK(!q.empty());
}

BOOST_AUTO_TEST_CASE(Clear)
{
  kondrat::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.clear();

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(Pop)
{
  kondrat::Queue< int > q;
  q.push(1);
  q.push(2);

  BOOST_CHECK(q.front() == 1);
  q.pop();
  BOOST_CHECK(q.front() == 2);
  q.pop();

  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(OperatorsConstructors)
{
  kondrat::Queue< int > q;
  q.push(32);
  q.push(42);
  q.push(52);

  kondrat::Queue< int > qCopy(q);
  BOOST_CHECK(qCopy.size() == 3);
  BOOST_CHECK(!qCopy.empty());
  BOOST_CHECK(qCopy.front() == 32);

  kondrat::Queue< int > qMove(std::move(qCopy));
  BOOST_CHECK(qCopy.size() == 0);
  BOOST_CHECK(qCopy.empty());
  BOOST_CHECK(qMove.front() == 32);
  BOOST_CHECK(qMove.size() == 3);

  kondrat::Queue< int > qCopyOperator = qMove;
  BOOST_CHECK(qCopyOperator.size() == 3);
  BOOST_CHECK(qCopyOperator.front() == 32);
  BOOST_CHECK(qMove.size() == 3);

  kondrat::Queue< int > qMoveOperator = std::move(qCopyOperator);
  BOOST_CHECK(qMoveOperator.size() == 3);
  BOOST_CHECK(qCopyOperator.empty());

  q.clear();
  qMove.clear();
  qMoveOperator.clear();
  BOOST_CHECK(qMoveOperator.empty());
  BOOST_CHECK(qMove.empty());
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()
