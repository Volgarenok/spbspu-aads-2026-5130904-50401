#include <boost/test/unit_test.hpp>
#include "engine.hpp"

using namespace chess;

BOOST_AUTO_TEST_CASE(mate_in_1)
{
  Position pos({
    {H6, WHITE_KING}, {H8, BLACK_KING}, {F1, WHITE_ROOK}
  });
  BOOST_TEST(Engine{}.findBestMove(pos, 2).first == Move({F1, F8}));
}

BOOST_AUTO_TEST_CASE(mate_in_1_from_depth_1)
{
  Position pos({
    {H6, WHITE_KING}, {H8, BLACK_KING}, {F1, WHITE_ROOK}
  });
  BOOST_TEST(Engine{}.findBestMove(pos, 1).first == Move({F1, F8}));
}

BOOST_AUTO_TEST_CASE(quickest_mate)
{
  Position pos({
    {G6, WHITE_KING}, {H8, BLACK_KING}, {F1, WHITE_ROOK}
  });
  BOOST_TEST(Engine{}.findBestMove(pos, 4).first == Move({F1, F8}));
}

BOOST_AUTO_TEST_CASE(search_by_time)
{
  Position pos;
  pos.setInitial();
  BOOST_TEST(Engine{}.findBestMove(pos, 100, nullptr, 1).first != null_move);
}
