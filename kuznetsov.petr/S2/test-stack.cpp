#include <boost/test/unit_test.hpp>
#include "../common/includes/stack.hpp"

BOOST_AUTO_TEST_CASE(EmptyStackTest)
{
  kuznetsov::Stack< int > stack;
  BOOST_CHECK(stack.empty());
  BOOST_CHECK_THROW(stack.top(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(AddElementToStackTest)
{
  kuznetsov::Stack< int > stack;
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  stack.push(4);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(PopElementFromStackTest)
{
  kuznetsov::Stack< int > stack;
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(getAccessToTopOfStackTest)
{
  kuznetsov::Stack< int > stack;
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
  stack.push(8);
  BOOST_CHECK_EQUAL(stack.top(), 8);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 3);
}

BOOST_AUTO_TEST_CASE(CopyConstructorOfStackTest)
{
  kuznetsov::Stack< int > stack1;
  stack1.push(3);
  stack1.push(8);
  kuznetsov::Stack< int > stack2(stack1);
  BOOST_CHECK_EQUAL(stack1.size(), stack2.size());
  BOOST_CHECK_EQUAL(stack1.top(), stack2.top());
  stack1.pop();
  BOOST_CHECK(stack1.size() != stack2.size());
  BOOST_CHECK(stack1.top() != stack2.top());
}

BOOST_AUTO_TEST_CASE(MoveConstructorOfStackTest)
{
  kuznetsov::Stack< int > stack1;
  stack1.push(3);
  stack1.push(8);
  kuznetsov::Stack< int > stack2(std::move(stack1));
  BOOST_CHECK(stack1.empty());
  BOOST_CHECK(2 == stack2.size());
  BOOST_CHECK(8 == stack2.top());
  stack2.pop();
  BOOST_CHECK(3 == stack2.top());
}

BOOST_AUTO_TEST_CASE(CopyAssignmentOperatorOfStackTest)
{
  kuznetsov::Stack< int > stack1;
  stack1.push(3);
  stack1.push(8);
  kuznetsov::Stack< int > stack2;
  stack2 = stack1;
  BOOST_CHECK(stack1.size() == stack2.size());
  BOOST_CHECK(stack1.top() == stack2.top());
  stack1.pop();
  BOOST_CHECK(stack1.size() != stack2.size());
  BOOST_CHECK(stack1.top() != stack2.top());
}

BOOST_AUTO_TEST_CASE(MoveAssignmentOperatorOfStackTest)
{
  kuznetsov::Stack< int > stack1;
  stack1.push(3);
  stack1.push(8);
  kuznetsov::Stack< int > stack2;
  stack2 = std::move(stack1);
  BOOST_CHECK(stack1.empty());
  BOOST_CHECK(2 == stack2.size());
  BOOST_CHECK(8 == stack2.top());
  stack2.pop();
  BOOST_CHECK(3 == stack2.top());
}

