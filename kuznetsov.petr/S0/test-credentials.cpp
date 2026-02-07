#define BOOST_TEST_MODULE S0
#include <sstream>
#include <boost/test/included/unit_test.hpp>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(GetName_test)
{
  std::ostringstream out;
  kuznetsov::getName(out);
  BOOST_TEST(out.str() == "kuznetsov.petr");
}

