#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(stackTest)

BOOST_AUTO_TEST_CASE(default_stack_constructor)
{
  madieva::Stack< int > s;
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(add__top_a_few_items)
{
  madieva::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_CHECK(s.size() == 3);
  BOOST_CHECK(!s.empty());
}

BOOST_AUTO_TEST_CASE(deleting__items)
{
  madieva::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.pop();
  s.pop();
  s.pop();
  BOOST_CHECK(s.size() == 0);
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(checking_the_top_element_without_deleting_it)
{
  madieva::Stack< int > s;
  s.push(1);
  BOOST_CHECK(s.top() == 1);
  s.push(2);
  BOOST_CHECK(s.top() == 2);
  s.push(3);
  BOOST_CHECK(s.top() == 3);
  BOOST_CHECK(s.size() == 3);
  BOOST_CHECK(!s.empty());
}

BOOST_AUTO_TEST_CASE(checking_the_top_item_from_deletion)
{
  madieva::Stack< int > s;
  s.push(1);
  BOOST_CHECK(s.top() == 1);
  s.push(2);
  BOOST_CHECK(s.top() == 2);
  s.push(3);
  BOOST_CHECK(s.top() == 3);
  BOOST_CHECK(s.size() == 3);
  BOOST_CHECK(!s.empty());
  s.pop();
  BOOST_CHECK(s.top() == 2);
  s.pop();
  BOOST_CHECK(s.top() == 1);
  s.pop();
  BOOST_CHECK(s.size() == 0);
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(access_to_a_constant_element)
{
  madieva::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  const madieva::Stack< int > cs = s;

  const int & a = cs.top();
  BOOST_CHECK(a == 3);
}

BOOST_AUTO_TEST_CASE(top_when_the_stack_is_empty)
{
  madieva::Stack< int > s;
  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(pop_when_the_stack_is_empty)
{
  madieva::Stack< int > s;
  s.pop();
  BOOST_CHECK(s.empty());
  BOOST_CHECK(s.size() == 0);
}















BOOST_AUTO_TEST_CASE(clear_stack)
{
  madieva::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  s.clear();
  BOOST_CHECK(s.empty());
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear_empty_stack)
{
  madieva::Stack< int > s;
  s.clear();
  BOOST_CHECK(s.empty());
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(size_after_operations)
{
  madieva::Stack<int> s;
  BOOST_CHECK(s.size() == 0);
  s.push(10);
  BOOST_CHECK(s.size() == 1);
  s.push(20);
  BOOST_CHECK(s.size() == 2);
  s.pop();
  BOOST_CHECK(s.size() == 1);
  s.clear();
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(empty_on_const_stack)
{
  madieva::Stack<int> s;
  s.push(1);
  const madieva::Stack<int>& cs = s;
  BOOST_CHECK(!cs.empty());
  BOOST_CHECK(s.top() == 1);
  BOOST_CHECK(cs.top() == 1);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  madieva::Stack<int> s1;
  s1.push(1);
  s1.push(2);
  madieva::Stack<int> s2(s1);
  BOOST_CHECK(s2.size() == 2);
  BOOST_CHECK(s2.top() == 2);
  s2.pop();
  BOOST_CHECK(s2.top() == 1);
  BOOST_CHECK(s1.size() == 2);
}

BOOST_AUTO_TEST_CASE(assignment_operator)
{
  madieva::Stack<int> s1;
  s1.push(1);
  s1.push(2);
  madieva::Stack<int> s2;
  s2 = s1;
  BOOST_CHECK(s2.size() == 2);
  BOOST_CHECK(s2.top() == 2);
  BOOST_CHECK(s1.size() == 2);
  BOOST_CHECK(s1.top() == 2);
}

BOOST_AUTO_TEST_SUITE_END()
