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

BOOST_AUTO_TEST_CASE(pushFrontAndPushBack)
{
  yalovsky::List< int > list;
  list.pushBack(2);
  list.pushFront(1);
  list.pushBack(3);

  BOOST_TEST(list.size() == 3);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 3);
  BOOST_TEST(*(list.begin() + 1) == 2);
}