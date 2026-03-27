#include <boost/test/unit_test.hpp>
#include <stack.hpp>

BOOST_AUTO_TEST_CASE(DefaultStackConstructorTest)
{
  zharov::Stack< int > stack;
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(CopyStackConstructorTest)
{
  zharov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);
  BOOST_CHECK_EQUAL(stack1.top(), 3);
  zharov::Stack< int > stack2 = zharov::Stack<int>(stack1);
  BOOST_CHECK_EQUAL(stack2.top(), 3);
}

BOOST_AUTO_TEST_CASE(MoveStackConstructorTest)
{
  zharov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);
  stack1.push(3);
  BOOST_CHECK_EQUAL(stack1.top(), 3);
  zharov::Stack< int > stack2 = zharov::Stack<int>(std::move(stack1));
  BOOST_CHECK_EQUAL(stack2.top(), 3);
  BOOST_CHECK_EQUAL(stack1.size(), 0);
}

BOOST_AUTO_TEST_CASE(StackDestructorTest)
{
  zharov::Stack< int > stack;
  for (size_t i = 0; i < 10; ++i) {
    stack.push(i);
  }
  BOOST_CHECK_EQUAL(stack.size(), 10);
}

BOOST_AUTO_TEST_CASE(StackCopyOperatorTest)
{
  zharov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(3);
  BOOST_CHECK_EQUAL(stack1.top(), 3);
  zharov::Stack< int > stack2;
  stack2.push(11);
  stack2.push(33);
  BOOST_CHECK_EQUAL(stack2.top(), 33);
  stack2 = stack1;
  BOOST_CHECK_EQUAL(stack2.top(), 3);
}

BOOST_AUTO_TEST_CASE(StackMoveOperatorTest)
{
  zharov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(3);
  BOOST_CHECK_EQUAL(stack1.top(), 3);
  zharov::Stack< int > stack2;
  stack2.push(11);
  stack2.push(33);
  BOOST_CHECK_EQUAL(stack2.top(), 33);
  stack2 = std::move(stack1);
  BOOST_CHECK_EQUAL(stack2.top(), 3);
  BOOST_CHECK_EQUAL(stack1.size(), 0);
}

BOOST_AUTO_TEST_CASE(StackFrontTest)
{
  zharov::Stack< int > stack;
  stack.push(1);
  stack.push(3);
  const zharov::Stack< int > stack2(stack);
}

BOOST_AUTO_TEST_CASE(StackBackTest)
{
  zharov::Stack< int > stack;
  stack.push(1);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
  const zharov::Stack< int > stack2(stack);
  BOOST_CHECK_EQUAL(stack2.top(), 3);
}

BOOST_AUTO_TEST_CASE(StackpushTest)
{
  zharov::Stack< int > stack;
  stack.push(1);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(StackDropTest)
{
  zharov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  int n = stack.drop();
  BOOST_CHECK_EQUAL(n, 3);
  BOOST_CHECK_EQUAL(stack.top(), 2);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(StackClearTest)
{
  zharov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 3);
  stack.clear();
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(StackSizeTest)
{
  zharov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 3);
}

BOOST_AUTO_TEST_CASE(StackEmptyTest)
{
  zharov::Stack< int > stack;
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(StackSwapTest)
{
  zharov::Stack< int > stack1;
  stack1.push(10);
  stack1.push(20);
  stack1.push(30);
  zharov::Stack< int > stack2;
  stack2.push(100);
  stack2.push(200);
  BOOST_CHECK_EQUAL(stack1.size(), 3);
  BOOST_CHECK_EQUAL(stack1.top(), 30);
  BOOST_CHECK_EQUAL(stack2.size(), 2);
  BOOST_CHECK_EQUAL(stack2.top(), 200);
  stack1.swap(stack2);
  BOOST_CHECK_EQUAL(stack1.size(), 2);
  BOOST_CHECK_EQUAL(stack1.top(), 200);
  BOOST_CHECK_EQUAL(stack2.size(), 3);
  BOOST_CHECK_EQUAL(stack2.top(), 30);
}
