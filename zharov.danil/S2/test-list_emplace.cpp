#include <utility>
#include <boost/test/unit_test.hpp>
#include <list.hpp>

BOOST_AUTO_TEST_CASE(ListEmplaceBackSingleArgTest)
{
  zharov::List< int > list;
  list.emplaceBack(1);
  list.emplaceBack(2);
  list.emplaceBack(3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(ListEmplaceBackMultiArgTest)
{
  zharov::List< std::pair< int, int > > list;
  list.emplaceBack(1, 2);
  list.emplaceBack(3, 4);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front().first, 1);
  BOOST_CHECK_EQUAL(list.front().second, 2);
  BOOST_CHECK_EQUAL(list.back().first, 3);
  BOOST_CHECK_EQUAL(list.back().second, 4);
}

BOOST_AUTO_TEST_CASE(ListEmplaceFrontSingleArgTest)
{
  zharov::List< int > list;
  list.emplaceFront(1);
  list.emplaceFront(2);
  list.emplaceFront(3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 3);
  BOOST_CHECK_EQUAL(list.back(), 1);
}

BOOST_AUTO_TEST_CASE(ListEmplaceFrontMultiArgTest)
{
  zharov::List< std::pair< int, int > > list;
  list.emplaceFront(1, 2);
  list.emplaceFront(3, 4);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front().first, 3);
  BOOST_CHECK_EQUAL(list.front().second, 4);
  BOOST_CHECK_EQUAL(list.back().first, 1);
  BOOST_CHECK_EQUAL(list.back().second, 2);
}

BOOST_AUTO_TEST_CASE(ListEmplaceMiddleTest)
{
  zharov::List< std::pair< int, int > > list;
  list.emplaceBack(1, 2);
  list.emplaceBack(5, 6);
  zharov::LIter< std::pair< int, int > > it = list.begin();
  ++it;
  list.emplace(it, 3, 4);
  BOOST_CHECK_EQUAL(list.size(), 3);
  it = list.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 3);
  BOOST_CHECK_EQUAL(it->second, 4);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 5);
}

BOOST_AUTO_TEST_CASE(ListEmplaceAtBeginTest)
{
  zharov::List< int > list;
  list.emplaceBack(2);
  list.emplaceBack(3);
  list.emplace(list.begin(), 1);
  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_CASE(ListEmplaceAtEndTest)
{
  zharov::List< int > list;
  list.emplaceBack(1);
  list.emplaceBack(2);
  list.emplace(list.end(), 3);
  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(ListEmplaceReturnIteratorTest)
{
  zharov::List< std::pair< int, int > > list;
  list.emplaceBack(1, 2);
  zharov::LIter< std::pair< int, int > > it = list.emplaceBack(3, 4);
  BOOST_CHECK_EQUAL(it->first, 3);
  BOOST_CHECK_EQUAL(it->second, 4);
}

BOOST_AUTO_TEST_CASE(ListEmplaceEmptyListTest)
{
  zharov::List< std::pair< int, int > > list;
  list.emplaceBack(10, 20);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front().first, 10);
  BOOST_CHECK_EQUAL(list.front().second, 20);
  BOOST_CHECK_EQUAL(list.back().first, 10);
}
