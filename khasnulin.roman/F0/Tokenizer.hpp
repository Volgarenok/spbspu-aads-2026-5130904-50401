#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <Vector.hpp>
#include <string>
namespace khasnulin
{
  struct Token
  {
    bool isQuoted;
    std::string text;
    Token() noexcept;
    Token(bool quoted, const std::string &text);
    Token(bool quoted, std::string &&text) noexcept;
  };

  Vector< Token > tokenize(const std::string &line);
}

#endif
