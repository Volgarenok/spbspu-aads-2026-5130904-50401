#define BOOST_TEST_MODULE S0
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(printNameTest)
{
  std::stringstream a;
  sedov::printName(a);
  BOOST_TEST(a.str() == "sedov.gleb");
}
