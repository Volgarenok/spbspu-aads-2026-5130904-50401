#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "List.hpp"

BOOST_AUTO_TEST_SUITE(reverseIteratorSuite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_empty_list_reverse)
{
  BiList< int > list;
  BOOST_CHECK(list.rbegin() == list.rend());
}

BOOST_AUTO_TEST_CASE(test_single_element_reverse)
{
  khasnulin::BiList< int > list;
  list.push_back(42);

  auto it = list.rbegin();
  BOOST_CHECK(*it == 42);

  ++it;
  BOOST_CHECK(it == list.rend());
}

BOOST_AUTO_TEST_CASE(test_order_reverse)
{
  khasnulin::BiList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto it = list.rbegin();
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(it == list.rend());
}

BOOST_AUTO_TEST_CASE(test_decrement_reverse)
{
  khasnulin::BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  auto it = list.rend();
  --it;
  BOOST_CHECK(*it == 1);
  --it;
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(it == list.rbegin());
}

BOOST_AUTO_TEST_SUITE_END()
