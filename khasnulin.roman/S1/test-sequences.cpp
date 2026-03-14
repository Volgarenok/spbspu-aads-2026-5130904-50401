#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <utility>

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

BOOST_AUTO_TEST_SUITE(ReadAllTestInputSuite)

BOOST_AUTO_TEST_CASE(test_readAll_multiple_sequences)
{

  std::stringstream ss("first 1 1 1\n"
                       "second 2 2 2 2\n"
                       "third\n"
                       "fourth 4 4\n");

  khasnulin::BiList< std::pair< std::string, khasnulin::BiList< int > > > result = khasnulin::readAll(ss);

  BOOST_CHECK_EQUAL(result.size(), 4);

  auto it = result.begin();
  BOOST_CHECK_EQUAL(it->first, "first");
  BOOST_CHECK_EQUAL(it->second.size(), 3);

  ++it;
  BOOST_CHECK_EQUAL(it->first, "second");
  BOOST_CHECK_EQUAL(it->second.size(), 4);

  ++it;
  BOOST_CHECK_EQUAL(it->first, "third");
  BOOST_CHECK(it->second.empty());

  ++it;
  BOOST_CHECK_EQUAL(it->first, "fourth");
  BOOST_CHECK_EQUAL(it->second.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_readAll_empty_input)
{
  std::stringstream ss("\n");
  auto result = khasnulin::readAll(ss);
  BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(OutputDataByInputTests)

using namespace khasnulin;
using Row = std::pair< std::string, BiList< int > >;

BOOST_AUTO_TEST_CASE(test_printSequenceNames)
{
  BiList< Row > data;

  data.push_back({"first", {}});
  data.push_back({"second", {}});

  std::stringstream ss;
  printSequenceNames(ss, data);

  std::string output = ss.str();
  BOOST_CHECK_EQUAL(output, "first second\n");
}

BOOST_AUTO_TEST_CASE(test_printSequencesNumsByPlace)
{
  std::stringstream ss("first 1 1 1\n"
                       "second 2 2 2 2\n"
                       "third\n"
                       "fourth 4 4\n");

  BiList< Row > result = khasnulin::readAll(ss);

  BiList< BiList< int > > new_lists = getTransosedNumsSequences(result);

  std::stringstream res_ss;

  printSequencesNumsByPlace(res_ss, new_lists);

  std::string expected = "1 2 4\n"
                         "1 2 4\n"
                         "1 2\n"
                         "2\n";

  BOOST_CHECK_EQUAL(res_ss.str(), expected);
}

BOOST_AUTO_TEST_CASE(test_printSums_standard)
{
  std::stringstream ss("first 1 1 1\n"
                       "second 2 2 2 2\n"
                       "third\n"
                       "fourth 4 4\n");

  BiList< Row > result = khasnulin::readAll(ss);

  BiList< BiList< int > > new_lists = getTransosedNumsSequences(result);

  std::stringstream res_ss;

  printSumsOfSequences(res_ss, new_lists);

  std::string expected = "7 7 3 2\n";

  BOOST_CHECK_EQUAL(res_ss.str(), expected);
}

BOOST_AUTO_TEST_CASE(test_printSums_with_zeros)
{
  std::stringstream ss("first 0 5\n"
                       "second -1 1\n");

  BiList< Row > result = khasnulin::readAll(ss);

  BiList< BiList< int > > new_lists = getTransosedNumsSequences(result);

  std::stringstream res_ss;

  printSumsOfSequences(res_ss, new_lists);

  std::string expected = "-1 6\n";

  BOOST_CHECK_EQUAL(res_ss.str(), expected);
}

BOOST_AUTO_TEST_CASE(test_readAll_only_names)
{
  std::stringstream ss("first\n"
                       "second\n"
                       "third\n");

  auto result = khasnulin::readAll(ss);

  BOOST_CHECK_EQUAL(result.size(), 3);

  auto it = result.begin();
  BOOST_CHECK_EQUAL(it->first, "first");
  BOOST_CHECK(it->second.empty());

  ++it;
  BOOST_CHECK_EQUAL(it->first, "second");
  BOOST_CHECK(it->second.empty());
}

BOOST_AUTO_TEST_CASE(test_transpose_no_numbers)
{
  using Row = std::pair< std::string, BiList< int > >;
  BiList< Row > data;
  data.push_back({"first", {}});
  data.push_back({"second", {}});

  BiList< BiList< int > > result = khasnulin::getTransosedNumsSequences(data);

  BOOST_CHECK(result.empty());
  BOOST_CHECK_EQUAL(result.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_output_names_but_no_numbers)
{
  std::stringstream input("first\nsecond\n");
  std::stringstream output;

  auto data = khasnulin::readAll(input);

  if (data.empty())
  {
    output << "0\n";
  }
  else
  {
    khasnulin::printSequenceNames(output, data);
    auto transposed = khasnulin::getTransosedNumsSequences(data);
    if (transposed.empty())
    {
      output << "0\n";
    }
    else
    {
      khasnulin::printSequencesNumsByPlace(output, transposed);
      khasnulin::printSumsOfSequences(output, transposed);
    }
  }

  std::string expected = "first second\n0\n";
  BOOST_CHECK_EQUAL(output.str(), expected);
}

BOOST_AUTO_TEST_CASE(test_printSums_overflow)
{
  using namespace khasnulin;

  BiList< BiList< int > > data;
  BiList< int > big_numbers;

  int max_val = std::numeric_limits< int >::max();
  big_numbers.push_back(max_val);
  big_numbers.push_back(1);

  data.push_back(std::move(big_numbers));

  std::stringstream ss;

  BOOST_CHECK_THROW(printSumsOfSequences(ss, data), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(test_safeSum_equal_large_numbers)
{
  int large = 1500000000;
  BOOST_CHECK_THROW(safeSum(large, large), std::overflow_error);
}

BOOST_AUTO_TEST_SUITE_END()
