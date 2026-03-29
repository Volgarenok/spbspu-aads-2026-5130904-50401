#include <boost/test/unit_test.hpp>
#include <list.hpp>

BOOST_AUTO_TEST_SUITE(list_suite)

BOOST_AUTO_TEST_CASE(test_create_list)
{
  hvostov::List< int > list;
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_insert_element)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  BOOST_CHECK(*it == 6);
  it = list.insertAfter(it, 7);
  BOOST_CHECK(*it == 7);
  it = list.begin();
  list.insertAfter(it, 10);
  BOOST_CHECK(*it == 6);
  it++;
  BOOST_CHECK(*it == 10);
  it++;
  BOOST_CHECK(*it == 7);
  it = list.end();
  list.insertAfter(it, 8);
  it++;
  BOOST_CHECK(*it == 8);
  it++;
  BOOST_CHECK(*it == 6);
}

BOOST_AUTO_TEST_CASE(test_erase_element)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  it = list.begin();
  list.eraseAfter(it);
  it++;
  BOOST_CHECK(!list.empty());
  BOOST_CHECK(it == list.end());
  it = list.begin();
  BOOST_CHECK((*it) == 6);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  BOOST_CHECK(!list.empty());
  list.clear();
  BOOST_CHECK(list.empty());
  it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  BOOST_CHECK(!list.empty());
  list.clear();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_SUITE_END()
