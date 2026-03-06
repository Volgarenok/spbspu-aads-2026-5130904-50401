#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <stdexcept>
#include <string>

#include "list.hpp"

BOOST_AUTO_TEST_SUITE(list_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_create_empty_list)
{
  BiList< int > list;

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_element)
{
  BiList< int > list;
  list.push_back(1);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
}

BOOST_AUTO_TEST_CASE(test_front_and_begin_iter_value_equal)
{
  BiList< int > list;
  list.push_back(1);

  LIter< int > it = list.begin();

  BOOST_CHECK_EQUAL(list.front(), *it);
}

BOOST_AUTO_TEST_CASE(test_iters_moving)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);

  LIter< int > it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  --it;
  BOOST_CHECK_EQUAL(*it, 2);
  --it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_list_copy_constructor)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  BiList< int > listCP(list);

  BOOST_CHECK_EQUAL(listCP.front(), 1);
  BOOST_CHECK_EQUAL(listCP.back(), 2);
}

BOOST_AUTO_TEST_CASE(test_list_move_constructor)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  BiList< int > listCP(std::move(list));

  BOOST_CHECK_EQUAL(listCP.front(), 1);
  BOOST_CHECK_EQUAL(listCP.back(), 2);

  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_list_self_assignment)
{
  BiList< int > list;
  list.push_back(1);

  list = list;

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(iterators_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_iterators_conversion)
{
  BiList< int > list;
  list.push_back(1);

  auto iter = list.begin();
  auto cIter = list.cbegin();

  BOOST_CHECK(cIter == iter);

  LCIter< int > cIter2 = iter;
  BOOST_CHECK(cIter2 == cIter);
}

struct People
{
  std::string name;
  int age;
};

BOOST_AUTO_TEST_CASE(test_iterators_arrow_operator)
{
  BiList< People > list;
  list.push_back({"Vasiliy", 28});

  auto iter = list.begin();

  BOOST_CHECK_EQUAL(iter->name, "Vasiliy");
  BOOST_CHECK_EQUAL(iter->age, 28);
}

BOOST_AUTO_TEST_CASE(test_empty_iterator_dereferencing_error)
{
  BiList< int > list;

  auto iter = list.begin();

  BOOST_CHECK_EQUAL(iter, list.end());

  BOOST_CHECK_THROW(*iter, std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_arrow_operator_iterator_on_the_end)
{
  BiList< std::string > list;
  list.push_back("Ivan");

  auto iter = list.begin();
  BOOST_CHECK_EQUAL(*iter, "Ivan");
  ++iter;
  BOOST_CHECK_THROW(iter->clear(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_multiple_iterators_and_not_equal_operation)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  auto it1 = list.begin();
  auto it2 = list.begin();

  BOOST_CHECK(it1 == it2);
  it2++;
  BOOST_CHECK(it1 != it2);
}

BOOST_AUTO_TEST_SUITE_END()