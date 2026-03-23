#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTestSuite)

BOOST_AUTO_TEST_CASE(CreationOfStack)
{
  kondrat::Stack< int > s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushFirst)
{
  kondrat::Stack< int > s;
  s.push(1);
  s.push(2);

  BOOST_CHECK(s.size() == 2);
  BOOST_CHECK(s.first() == 2);
}

BOOST_AUTO_TEST_CASE(DropClear)
{
  kondrat::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);

  BOOST_CHECK(s.size() == 3);

  int res = s.drop();

  BOOST_CHECK(s.size() == 2);
  BOOST_CHECK(res == 3);

  s.clear();

  BOOST_CHECK(s.size() == 0);
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(OperatorsConstructors)
{
  kondrat::Stack< int > s;
  s.push(32);
  s.push(42);
  s.push(52);

  kondrat::Stack< int > sCopy(s);
  BOOST_CHECK(sCopy.size() == 3);
  BOOST_CHECK(!sCopy.empty());
  BOOST_CHECK(sCopy.first() == 52);

  kondrat::Stack< int > sMove(std::move(sCopy));
  BOOST_CHECK(sCopy.size() == 0);
  BOOST_CHECK(sCopy.empty());
  BOOST_CHECK(sMove.first() == 52);
  BOOST_CHECK(sMove.size() == 3);

  kondrat::Stack< int > sCopyOperator = sMove;
  BOOST_CHECK(sCopyOperator.size() == 3);
  BOOST_CHECK(sCopyOperator.first() == 52);
  BOOST_CHECK(sMove.size() == 3);

  kondrat::Stack< int > sMoveOperator = std::move(sCopyOperator);
  BOOST_CHECK(sMoveOperator.size() == 3);
  BOOST_CHECK(sCopyOperator.empty());

  s.clear();
  sMove.clear();
  sMoveOperator.clear();
  BOOST_CHECK(sMoveOperator.empty());
  BOOST_CHECK(sMove.empty());
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_SUITE_END()
