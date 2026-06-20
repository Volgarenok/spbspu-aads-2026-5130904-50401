#include <boost/test/unit_test.hpp>
#include <stack.hpp>

BOOST_AUTO_TEST_SUITE(stack_suite)

BOOST_AUTO_TEST_CASE(test_create_stack)
{
  hvostov::Stack< int > stack;
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_push_element)
{
  hvostov::Stack< int > stack;
  stack.push(6);
  BOOST_CHECK(!stack.empty());
  BOOST_CHECK(stack.top() == 6);
  stack.push(7);
  BOOST_CHECK(stack.top() == 7);
  stack.push(8);
  BOOST_CHECK(stack.top() == 8);
}

BOOST_AUTO_TEST_CASE(test_pop_element)
{
  hvostov::Stack< int > stack;
  stack.push(6);
  stack.push(7);
  stack.push(8);
  BOOST_CHECK(stack.top() == 8);
  stack.pop();
  BOOST_CHECK(stack.top() == 7);
  stack.pop();
  BOOST_CHECK(stack.top() == 6);
  stack.pop();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::Stack< int > stack;
  stack.push(6);
  stack.push(7);
  stack.push(8);
  BOOST_CHECK(!stack.empty());
  stack.clear();
  BOOST_CHECK(stack.empty());
  stack.push(6);
  stack.push(7);
  BOOST_CHECK(!stack.empty());
  stack.clear();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  hvostov::Stack< int > stack;
  BOOST_CHECK_EQUAL(stack.size(), 0);

  stack.push(1);
  BOOST_CHECK_EQUAL(stack.size(), 1);

  stack.push(2);
  BOOST_CHECK_EQUAL(stack.size(), 2);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.size(), 1);

  stack.clear();
  BOOST_CHECK_EQUAL(stack.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_const_methods)
{
  hvostov::Stack< int > stack;
  stack.push(42);
  stack.push(43);

  const hvostov::Stack< int >& const_stack = stack;
  BOOST_CHECK_EQUAL(const_stack.top(), 43);
  BOOST_CHECK_EQUAL(const_stack.size(), 2);
  BOOST_CHECK(!const_stack.empty());
}

BOOST_AUTO_TEST_CASE(test_move_push)
{
  hvostov::Stack< std::string > stack;
  std::string str = "test";
  stack.push(std::move(str));
  BOOST_CHECK_EQUAL(stack.top(), "test");
}

BOOST_AUTO_TEST_CASE(test_copy_and_move_stack)
{
  hvostov::Stack< int > stack1;
  stack1.push(1);
  stack1.push(2);

  hvostov::Stack< int > stack2 = stack1;
  BOOST_CHECK_EQUAL(stack2.top(), 2);
  stack2.pop();
  BOOST_CHECK_EQUAL(stack2.top(), 1);

  hvostov::Stack< int > stack3 = std::move(stack1);
  BOOST_CHECK_EQUAL(stack3.top(), 2);
  BOOST_CHECK(stack1.empty());
}

BOOST_AUTO_TEST_CASE(test_large_stack)
{
  hvostov::Stack< int > stack;
  const int N = 1000;

  for (int i = 0; i < N; ++i) {
    stack.push(i);
  }

  BOOST_CHECK_EQUAL(stack.size(), N);

  for (int i = N - 1; i >= 0; --i) {
    BOOST_CHECK_EQUAL(stack.top(), i);
    stack.pop();
  }

  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(test_top_after_operations)
{
  hvostov::Stack< int > stack;
  stack.push(10);
  stack.push(20);
  stack.push(30);

  BOOST_CHECK_EQUAL(stack.top(), 30);

  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 20);

  stack.push(40);
  BOOST_CHECK_EQUAL(stack.top(), 40);

  stack.pop();
  stack.pop();
  BOOST_CHECK_EQUAL(stack.top(), 10);
}

BOOST_AUTO_TEST_CASE(test_stack_with_different_types)
{
  hvostov::Stack< double > dstack;
  dstack.push(3.14);
  dstack.push(2.718);
  BOOST_CHECK_EQUAL(dstack.top(), 2.718);
  dstack.pop();
  BOOST_CHECK_EQUAL(dstack.top(), 3.14);

  hvostov::Stack< char > cstack;
  cstack.push('A');
  cstack.push('B');
  BOOST_CHECK_EQUAL(cstack.top(), 'B');
}

BOOST_AUTO_TEST_CASE(test_push_after_clear)
{
  hvostov::Stack< int > stack;
  stack.push(1);
  stack.push(2);
  stack.clear();
  BOOST_CHECK(stack.empty());

  stack.push(3);
  BOOST_CHECK_EQUAL(stack.top(), 3);
  BOOST_CHECK_EQUAL(stack.size(), 1);

  stack.push(4);
  BOOST_CHECK_EQUAL(stack.top(), 4);
  BOOST_CHECK_EQUAL(stack.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_stack_emplace)
{
  hvostov::Stack< std::pair< int, std::string > > stack;

  stack.emplace(1, "first");
  stack.emplace(2, "second");
  stack.emplace(3, "third");

  BOOST_CHECK_EQUAL(stack.size(), 3);
  BOOST_CHECK_EQUAL(stack.top().first, 3);
  BOOST_CHECK_EQUAL(stack.top().second, "third");

  stack.pop();
  BOOST_CHECK_EQUAL(stack.top().first, 2);

  stack.pop();
  stack.pop();
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
