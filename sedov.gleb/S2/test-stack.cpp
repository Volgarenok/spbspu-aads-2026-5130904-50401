#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTest)

BOOST_AUTO_TEST_CASE(ConstructAndOperatorsTests)
{
  sedov::Stack< int > s1;
  BOOST_CHECK(s1.empty());
  s1.push(1);
  sedov::Stack< int > s2(s1);
  BOOST_CHECK(s2.size() == 1);
  BOOST_CHECK(s2.top() == 1);
  s2.push(2);
  sedov::Stack< int > s3(std::move(s2));
  BOOST_CHECK(s3.size() == 2);
  BOOST_CHECK(s3.top() == 2);
  s3.push(3);
  sedov::Stack< int > s4 = s3;
  BOOST_CHECK(s4.size() == 3);
  BOOST_CHECK(s4.top() == 3);
  sedov::Stack< int > s5 = std::move(s4);
  BOOST_CHECK(s5.size() == 3);
  BOOST_CHECK(s5.top() == 3);
}

BOOST_AUTO_TEST_CASE(EmptyAndSizeTests)
{
  sedov::Stack< int > s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK(s.size() == 0);
  s.push(1);
  s.push(2);
  BOOST_CHECK(!s.empty());
  BOOST_CHECK(s.size() == 2);
}

BOOST_AUTO_TEST_CASE(PushPopTopTests)
{
  sedov::Stack< int > s;
  s.push(1);
  BOOST_CHECK(s.top() == 1);
  s.push(2);
  BOOST_CHECK(s.top() == 2);
  s.push(3);
  BOOST_CHECK(s.top() == 3);
  s.pop();
  BOOST_CHECK(s.top() == 2);
  s.pop();
  BOOST_CHECK(s.top() == 1);
}

BOOST_AUTO_TEST_CASE(SwapTests)
{
  sedov::Stack< int > s1;
  s1.push(1);
  sedov::Stack< int > s2;
  s2.push(2);
  s2.push(3);
  s2.swap(s1);
  BOOST_CHECK(s1.top() == 3);
  BOOST_CHECK(s2.top() == 1);
  BOOST_CHECK(s1.size() == 2);
  BOOST_CHECK(s2.size() == 1);
}

BOOST_AUTO_TEST_CASE(ClearTests)
{
  sedov::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_CHECK(s.size() == 3);
  s.clear();
  BOOST_CHECK(s.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()
