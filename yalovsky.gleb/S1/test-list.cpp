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
  BOOST_CHECK(list.begin() == list.end());
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

BOOST_AUTO_TEST_CASE(insertAndErase)
{
  yalovsky::List< int > list;
  list.pushBack(1);
  list.pushBack(3);

  yalovsky::List< int >::iterator pos = list.begin();
  ++pos;
  list.insert(pos, 2);

  BOOST_TEST(*(list.begin() + 1) == 2);

  pos = list.begin() + 1;
  yalovsky::List< int >::iterator next = list.erase(pos);
  BOOST_TEST(*next == 3);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*(list.begin() + 1) == 3);
}

BOOST_AUTO_TEST_CASE(copyConstructorCreatesIndependentCopy)
{
  yalovsky::List< int > source;
  source.pushBack(1);
  source.pushBack(2);

  yalovsky::List< int > copy(source);
  copy.popFront();
  copy.pushBack(3);

  BOOST_TEST(source.size() == 2);
  BOOST_TEST(source.front() == 1);
  BOOST_TEST(source.back() == 2);
  BOOST_TEST(copy.front() == 2);
  BOOST_TEST(copy.back() == 3);
}

BOOST_AUTO_TEST_CASE(moveConstructorLeavesSourceValid)
{
  yalovsky::List< int > source;
  source.pushBack(10);
  source.pushBack(20);

  yalovsky::List< int > moved(std::move(source));

  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(moved.front() == 10);
  BOOST_TEST(moved.back() == 20);
  BOOST_TEST(source.empty());
  BOOST_CHECK(source.begin() == source.end());
}

BOOST_AUTO_TEST_CASE(clearRemovesAllElements)
{
  yalovsky::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  list.clear();

  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(readSequencesWorksWithEmptySequence)
{
  std::istringstream input("first 1 2\nsecond\nthird 3\n");
  yalovsky::SequenceList sequences;

  yalovsky::readSequences(input, sequences);

  BOOST_TEST(sequences.size() == 3);
  BOOST_TEST(sequences.front().first == "first");
  BOOST_TEST((sequences.begin() + 1)->first == "second");
  BOOST_TEST(sequences.back().first == "third");
  BOOST_TEST((sequences.begin() + 1)->second.empty());
}

BOOST_AUTO_TEST_CASE(transposeAndSumsMatchTaskLogic)
{
  yalovsky::SequenceList sequences;

  yalovsky::NumberList first;
  first.pushBack(1);
  first.pushBack(1);
  first.pushBack(1);
  sequences.pushBack(std::make_pair(std::string("first"), first));

  yalovsky::NumberList second;
  second.pushBack(2);
  second.pushBack(2);
  second.pushBack(2);
  second.pushBack(2);
  sequences.pushBack(std::make_pair(std::string("second"), second));

  yalovsky::NumberList third;
  sequences.pushBack(std::make_pair(std::string("third"), third));

  yalovsky::NumberList fourth;
  fourth.pushBack(4);
  fourth.pushBack(4);
  sequences.pushBack(std::make_pair(std::string("fourth"), fourth));

  yalovsky::Matrix matrix;
  yalovsky::transposeSequences(sequences, matrix);

  std::ostringstream matrixOut;
  yalovsky::printMatrix(matrixOut, matrix);
  BOOST_TEST(matrixOut.str() == "1 2 4\n1 2 4\n1 2\n2\n");

  yalovsky::NumberList sums;
  yalovsky::calculateSums(matrix, sums);

  std::ostringstream sumsOut;
  yalovsky::printNumberList(sumsOut, sums);
  BOOST_TEST(sumsOut.str() == "7 7 3 2\n");
}

BOOST_AUTO_TEST_CASE(calculateSumsThrowsOnOverflow)
{
  yalovsky::Matrix matrix;
  yalovsky::NumberList row;
  row.pushBack(std::numeric_limits< std::size_t >::max());
  row.pushBack(1);
  matrix.pushBack(row);

  yalovsky::NumberList sums;
  BOOST_CHECK_THROW(yalovsky::calculateSums(matrix, sums), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(onlyEmptySequenceProducesZeroSums)
{
  std::istringstream input("d\n");
  yalovsky::SequenceList sequences;

  yalovsky::readSequences(input, sequences);

  BOOST_TEST(sequences.size() == 1);
  BOOST_TEST(sequences.front().first == "d");
  BOOST_TEST(sequences.front().second.empty());

  yalovsky::Matrix matrix;
  yalovsky::transposeSequences(sequences, matrix);
  BOOST_TEST(matrix.empty());

  yalovsky::NumberList sums;
  yalovsky::calculateSums(matrix, sums);
  BOOST_TEST(sums.empty());
}
