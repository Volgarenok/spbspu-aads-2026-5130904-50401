#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"
BOOST_AUTO_TEST_SUITE(credentials_suit)

BOOST_AUTO_TEST_CASE(credential_test)
{
  std::ostringstream out;
  madieva::out_credential(out);
  BOOST_TEST(out.str() == "madieva.tatyana");
}
BOOST_AUTO_TEST_CASE(yecredential_test)
{
  std::ostringstream out;
  madieva::out_credential(out);
  BOOST_TEST(out.str() == "madieva.tatyana");
}
BOOST_AUTO_TEST_SUITE_END()
