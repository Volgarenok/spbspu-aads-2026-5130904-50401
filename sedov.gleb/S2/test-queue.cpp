#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTest)

BOOST_AUTO_TEST_CASE(ConstructAndOperatorsTests)
{
  sedov::Queue< int > q1;
  BOOST_CHECK(q.empty());
  q.push(1);
  sedov::Queue< int > q2(q1);
  BOOST_CHECK(q2.size() == 1);
  BOOST_CHECK(q2.front() == 1);
  BOOST_CHECK(q2.back() == 1);
  q2.push(2);
  sedov::Queue< int > q3(std::move(q2));
  BOOST_CHECK(q3.size() == 2);
  BOOST_CHECK(q3.front() == 1);
  BOOST_CHECK(q3.back() == 2);
  q3.push(3);
  sedov::Queue< int > q4 = q3;
  BOOST_CHECK(q4.size() == 3);
  BOOST_CHECK(q4.front() == 1);
  BOOST_CHECK(q4.back() == 3);
  sedov::Queue< int > q5 = std::move(q4);
  BOOST_CHECK(q5.size() == 3);
  BOOST_CHECK(q5.front() == 1);
  BOOST_CHECK(q5.back() == 3);
}

BOOST_AUTO_TEST_CASE(EmptyAndSizeTests)
{
  sedov::Queue< int > q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
  q.push(1);
  q.push(2);
  BOOST_CHECK(!q.empty());
  BOOST_CHECK(q.size() == 2);
}

BOOST_AUTO_TEST_CASE(PushPopFrontBackTests)
{
  sedov::Queue< int > q;
  q.push(1);
  BOOST_CHECK(q.back() == 1);
  q.push(2);
  BOOST_CHECK(q.back() == 2);
  q.push(3);
  BOOST_CHECK(q.back() == 3);
  q.pop();
  BOOST_CHECK(q.front() == 2);
  q.pop();
  BOOST_CHECK(q.front() == 3);
}

BOOST_AUTO_TEST_CASE(SwapTests)
{
  sedov::Queue< int > q1;
  q1.push(1);
  sedov::Queue< int > q2;
  q2.push(2);
  q2.push(2);
  q2.swap(q1);
  BOOST_CHECK(q1.front() == 2);
  BOOST_CHECK(q2.front() == 1);
  BOOST_CHECK(q1.size() == 2);
  BOOST_CHECK(q2.size() == 1);
  BOOST_CHECK(q1.back() == 3);
}

BOOST_AUTO_TEST_CASE(ClearTests)
{
  sedov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK(q.size() == 3);
  q.clear();
  BOOST_CHECK(q.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()
