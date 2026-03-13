#include "io.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <sstream>

using namespace levkin;
BOOST_AUTO_TEST_CASE(read_data_test)
{
  std::stringstream input("first 1 1 1\nsecond 2 2 2 2\nthird\nfourth 4 4");
  Data data;

  readData(input, data);
  auto it = data.begin();

  BOOST_CHECK_EQUAL(it->first, "first");
  BOOST_CHECK_EQUAL(*(it->second.begin()), 1);

  ++it;
  ++it;
  BOOST_CHECK_EQUAL(it->first, "third");
  BOOST_CHECK(it->second.begin() == it->second.end());
}
