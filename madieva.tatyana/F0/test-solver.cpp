#include <boost/test/unit_test.hpp>
#include "solver.hpp"

BOOST_AUTO_TEST_SUITE(SolverTest)

BOOST_AUTO_TEST_CASE(empty_line_no_hints)
{
  madieva::Vector< int > line;
  madieva::Vector< size_t > hints;
  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(result.getSize() == 0);
  BOOST_CHECK(!changed);
}

BOOST_AUTO_TEST_CASE(line_hint_5)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  madieva::Vector< size_t > hints;
  hints.pushBack(5);

  madieva::Vector< int > result;
  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(changed);
  BOOST_CHECK(result.getSize() == 5);

  for (size_t i = 0; i < result.getSize(); ++i) {
    BOOST_CHECK(result[i] == 1);
  }
}

BOOST_AUTO_TEST_CASE(line_hint_4)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  madieva::Vector< size_t > hints;
  hints.pushBack(4);

  madieva::Vector< int > result;
  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(changed);
  BOOST_CHECK(result.getSize() == 5);
  BOOST_CHECK(result[1] == 1);
  BOOST_CHECK(result[2] == 1);
  BOOST_CHECK(result[3] == 1);
  BOOST_CHECK(result[0] == 0);
  BOOST_CHECK(result[4] == 0);
}

BOOST_AUTO_TEST_CASE(line_hint_2_1)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  madieva::Vector< size_t > hints;
  hints.pushBack(2);
  hints.pushBack(1);

  madieva::Vector< int > result;
  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(changed);
  BOOST_CHECK(result.getSize() == 5);

  BOOST_CHECK(result[0] == 0);
  BOOST_CHECK(result[1] == 1);
  BOOST_CHECK(result[2] == 0);
  BOOST_CHECK(result[3] == 0);
  BOOST_CHECK(result[4] == 0);
}

BOOST_AUTO_TEST_CASE(line_with_known_cell)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  line[2] = 1;

  madieva::Vector< size_t > hints;
  hints.pushBack(2);
  hints.pushBack(1);

  madieva::Vector< int > result;
  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(changed);
  BOOST_CHECK(result[0] == -1);
  BOOST_CHECK(result[1] == 1);
  BOOST_CHECK(result[2] == 1);
  BOOST_CHECK(result[3] == -1);
  BOOST_CHECK(result[4] == 1);
}

BOOST_AUTO_TEST_CASE(contradiction)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  line[0] = 1;
  line[2] = -1;

  madieva::Vector< size_t > hints;
  hints.pushBack(2);
  hints.pushBack(1);

  madieva::Vector< int > result;
  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(changed);
  BOOST_CHECK(result[0] == 1);
  BOOST_CHECK(result[1] == 1);
  BOOST_CHECK(result[2] == -1);
  BOOST_CHECK(result[3] == 0);
  BOOST_CHECK(result[4] == 0);
}

BOOST_AUTO_TEST_CASE(no_hints)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  madieva::Vector< size_t > hints;
  madieva::Vector< int > result;
  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(changed);
  BOOST_CHECK(result.getSize() == 5);

  for (size_t i = 0; i < result.getSize(); ++i) {
    BOOST_CHECK(result[i] == -1);
  }
}

BOOST_AUTO_TEST_CASE(center_block_forced)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  madieva::Vector< size_t > hints;
  hints.pushBack(4);

  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(
    line,
    hints,
    result);

  BOOST_CHECK(changed);

  BOOST_CHECK(result[0] == 0);
  BOOST_CHECK(result[1] == 1);
  BOOST_CHECK(result[2] == 1);
  BOOST_CHECK(result[3] == 1);
  BOOST_CHECK(result[4] == 0);
}

BOOST_AUTO_TEST_CASE(impossible_line)
{
  madieva::Vector< int > line;

  line.pushBack(-1);
  line.pushBack(-1);
  line.pushBack(0);
  line.pushBack(0);
  line.pushBack(0);

  madieva::Vector< size_t > hints;
  hints.pushBack(3);

  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(
    line,
    hints,
    result);

  BOOST_CHECK(changed);

  BOOST_CHECK(result[0] == -1);
  BOOST_CHECK(result[1] == -1);
  BOOST_CHECK(result[2] == 1);
  BOOST_CHECK(result[3] == 1);
  BOOST_CHECK(result[4] == 1);
}

BOOST_AUTO_TEST_CASE(inserting_empty_cells)
{
  madieva::Vector< int > line;

  line.pushBack(1);
  line.pushBack(1);
  line.pushBack(1);
  line.pushBack(0);
  line.pushBack(0);

  madieva::Vector< size_t > hints;
  hints.pushBack(3);

  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(
    line,
    hints,
    result);

  BOOST_CHECK(changed);

  BOOST_CHECK(result[0] == 1);
  BOOST_CHECK(result[1] == 1);
  BOOST_CHECK(result[2] == 1);
  BOOST_CHECK(result[3] == -1);
  BOOST_CHECK(result[4] == -1);
}

BOOST_AUTO_TEST_CASE(empty_line)
{
  madieva::Vector< int > line;

  line.pushBack(0);
  line.pushBack(0);
  line.pushBack(0);
  line.pushBack(0);
  line.pushBack(0);

  madieva::Vector< size_t > hints;

  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(
    line,
    hints,
    result);

  BOOST_CHECK(changed);

  BOOST_CHECK(result[0] == -1);
  BOOST_CHECK(result[1] == -1);
  BOOST_CHECK(result[2] == -1);
  BOOST_CHECK(result[3] == -1);
  BOOST_CHECK(result[4] == -1);
}

