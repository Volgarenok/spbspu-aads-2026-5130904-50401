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

BOOST_AUTO_TEST_SUITE_END()
