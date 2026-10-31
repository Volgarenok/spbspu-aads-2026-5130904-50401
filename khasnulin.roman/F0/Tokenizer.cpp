#include "Tokenizer.hpp"
#include "Vector.hpp"
#include <stdexcept>
#include <string>
#include <utility>

khasnulin::Token::Token() noexcept:
    isQuoted(false),
    text()
{
}

khasnulin::Token::Token(bool quoted, const std::string &text):
    isQuoted(quoted),
    text(text)
{
}

khasnulin::Token::Token(bool quoted, std::string &&text) noexcept:
    isQuoted(quoted),
    text(std::move(text))
{
}

khasnulin::Vector< khasnulin::Token > khasnulin::tokenize(const std::string &line)
{
  Vector< Token > result;
  bool inQuote = false;
  bool afterQuote = false;
  std::string tempStr;
  std::size_t end = line.size();

  if (!line.empty() && line[end - 1] == '\r')
  {
    --end;
  }

  for (std::size_t i = 0; i < end; ++i)
  {
    char ch = line[i];
    if (afterQuote)
    {
      if (ch != ' ')
      {
        throw std::invalid_argument("tokenize: invalid line, there must be space or end of line after qoute");
      }
      afterQuote = false;
    }
    else if (inQuote)
    {
      if (ch == '\"')
      {
        inQuote = false;
        result.pushBack(Token(true, std::move(tempStr)));
        tempStr = "";
        afterQuote = true;
      }
      else
      {
        tempStr.push_back(ch);
      }
    }
    else if (ch != ' ' && ch != '\"')
    {
      tempStr.push_back(ch);
    }
    else if (ch == '\"')
    {
      if (tempStr.empty())
      {
        inQuote = true;
      }
      else
      {
        throw std::invalid_argument("tokenize: invalid line, quote must be after space or on new line");
      }
    }
    else
    {
      if (!tempStr.empty())
      {
        result.pushBack(Token(false, std::move(tempStr)));
        tempStr = "";
      }
    }
  }

  if (inQuote)
  {
    throw std::invalid_argument("tokenize: invalid line, all quotes must be closed");
  }

  if (!tempStr.empty())
  {
    result.pushBack(Token(false, std::move(tempStr)));
  }

  return result;
}
