#include <boost/test/unit_test.hpp>
#include "bidir_list.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  tarasenko::BidirList< int > list;
  BOOST_CHECK(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}
