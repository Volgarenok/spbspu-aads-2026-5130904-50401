#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

#include <vector.hpp>
#include <string>

namespace madieva {

  class Template
  {
  public:
    Template() = default;
    Template(int rows, int cols);

    bool loadFromFile(const std::string & filename);

    int getRows() const noexcept;
    int getCols() const noexcept;
    const Vector< Vector< int > > & getRowHints() const;
    const Vector< Vector< int > > & getColHints() const;
    const Vector< Vector< int > > & getSolution() const;

  private:
    int rows_;
    int cols_;
    Vector< Vector< int > > rowHints_;
    Vector< Vector< int > > colHints_;
    Vector< Vector< int > > solution_;
    bool isSolvable_;

    bool solve();
  };
}

#endif
