#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <vector.hpp>
#include <string>

namespace madieva {

  class Template
  {
  public:
    Template() noexcept;
    Template(const std::string & filename);

    bool loadFromFile(const std::string & filename);

    size_t getRows() const noexcept;
    size_t getCols() const noexcept;
    size_t getFill() const noexcept;
    const Vector< Vector< size_t > > & getRowHints() const;
    const Vector< Vector< size_t > > & getColHints() const;
    const Vector< Vector< int > > & getSolution() const;
    bool solvable() const noexcept;

  private:
    size_t rows_;
    size_t cols_;
    size_t fill_;
    Vector< Vector< size_t > > rowHints_;
    Vector< Vector< size_t > > colHints_;
    Vector< Vector< int > > solution_;
    bool isSolvable_;

    bool solve();
  };
}

#endif
