#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  burukov::Queue< int > q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushAndFront)
{
  burukov::Queue< int > q;
  q.push(1);
  BOOST_CHECK(q.front() == 1);
  q.push(2);
  BOOST_CHECK(q.front() == 1);
  q.push(3);
  BOOST_CHECK(q.front() == 1);
}

BOOST_AUTO_TEST_CASE(PopOrder)
{
  burukov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK(q.front() == 1);
  q.pop();
  BOOST_CHECK(q.front() == 2);
  q.pop();
  BOOST_CHECK(q.front() == 3);
  q.pop();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(FifoOrder)
{
  burukov::Queue< int > q;
  for (int i = 0; i < 10; ++i)
  {
    q.push(i);
  }
  for (int i = 0; i < 10; ++i)
  {
    BOOST_CHECK(q.front() == i);
    q.pop();
  }
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(SizeTracking)
{
  burukov::Queue< int > q;
  BOOST_CHECK(q.size() == 0);
  q.push(1);
  BOOST_CHECK(q.size() == 1);
  q.push(2);
  BOOST_CHECK(q.size() == 2);
  q.pop();
  BOOST_CHECK(q.size() == 1);
}

BOOST_AUTO_TEST_CASE(CopyBehavior)
{
  burukov::Queue< int > q1;
  q1.push(1);
  q1.push(2);
  q1.push(3);
  burukov::Queue< int > q2(q1);
  BOOST_CHECK(q2.size() == 3);
  BOOST_CHECK(q2.front() == 1);
  q2.pop();
  BOOST_CHECK(q2.front() == 2);
  BOOST_CHECK(q1.front() == 1);
}

BOOST_AUTO_TEST_CASE(CopyThenPush)
{
  burukov::Queue< int > q1;
  q1.push(1);
  q1.push(2);
  burukov::Queue< int > q2(q1);
  q2.push(3);
  BOOST_CHECK(q2.size() == 3);
  q2.pop();
  q2.pop();
  BOOST_CHECK(q2.front() == 3);
  BOOST_CHECK(q1.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveBehavior)
{
  burukov::Queue< int > q1;
  q1.push(1);
  q1.push(2);
  burukov::Queue< int > q2(std::move(q1));
  BOOST_CHECK(q2.size() == 2);
  BOOST_CHECK(q2.front() == 1);
  BOOST_CHECK(q1.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  burukov::Queue< int > q1;
  q1.push(10);
  q1.push(20);
  burukov::Queue< int > q2;
  q2.push(99);
  q2 = q1;
  BOOST_CHECK(q2.size() == 2);
  BOOST_CHECK(q2.front() == 10);
  q2.pop();
  BOOST_CHECK(q2.front() == 20);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  burukov::Queue< int > q1;
  q1.push(10);
  q1.push(20);
  burukov::Queue< int > q2;
  q2 = std::move(q1);
  BOOST_CHECK(q2.size() == 2);
  BOOST_CHECK(q2.front() == 10);
  BOOST_CHECK(q1.empty());
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  burukov::Queue< int > q1;
  q1.push(1);
  burukov::Queue< int > q2;
  q2.push(2);
  q2.push(3);
  q2.swap(q1);
  BOOST_CHECK(q1.front() == 2);
  BOOST_CHECK(q1.size() == 2);
  BOOST_CHECK(q2.front() == 1);
  BOOST_CHECK(q2.size() == 1);
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  burukov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK(q.size() == 3);
  q.clear();
  BOOST_CHECK(q.size() == 0);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(StringQueue)
{
  burukov::Queue< std::string > q;
  q.push("first");
  q.push("second");
  q.push("third");
  BOOST_CHECK(q.front() == "first");
  q.pop();
  BOOST_CHECK(q.front() == "second");
  q.pop();
  BOOST_CHECK(q.front() == "third");
}

BOOST_AUTO_TEST_CASE(PopAllThenPush)
{
  burukov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.pop();
  q.pop();
  BOOST_CHECK(q.empty());
  q.push(3);
  BOOST_CHECK(q.front() == 3);
  BOOST_CHECK(q.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
