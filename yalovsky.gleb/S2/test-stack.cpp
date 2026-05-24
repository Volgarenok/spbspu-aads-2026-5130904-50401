#include <boost/test/unit_test.hpp>

#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTest)

BOOST_AUTO_TEST_CASE(constructAndOperators)
{
  yalovsky::Stack< int > stack1;
  BOOST_TEST(stack1.empty());

  stack1.push(1);
  yalovsky::Stack< int > stack2(stack1);
  BOOST_TEST(stack2.size() == 1);
  BOOST_TEST(stack2.top() == 1);

  stack2.push(2);
  yalovsky::Stack< int > stack3(std::move(stack2));
  BOOST_TEST(stack3.size() == 2);
  BOOST_TEST(stack3.top() == 2);

  stack3.push(3);
  yalovsky::Stack< int > stack4 = stack3;
  BOOST_TEST(stack4.size() == 3);
  BOOST_TEST(stack4.top() == 3);

  yalovsky::Stack< int > stack5 = std::move(stack4);
  BOOST_TEST(stack5.size() == 3);
  BOOST_TEST(stack5.top() == 3);
}

BOOST_AUTO_TEST_CASE(emptyAndSize)
{
  yalovsky::Stack< int > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);

  stack.push(1);
  stack.push(2);
  BOOST_TEST(!stack.empty());
  BOOST_TEST(stack.size() == 2);
}

BOOST_AUTO_TEST_CASE(pushPopTopAndDrop)
{
  yalovsky::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);

  BOOST_TEST(stack.top() == 3);
  BOOST_TEST(stack.drop() == 3);
  BOOST_TEST(stack.top() == 2);

  stack.pop();
  BOOST_TEST(stack.top() == 1);
}

BOOST_AUTO_TEST_CASE(swapAndClear)
{
  yalovsky::Stack< int > stack1;
  stack1.push(1);

  yalovsky::Stack< int > stack2;
  stack2.push(2);
  stack2.push(3);

  stack1.swap(stack2);
  BOOST_TEST(stack1.top() == 3);
  BOOST_TEST(stack1.size() == 2);
  BOOST_TEST(stack2.top() == 1);
  BOOST_TEST(stack2.size() == 1);

  stack1.clear();
  BOOST_TEST(stack1.empty());
}

BOOST_AUTO_TEST_SUITE_END()
