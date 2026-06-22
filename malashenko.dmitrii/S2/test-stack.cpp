#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(Stack_test_suite)
BOOST_AUTO_TEST_CASE(constructors_and_operators)
{
  malashenko::Stack< int > st;
  BOOST_CHECK(st.empty());
  st.push(1);
  malashenko::Stack< int > stCopy(st);
  BOOST_CHECK(stCopy.size() == 1);
  BOOST_CHECK(stCopy.top() == 1);
  stCopy.push(2);
  malashenko::Stack< int > stMove(std::move(stCopy));
  BOOST_CHECK(stMove.size() == 2);
  BOOST_CHECK(stMove.top() == 2);
  stMove.push(3);
  stMove.push(4);
  malashenko::Stack< int > stCopyOp = stMove;
  BOOST_CHECK(stCopyOp.size() == 4);
  BOOST_CHECK(stCopyOp.top() == 4);

  malashenko::Stack< int > stMoveOp = std::move(stCopyOp);
  BOOST_CHECK(stMoveOp.size() == 4);
  BOOST_CHECK(stMoveOp.top() == 4);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  malashenko::Stack< int > st;
  BOOST_CHECK(st.empty());
  BOOST_CHECK(st.size() == 0);

  st.push(1);

  BOOST_CHECK(!st.empty());
  BOOST_CHECK(st.size() == 1);
}

BOOST_AUTO_TEST_CASE(push_pop_top)
{
  malashenko::Stack< int > st;
  st.push(1);
  BOOST_CHECK(st.top() == 1);

  st.push(2);
  BOOST_CHECK(st.top() == 2);

  st.push(3);
  BOOST_CHECK(st.top() == 3);

  st.push(4);
  BOOST_CHECK(st.top() == 4);


  st.pop();
  BOOST_CHECK(st.top() == 3);

  st.pop();
  BOOST_CHECK(st.top() == 2);

  st.pop();
  BOOST_CHECK(st.top() == 1);
}

BOOST_AUTO_TEST_CASE(swap)
{
  malashenko::Stack< int > st1;
  st1.push(1);

  malashenko::Stack< int > st2;
  st2.push(9);

  st1.swap(st2);

  BOOST_CHECK(st1.top() == 9);
  BOOST_CHECK(st2.top() == 1);
}

BOOST_AUTO_TEST_CASE(clear)
{
  malashenko::Stack< int > st;
  st.push(1);
  st.push(2);
  st.push(3);
  st.push(4);
  st.push(5);

  BOOST_CHECK(st.size() == 5);

  st.clear();

  BOOST_CHECK(st.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()
