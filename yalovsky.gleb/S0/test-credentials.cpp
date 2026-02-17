#define BOOST_TEST_MODULE S0
#include <boost/test/included/unit_test.hpp>
#include "credentials.hpp"
#include <sstream>

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  yalovsky::out_credentials(out);
  BOOST_TEST(out.str() == "yalovsky.gleb");
}
