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

  BOOST_CHECK(q.first() == 1);
  BOOST_CHECK(q.last() == 3);
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
  int res = q.drop();

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
  BOOST_CHECK(qCopy.first() == 32);
  BOOST_CHECK(qCopy.last() == 52);

  Queue< int > qMove(std::move(qCopy));

  BOOST_CHECK(qCopy.size() == 0);
  BOOST_CHECK(qCopy.empty());
  BOOST_CHECK(qMove.first() == 32);
  BOOST_CHECK(qMove.last() == 52);
  BOOST_CHECK(qMove.size() == 3);

  Queue< int > qCopyOperator = qMove;

  BOOST_CHECK(qCopyOperator.size() == 3);
  BOOST_CHECK(qCopyOperator.first() == 32);
  BOOST_CHECK(qCopyOperator.last() == 52);
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
