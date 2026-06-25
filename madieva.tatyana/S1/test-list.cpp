#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(push_front_empty)
{
  madieva::List< int > list;
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(push_front)
{
  madieva::List< int > list;
  list.push_front(10);
  list.push_front(20);
  list.push_front(30);
  list.push_front(40);
  BOOST_CHECK(list.size() == 4);

  auto it = list.begin();
  BOOST_CHECK(*it == 40);
  ++it;
  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 10);
}

BOOST_AUTO_TEST_CASE(push_back)
{
  madieva::List< int > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.push_back(40);
  BOOST_CHECK(list.size() == 4);

  auto it = list.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(*it == 40);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  madieva::List<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  list.pop_front();
  BOOST_CHECK(list.size() == 2);
  auto it = list.begin();
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  madieva::List<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  list.pop_back();
  BOOST_CHECK(list.size() == 2);
  auto it = list.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
}

BOOST_AUTO_TEST_CASE(pop_back_one)
{
  madieva::List<int> list;
  list.push_back(10);
  list.pop_back();
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(pop_front_one)
{
  madieva::List<int> list;
  list.push_front(10);
  list.pop_front();
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear)
{
  madieva::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.clear();
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
