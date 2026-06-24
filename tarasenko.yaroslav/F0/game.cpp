#include <ostream>
#include <stdexcept>

#include "game.hpp"

namespace chess
{
  Game::Game():
    currentMove_(0),
    flipped_(false)
  {
    startPosition_.setInitial();
    currentPosition_ = startPosition_;
  }

  Game::Game(const Position& start):
    startPosition_(start),
    currentPosition_(start),
    currentMove_(0),
    flipped_(false)
  {}

  const Position& Game::getPosition() const noexcept
  {
    return currentPosition_;
  }

  const Position& Game::getStartPosition() const noexcept
  {
    return startPosition_;
  }

  size_t Game::getCurrentMove() const noexcept
  {
    return currentMove_;
  }

  size_t Game::getMovesCount() const noexcept
  {
    return moves_.getSize();
  }

  const Move& Game::getMove(size_t index) const
  {
    return moves_.at(index).move_;
  }

  void Game::makeMove(const Move& move)
  {
    if (currentMove_ < moves_.getSize())
    {
      moves_.erase(currentMove_, moves_.getSize());
    }
    GameMove game_move;
    game_move.move_ = move;
    currentPosition_.makeMove(game_move.move_, game_move.undo_);
    moves_.pushBack(game_move);
    ++currentMove_;
  }

  void Game::undoMove()
  {
    if (currentMove_ == 0)
    {
      throw std::logic_error("no undo info");
    }
    --currentMove_;
    currentPosition_.undoMove(moves_[currentMove_].move_, moves_[currentMove_].undo_);
    moves_.erase(currentMove_, moves_.getSize());
  }

  void Game::prevMove()
  {
    if (currentMove_ == 0)
    {
      throw std::logic_error("no previous move");
    }
    --currentMove_;
    currentPosition_.undoMove(moves_[currentMove_].move_, moves_[currentMove_].undo_);
  }

  void Game::nextMove()
  {
    if (currentMove_ == moves_.getSize())
    {
      throw std::logic_error("no next move");
    }
    currentPosition_.makeMove(moves_[currentMove_].move_, moves_[currentMove_].undo_);
    ++currentMove_;
  }

  void Game::flip()
  {
    flipped_ = !flipped_;
  }

  void Game::print(std::ostream& out) const
  {
    currentPosition_.print(out, flipped_);
  }
}
