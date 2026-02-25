#include <sstream>
#include <boost/test/unit_test.hpp>
#include "credentials.hpp"


BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  levkin::out_credentials(out);
  BOOST_TEST(out.str() == "levkin.dima");
}
