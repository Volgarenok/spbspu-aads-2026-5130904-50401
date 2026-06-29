#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stackTesting)
BOOST_AUTO_TEST_CASE(defaultConstruct)
{
  donkeev::Stack< int > stack;
  BOOST_TEST(stack.size() == 0);
  BOOST_TEST(stack.isEmpty());
}
BOOST_AUTO_TEST_CASE(initializerListConstruct)
{
  donkeev::Stack< int > stack{1, 2};
  BOOST_TEST(stack.size() == 2);
  BOOST_TEST(stack.top() == 2);
}

BOOST_AUTO_TEST_CASE(pushTest)
{
  donkeev::Stack< int > stack;
  stack.push(2);
  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(stack.top() == 2);
}
BOOST_AUTO_TEST_CASE(popTest)
{
  donkeev::Stack< int > stack{1, 2};
  stack.pop();
  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(stack.top() == 1);
}

BOOST_AUTO_TEST_CASE(getTopValue)
{
  donkeev::Stack< int > stack{1};
  BOOST_TEST(stack.top() == 1);
  stack.push(2);
  BOOST_TEST(stack.top() == 2);
}
BOOST_AUTO_TEST_CASE(getConstTopValue)
{
  const donkeev::Stack< int > stack{1, 2};
  BOOST_TEST(stack.top() == 2);
}

BOOST_AUTO_TEST_CASE(isEmptyTest)
{
  donkeev::Stack< int > stack{1};
  stack.pop();
  BOOST_TEST(stack.isEmpty());
}
BOOST_AUTO_TEST_CASE(sizeTest)
{
  donkeev::Stack< int > stack{1};
  BOOST_TEST(stack.size() == 1);
  stack.push(2);
  BOOST_TEST(stack.size() == 2);
}
BOOST_AUTO_TEST_CASE(clearTest)
{
  donkeev::Stack< int > stack{1};
  BOOST_TEST(stack.size() == 1);
  stack.push(2);
  BOOST_TEST(stack.size() == 2);
  stack.clear();
  BOOST_TEST(stack.size() == 0);
  BOOST_TEST(stack.isEmpty());
}

BOOST_AUTO_TEST_CASE(stackDestruct)
{
  donkeev::Stack< int > stack{1, 2};
  BOOST_TEST(stack.size() == 2);
  stack.~Stack();
  BOOST_TEST(stack.size() == 0);
  BOOST_TEST(stack.isEmpty());
}
BOOST_AUTO_TEST_SUITE_END()
