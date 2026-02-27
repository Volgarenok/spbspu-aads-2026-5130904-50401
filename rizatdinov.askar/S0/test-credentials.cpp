#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(cedentials_test)
{
  std::ostringstream out;
  rizatdinov::out_credentials(out);
  BOOST_TEST(out.str() == "rizatdinov.askar");
}
