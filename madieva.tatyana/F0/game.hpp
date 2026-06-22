#ifndef GAME_HPP
#define GAME_HPP

#include <vector.hpp>
#include <string>
#include "template.hpp"

namespace madieva {
  class Game
  {
  public:
    Game() noexcept = default;
    Game(const Template & tmpl);
    Game(const Game & game);
    Game(const std::string & filename);

    bool fill(size_t row, size_t col);
    bool emptyCell(size_t row, size_t col);
    bool win() const;

    size_t getRows() const noexcept;
    size_t getCols() const noexcept;
    const Vector< Vector< size_t > > & getRowHints() const;
    const Vector< Vector< size_t > > & getColHints() const;
    const Vector< Vector< int > > & getSolution() const;
    size_t getFilledCount() const noexcept;
    size_t getTotalFilled() const noexcept;
    const Vector< Vector< int > > & getState() const;
    void getMaxRowHintLength();
    void getMaxColHintLength();
    bool saveToFile(const std::string & filename) const;
    bool loadFromFile(const std::string & filename);
    void print(std::ostream& out) const;

  private:
    size_t rows_;
    size_t cols_;
    Vector< Vector< int > > state_;
    Vector< Vector< size_t > > rowHints_;
    Vector< Vector< size_t > > colHints_;
    Vector< Vector< int > > solution_;
    size_t filledCount_;
    size_t totalFilled_;
    size_t cachedMaxRowHintLen_;
    size_t cachedMaxColHintLen_;
    bool isInBounds(size_t row, size_t col) const;
  };
}

#endif
