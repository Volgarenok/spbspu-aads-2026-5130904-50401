#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(Stack_test_suite)

BOOST_AUTO_TEST_CASE(constructors_and_operators)
{
  petrov::Stack< int > st;
  BOOST_CHECK(st.empty());
  st.push(1);
  petrov::Stack< int > stCopy(st);
  BOOST_CHECK(stCopy.size() == 1);
  BOOST_CHECK(stCopy.top() == 1);
  stCopy.push(2);
  petrov::Stack< int > stMove(std::move(stCopy));
  BOOST_CHECK(stMove.size() == 2);
  BOOST_CHECK(stMove.top() == 2);
  stMove.push(3);
  stMove.push(4);
  petrov::Stack< int > stCopyOp = stMove;
  BOOST_CHECK(stCopyOp.size() == 4);
  BOOST_CHECK(stCopyOp.top() == 4);

  petrov::Stack< int > stMoveOp = std::move(stCopyOp);
  BOOST_CHECK(stMoveOp.size() == 4);
  BOOST_CHECK(stMoveOp.top() == 4);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  petrov::Stack< int > st;
  BOOST_CHECK(st.empty());
  BOOST_CHECK(st.size() == 0);

  st.push(1);

  BOOST_CHECK(!st.empty());
  BOOST_CHECK(st.size() == 1);
}

BOOST_AUTO_TEST_CASE(push_pop_top)
{
  petrov::Stack< int > st;
  st.push(1);
  BOOST_CHECK(st.top() == 1);

  st.push(2);
  BOOST_CHECK(st.top() == 2);

  st.push(3);
  BOOST_CHECK(st.top() == 3);

  st.push(4);
  BOOST_CHECK(st.top() == 4);

  BOOST_CHECK(st.top() == 4);
  st.pop();
  BOOST_CHECK(st.top() == 3);
  st.pop();
  BOOST_CHECK(st.top() == 2);
  st.pop();
  BOOST_CHECK(st.top() == 1);
  st.pop();

  BOOST_CHECK(st.empty());
}

BOOST_AUTO_TEST_SUITE_END()
