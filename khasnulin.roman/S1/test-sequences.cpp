#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "List.hpp"
#include "Sequences.hpp"

BOOST_AUTO_TEST_SUITE(SequencesTestSuite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_readRow_basic)
{
  std::stringstream ss("first 1 1 1");
  auto result = readRow(ss);

  BOOST_CHECK_EQUAL(result.first, "first");
  BOOST_CHECK_EQUAL(result.second.size(), 3);

  auto it = result.second.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 1);
  BOOST_CHECK_EQUAL(*(++it), 1);
}

BOOST_AUTO_TEST_CASE(test_readRow_empty_list)
{
  std::stringstream ss("third");
  auto result = khasnulin::readRow(ss);

  BOOST_CHECK_EQUAL(result.first, "third");
  BOOST_CHECK(result.second.empty());
  BOOST_CHECK_EQUAL(result.second.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_readRow_different_numbers)
{
  std::stringstream ss("second 10 20 30 40");
  auto result = khasnulin::readRow(ss);

  BOOST_CHECK_EQUAL(result.first, "second");
  BOOST_CHECK_EQUAL(result.second.size(), 4);

  auto it = result.second.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  BOOST_CHECK_EQUAL(*(++it), 20);
  BOOST_CHECK_EQUAL(*(++it), 30);
  BOOST_CHECK_EQUAL(*(++it), 40);
}

BOOST_AUTO_TEST_SUITE_END();