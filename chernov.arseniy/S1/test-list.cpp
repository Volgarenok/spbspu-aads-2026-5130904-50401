#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(list_suite)

BOOST_AUTO_TEST_CASE(test_empty_list)
{
  chernov::List< int > list;
  BOOST_TEST(list.empty());
  BOOST_CHECK(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_SUITE_END()
