#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  burukov::Stack< int > s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushAndTop)
{
  burukov::Stack< int > s;
  s.push(1);
  BOOST_CHECK(s.top() == 1);
  s.push(2);
  BOOST_CHECK(s.top() == 2);
  s.push(3);
  BOOST_CHECK(s.top() == 3);
}

BOOST_AUTO_TEST_CASE(PopOrder)
{
  burukov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.pop();
  BOOST_CHECK(s.top() == 2);
  s.pop();
  BOOST_CHECK(s.top() == 1);
  s.pop();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(SizeTracking)
{
  burukov::Stack< int > s;
  BOOST_CHECK(s.size() == 0);
  s.push(1);
  BOOST_CHECK(s.size() == 1);
  s.push(2);
  BOOST_CHECK(s.size() == 2);
  s.pop();
  BOOST_CHECK(s.size() == 1);
}

BOOST_AUTO_TEST_CASE(CopyBehavior)
{
  burukov::Stack< int > s1;
  s1.push(1);
  s1.push(2);
  burukov::Stack< int > s2(s1);
  BOOST_CHECK(s2.size() == 2);
  BOOST_CHECK(s2.top() == 2);
  s2.pop();
  BOOST_CHECK(s2.top() == 1);
  BOOST_CHECK(s1.top() == 2);
}

BOOST_AUTO_TEST_CASE(MoveBehavior)
{
  burukov::Stack< int > s1;
  s1.push(1);
  s1.push(2);
  burukov::Stack< int > s2(std::move(s1));
  BOOST_CHECK(s2.size() == 2);
  BOOST_CHECK(s2.top() == 2);
  BOOST_CHECK(s1.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  burukov::Stack< int > s1;
  s1.push(10);
  s1.push(20);
  burukov::Stack< int > s2;
  s2.push(99);
  s2 = s1;
  BOOST_CHECK(s2.size() == 2);
  BOOST_CHECK(s2.top() == 20);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  burukov::Stack< int > s1;
  s1.push(10);
  s1.push(20);
  burukov::Stack< int > s2;
  s2 = std::move(s1);
  BOOST_CHECK(s2.size() == 2);
  BOOST_CHECK(s2.top() == 20);
  BOOST_CHECK(s1.empty());
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  burukov::Stack< int > s1;
  s1.push(1);
  burukov::Stack< int > s2;
  s2.push(2);
  s2.push(3);
  s2.swap(s1);
  BOOST_CHECK(s1.top() == 3);
  BOOST_CHECK(s1.size() == 2);
  BOOST_CHECK(s2.top() == 1);
  BOOST_CHECK(s2.size() == 1);
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  burukov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_CHECK(s.size() == 3);
  s.clear();
  BOOST_CHECK(s.size() == 0);
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(StringStack)
{
  burukov::Stack< std::string > s;
  s.push("hello");
  s.push("world");
  BOOST_CHECK(s.top() == "world");
  s.pop();
  BOOST_CHECK(s.top() == "hello");
}

BOOST_AUTO_TEST_SUITE_END()
