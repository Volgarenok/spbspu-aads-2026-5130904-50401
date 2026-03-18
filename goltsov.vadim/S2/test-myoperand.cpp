#include <boost/test/unit_test.hpp>
#include "mathoperations.hpp"

BOOST_AUTO_TEST_CASE(concatenation_test)
{
  BOOST_CHECK(goltsov::concatenation(2, 3) == 23);
  BOOST_CHECK_THROW(goltsov::concatenation(9223, 372036854775808), std::overflow_error);
}