#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <stdexcept>
#include <string>

#include "LIter.hpp"
#include "list.hpp"

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

BOOST_AUTO_TEST_CASE(test_begin_not_equal_end_non_empty)
{
  BiList< int > list;
  list.push_back(100);

  LIter< int > beg = list.begin();
  LIter< int > end = list.end();

  BOOST_CHECK(beg != end);
}

BOOST_AUTO_TEST_CASE(test_reaching_end_iterator)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  LIter< int > it = list.begin();
  ++it;
  ++it;

  BOOST_CHECK(it == list.end());
  BOOST_CHECK_THROW(*it, std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_back_from_end)
{
  BiList< int > list;
  list.push_back(10);
  auto it = list.end();
  --it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_SUITE_END()