#include <boost/test/unit_test.hpp>

#include <limits>
#include <sstream>
#include <string>
#include <utility>

#include "input-output.hpp"
#include "list.hpp"

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  yalovsky::List< int > list;

  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.begin() == list.end());
}