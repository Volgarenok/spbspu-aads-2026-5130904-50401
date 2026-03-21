#include "boost/test/unit_test.hpp"
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_push)
{
  tarasenko::Stack< int > stack;
  BOOST_TEST(stack.empty());
  BOOST_TEST(stack.size() == 0);
  stack.push(5);
  BOOST_TEST(stack.size() == 1);
  BOOST_TEST(stack.top() == 5);
  const int& num1 = 3;
  stack.push(num1);
  BOOST_TEST(stack.top() == 3);
  const int& num2 = 2;
  stack.push(std::move(num2));
  BOOST_TEST(stack.top() == 2);
  stack.clear();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_CASE(stack_pop)
{
  tarasenko::Stack< int > stack;
  for (size_t i = 0; i < 10; ++i)
  {
    stack.push(i);
  }
  for (size_t i = 0; i < 10; ++i)
  {
    BOOST_TEST(stack.pop() == 9 - i);
    BOOST_TEST(stack.size() == 10 - (i + 1));
  }
}

BOOST_AUTO_TEST_CASE(stack_copy_and_move_semantic)
{
  tarasenko::Stack< int > stack;
  for (size_t i = 0; i < 10; ++i)
  {
    stack.push(i);
  }
  tarasenko::Stack< int > copy = stack;
  BOOST_TEST(copy.top() == 9);
  tarasenko::Stack< int > copy2;
  copy2 = stack;
  BOOST_TEST(copy2.top() == 9);
  tarasenko::Stack< int > moved = std::move(stack);
  BOOST_TEST(moved.top() == 9);
  tarasenko::Stack< int > moved2;
  moved2 = std::move(moved);
  BOOST_TEST(moved2.top() == 9);
}
