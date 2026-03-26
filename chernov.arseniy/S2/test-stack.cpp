#include <boost/test/unit_test.hpp>
#include <stack.hpp>

BOOST_AUTO_TEST_SUITE(stack_special_members_tests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  chernov::Stack< int > stack;
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  chernov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  chernov::Stack< int > stack2(stack1);
  BOOST_CHECK_EQUAL(stack2.size(), 2);
  BOOST_CHECK_EQUAL(stack2.top(), 2);

  stack2.push(3);
  BOOST_CHECK_EQUAL(stack1.size(), 2);
  BOOST_CHECK_EQUAL(stack2.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  chernov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  chernov::Stack<int> stack2(std::move(stack1));
  BOOST_CHECK_EQUAL(stack2.size(), 2);

  BOOST_CHECK_EQUAL(stack1.size(), 0);
  BOOST_CHECK(stack1.empty());

  stack2.push(3);
  BOOST_CHECK_EQUAL(stack2.size(), 3);
  BOOST_CHECK_EQUAL(stack1.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
  chernov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  stack1 = stack1;
  BOOST_CHECK_EQUAL(stack1.size(), 2);

  chernov::Stack< int > stack2;
  stack2 = stack1;
  BOOST_CHECK_EQUAL(stack2.size(), 2);

  stack2.push(3);
  BOOST_CHECK_EQUAL(stack1.size(), 2);
  BOOST_CHECK_EQUAL(stack2.size(), 3);

  stack1.clear();
  BOOST_CHECK_EQUAL(stack1.size(), 0);
  stack2 = stack1;
  BOOST_CHECK_EQUAL(stack2.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_operator)
{
  chernov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  stack1 = stack1;
  BOOST_CHECK_EQUAL(stack1.size(), 2);

  chernov::Stack< int > stack2;
  stack2 = std::move(stack1);
  BOOST_CHECK_EQUAL(stack2.size(), 2);

  BOOST_CHECK_EQUAL(stack1.size(), 0);
  BOOST_CHECK(stack1.empty());

  stack2.push(3);
  BOOST_CHECK_EQUAL(stack2.size(), 3);
  BOOST_CHECK_EQUAL(stack1.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(stack_capacity_tests)

BOOST_AUTO_TEST_CASE(test_top)
{
  chernov::Stack< int > stack;
  stack.push(1);
  BOOST_CHECK_EQUAL(stack.top(), 1);

  stack.push(2);
  BOOST_CHECK_EQUAL(stack.top(), 2);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 1);
}

BOOST_AUTO_TEST_CASE(test_const_top)
{
  chernov::Stack< int > stack;
  stack.push(1);

  const chernov::Stack< int > & const_stack = stack;
  BOOST_CHECK_EQUAL(const_stack.top(), 1);
}

BOOST_AUTO_TEST_CASE(test_size)
{
  chernov::Stack< int > stack;
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(1);
  BOOST_CHECK_EQUAL(stack.size(), 1);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(2);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  chernov::Stack< int > stack;
  BOOST_CHECK(stack.empty());

  stack.push(1);
  BOOST_CHECK(!stack.empty());

  stack.push(2);
  stack.clear();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(stack_modifiers_tests)

BOOST_AUTO_TEST_CASE(test_push)
{
  chernov::Stack< int > stack;

  stack.push(1);
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.top(), 1);

  stack.push(2);
  stack.push(3);
  BOOST_CHECK_EQUAL(stack.size(), 3);
  BOOST_CHECK_EQUAL(stack.top(), 3);

  int value = 4;
  stack.push(value);
  BOOST_CHECK_EQUAL(stack.size(), 4);
  BOOST_CHECK_EQUAL(stack.top(), 4);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  chernov::Stack< int > stack;

  stack.push(1);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(2);
  stack.push(3);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 1);
  BOOST_CHECK_EQUAL(stack.top(), 2);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 0);
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  chernov::Stack< int > stack;

  stack.clear();
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(1);
  stack.clear();
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(2);
  stack.push(3);
  stack.push(4);
  stack.clear();
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.clear();
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_lifo_order)
{
  chernov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);

  BOOST_CHECK_EQUAL(stack.top(), 3);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 2);
  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 1);
  stack.pop();
}

BOOST_AUTO_TEST_SUITE_END()
