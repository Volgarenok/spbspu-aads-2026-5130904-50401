#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_SUITE(credentials_suite)

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  zubarev::out_credentials(out);
  BOOST_TEST(out.str() == "zubarev.arsenii");
}

BOOST_AUTO_TEST_CASE(yacredentials_test)
{
  std::ostringstream out;
  zubarev::out_credentials(out);
  BOOST_TEST(out.str() == "zubarev.arsenii");
}

BOOST_AUTO_TEST_SUITE_END()
