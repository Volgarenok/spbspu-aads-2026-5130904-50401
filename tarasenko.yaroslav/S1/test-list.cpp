#include <boost/test/unit_test.hpp>
#include "bidir_list.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  tarasenko::BidirList< int > list;
  BOOST_CHECK(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(adding_elems_to_the_end)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(--list.end()) == 5);
  list.push_back(2);
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(--list.end()) == 2);
}

BOOST_AUTO_TEST_CASE(dereference)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(list.cbegin()) == 5);
}

BOOST_AUTO_TEST_CASE(accessing_fields)
{
  struct Int
  {
    int a;
  };
  tarasenko::BidirList< Int > list;
  list.push_back(Int{5});
  BOOST_CHECK(list.begin()->a == 5);
  BOOST_CHECK(list.cbegin()->a == 5);
}

BOOST_AUTO_TEST_CASE(increment)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.push_back(2);
  list.push_back(3);
  tarasenko::ListIter< int > it = list.begin();
  tarasenko::ListConstIter< int > cit = list.cbegin();
  BOOST_CHECK(*(it++) == 5);
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(*(++it) == 3);
  BOOST_CHECK(++it == list.end());
  BOOST_CHECK(*(cit++) == 5);
  BOOST_CHECK(*cit == 2);
  BOOST_CHECK(*(++cit) == 3);
  BOOST_CHECK(++cit == list.cend());
}

BOOST_AUTO_TEST_CASE(decrement)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.push_back(2);
  list.push_back(3);
  tarasenko::ListIter< int > it = list.end();
  tarasenko::ListConstIter< int > cit = list.cend();
  BOOST_CHECK(*(--it) == 3);
  BOOST_CHECK(*(it--) == 3);
  BOOST_CHECK(*(it--) == 2);
  BOOST_CHECK(it == list.begin());
  BOOST_CHECK(*(--cit) == 3);
  BOOST_CHECK(*(cit--) == 3);
  BOOST_CHECK(*(cit--) == 2);
  BOOST_CHECK(cit == list.cbegin());
}

BOOST_AUTO_TEST_CASE(adding_elems_to_the_begin)
{
  tarasenko::BidirList< int > list;
  list.push_front(5);
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(--list.end()) == 5);
  list.push_front(2);
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 2);
  BOOST_CHECK(*(--list.end()) == 5);
}

BOOST_AUTO_TEST_CASE(empty)
{
  tarasenko::BidirList< int > list;
  BOOST_CHECK(list.empty() == true);
  list.push_front(5);
  BOOST_CHECK(list.empty() == false);
}