BOOST_AUTO_TEST_CASE(empty_line_with_error)
{
  madieva::Vector< int > line;

  line.pushBack(0);
  line.pushBack(0);
  line.pushBack(1);
  line.pushBack(0);
  line.pushBack(0);

  madieva::Vector< size_t > hints;

  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(
    line,
    hints,
    result);

  BOOST_CHECK(!changed);
}

BOOST_AUTO_TEST_CASE(line_with_known_empty_cells)
{
  madieva::Vector< int > line;

  line.pushBack(0);
  line.pushBack(-1);
  line.pushBack(0);
  line.pushBack(-1);
  line.pushBack(0);
  line.pushBack(0);

  madieva::Vector< size_t > hints;
  hints.pushBack(1);
  hints.pushBack(1);
  hints.pushBack(1);

  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(
    line,
    hints,
    result);

  BOOST_CHECK(changed);

  BOOST_CHECK(result[0] == 1);
  BOOST_CHECK(result[1] == -1);
  BOOST_CHECK(result[2] == 1);
  BOOST_CHECK(result[3] == -1);
  BOOST_CHECK(result[4] == 0);
  BOOST_CHECK(result[5] == 0);
}

BOOST_AUTO_TEST_CASE(single_group_middle)
{
  madieva::Vector< int > line;

  for (size_t i = 0; i < 5; ++i) {
    line.pushBack(0);
  }

  madieva::Vector< size_t > hints;
  hints.pushBack(2);

  madieva::Vector< int > result;

  bool changed = madieva::analyzeLine(line, hints, result);

  BOOST_CHECK(!changed);

  BOOST_CHECK(result[0] == 0);
  BOOST_CHECK(result[1] == 0);
  BOOST_CHECK(result[2] == 0);
  BOOST_CHECK(result[3] == 0);
  BOOST_CHECK(result[4] == 0);
}

BOOST_AUTO_TEST_CASE(empty_puzzle)
{
  madieva::Vector< madieva::Vector< size_t > > rowHints;
  madieva::Vector< madieva::Vector< size_t > > colHints;

  madieva::Vector< madieva::Vector< int > > result =
    madieva::solvePuzzle(rowHints, colHints, 0, 0);

  BOOST_CHECK(result.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(single_cell_filled)
{
  madieva::Vector< madieva::Vector< size_t > > rowHints;
  madieva::Vector< madieva::Vector< size_t > > colHints;

  rowHints.pushBack(madieva::Vector< size_t >());
  rowHints[0].pushBack(1);

  colHints.pushBack(madieva::Vector< size_t >());
  colHints[0].pushBack(1);

  madieva::Vector< madieva::Vector< int > > result =
    madieva::solvePuzzle(rowHints, colHints, 1, 1);

  BOOST_CHECK(result.getSize() == 1);
  BOOST_CHECK(result[0].getSize() == 1);
  BOOST_CHECK(result[0][0] == 1);
}

BOOST_AUTO_TEST_CASE(single_cell_empty)
{
  madieva::Vector< madieva::Vector< size_t > > rowHints;
  madieva::Vector< madieva::Vector< size_t > > colHints;

  rowHints.pushBack(madieva::Vector< size_t >());
  colHints.pushBack(madieva::Vector< size_t >());

  madieva::Vector< madieva::Vector< int > > result =
    madieva::solvePuzzle(rowHints, colHints, 1, 1);

  BOOST_CHECK(result.getSize() == 1);
  BOOST_CHECK(result[0].getSize() == 1);
  BOOST_CHECK(result[0][0] == -1);
}


BOOST_AUTO_TEST_CASE(simple_2x2_filled)
{
  madieva::Vector< madieva::Vector< size_t > > rowHints;
  madieva::Vector< madieva::Vector< size_t > > colHints;

  for (size_t i = 0; i < 2; ++i) {
    rowHints.pushBack(madieva::Vector< size_t >());
    rowHints[i].pushBack(2);

    colHints.pushBack(madieva::Vector< size_t >());
    colHints[i].pushBack(2);
  }

  madieva::Vector< madieva::Vector< int > > result =
    madieva::solvePuzzle(rowHints, colHints, 2, 2);

  BOOST_CHECK(result.getSize() == 2);
  for (size_t i = 0; i < 2; ++i) {
    BOOST_CHECK(result[i].getSize() == 2);
    for (size_t j = 0; j < 2; ++j) {
      BOOST_CHECK(result[i][j] == 1);
    }
  }
}

BOOST_AUTO_TEST_CASE(simple_3x3_cross)
{
  madieva::Vector< madieva::Vector< size_t > > rowHints;
  madieva::Vector< madieva::Vector< size_t > > colHints;

  const int expected[3][3] = {
    { -1,  1, -1 },
    {  1,  1,  1 },
    { -1,  1, -1 }
  };

  const size_t rowHintValues[3] = { 1, 3, 1 };
  const size_t colHintValues[3] = { 1, 3, 1 };

  for (size_t i = 0; i < 3; ++i) {
    rowHints.pushBack(madieva::Vector< size_t >());
    rowHints[i].pushBack(rowHintValues[i]);

    colHints.pushBack(madieva::Vector< size_t >());
    colHints[i].pushBack(colHintValues[i]);
  }

  madieva::Vector< madieva::Vector< int > > result =
    madieva::solvePuzzle(rowHints, colHints, 3, 3);

  BOOST_CHECK(result.getSize() == 3);
  for (size_t i = 0; i < 3; ++i) {
    BOOST_CHECK(result[i].getSize() == 3);
    for (size_t j = 0; j < 3; ++j) {
      BOOST_CHECK(result[i][j] == expected[i][j]);
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
