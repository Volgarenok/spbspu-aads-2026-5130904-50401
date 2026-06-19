#ifndef INPUT_HPP
#define INPUT_HPP

#include "dataset.hpp"

#include <cstddef>
#include <iosfwd>
#include <string>

namespace lukashevich
{
  class TokenReader
  {
  public:
    explicit TokenReader(const std::string & line);

    bool read(std::string & token);
    bool empty() const;

  private:
    std::string line_;
    std::size_t position_;

    static bool isSpace(char symbol);
    void skipSpaces();
  };

  int parseInt(const std::string & token);
  void inputDataset(std::istream & input, DatasetTable & dataSets);
}

#endif
