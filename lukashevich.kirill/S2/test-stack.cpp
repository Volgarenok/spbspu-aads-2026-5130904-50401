#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using namespace lukashevich;

BOOST_AUTO_TEST_SUITE(StackTestSuite)

BOOST_AUTO_TEST_CASE(CreationOfStack)
{
  Stack< int > s;

  BOOST_CHECK(s.empty());
  BOOST_CHECK(s.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushFirst)
{
  Stack< int > s;
  s.push(1);
  s.push(2);

  BOOST_CHECK(s.size() == 2);
  BOOST_CHECK(s.first() == 2);
}

BOOST_AUTO_TEST_CASE(DropClear)
{
  Stack< int > s;
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
  Stack< int > s;
  s.push(32);
  s.push(42);
  s.push(52);
  Stack< int > sCopy(s);

  BOOST_CHECK(sCopy.size() == 3);
  BOOST_CHECK(!sCopy.empty());
  BOOST_CHECK(sCopy.first() == 52);

  Stack< int > sMove(std::move(sCopy));

  BOOST_CHECK(sCopy.size() == 0);
  BOOST_CHECK(sCopy.empty());
  BOOST_CHECK(sMove.first() == 52);
  BOOST_CHECK(sMove.size() == 3);

  Stack< int > sCopyOperator = sMove;

  BOOST_CHECK(sCopyOperator.size() == 3);
  BOOST_CHECK(sCopyOperator.first() == 52);
  BOOST_CHECK(sMove.size() == 3);
  
  Stack< int > sMoveOperator = std::move(sCopyOperator);

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
