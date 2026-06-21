#include <boost/test/unit_test.hpp>
#include "queue.hpp"

using namespace lukashevich;

BOOST_AUTO_TEST_SUITE(QueueTestSuite)

BOOST_AUTO_TEST_CASE(CreationOfQueue)
{
  Queue< int > q;

  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushFirstLast)
{
  Queue< int > q;

  q.push(1);
  q.push(2);
  q.push(3);

  BOOST_CHECK(q.front() == 1);
  BOOST_CHECK(q.back() == 3);
  BOOST_CHECK(q.size() == 3);
  BOOST_CHECK(!q.empty());
}

BOOST_AUTO_TEST_CASE(Clear)
{
  Queue< int > q;

  q.push(1);
  q.push(2);
  q.push(3);
  q.clear();

  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(Drop)
{
  Queue< int > q;
  q.push(1);
  int res = q.front();
  q.pop();

  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
  BOOST_CHECK(res == 1);
}
BOOST_AUTO_TEST_CASE(OperatorsConstructors)
{
  Queue< int > q;
  q.push(32);
  q.push(42);
  q.push(52);
  Queue< int > qCopy(q);

  BOOST_CHECK(qCopy.size() == 3);
  BOOST_CHECK(!qCopy.empty());
  BOOST_CHECK(qCopy.front() == 32);
  BOOST_CHECK(qCopy.back() == 52);

  Queue< int > qMove(std::move(qCopy));

  BOOST_CHECK(qCopy.size() == 0);
  BOOST_CHECK(qCopy.empty());
  BOOST_CHECK(qMove.front() == 32);
  BOOST_CHECK(qMove.back() == 52);
  BOOST_CHECK(qMove.size() == 3);

  Queue< int > qCopyOperator = qMove;

  BOOST_CHECK(qCopyOperator.size() == 3);
  BOOST_CHECK(qCopyOperator.front() == 32);
  BOOST_CHECK(qCopyOperator.back() == 52);
  BOOST_CHECK(qMove.size() == 3);

  Queue< int > qMoveOperator = std::move(qCopyOperator);

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
