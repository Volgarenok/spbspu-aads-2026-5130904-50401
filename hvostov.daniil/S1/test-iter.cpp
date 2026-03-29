#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(iter_suite)

BOOST_AUTO_TEST_CASE(test_begin)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  it = list.begin();
  BOOST_CHECK(*it == 6);
}

BOOST_AUTO_TEST_CASE(test_end)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  it = list.end();
  BOOST_CHECK(*it != 7);
  it++;
  BOOST_CHECK(*it == 6);
}

BOOST_AUTO_TEST_SUITE_END()
