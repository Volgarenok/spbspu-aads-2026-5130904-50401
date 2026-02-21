#include <sstream>

#include <boost/test/unit_test.hpp>

#include "credentials.hpp"

BOOST_AUTO_TEST_SUITE(credentials_suite)

BOOST_AUTO_TEST_CASE(credential_test)
{
  std::ostringstream out;
  khasnulin::out_credentials(out);
  BOOST_TEST(out.str() == "khasnulin.roman");
}

BOOST_AUTO_TEST_SUITE_END()
