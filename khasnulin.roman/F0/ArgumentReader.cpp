#include "ArgumentReader.hpp"
#include "Entities.hpp"
#include "Tokenizer.hpp"
#include "Vector.hpp"
#include <cstddef>
#include <limits>
#include <stdexcept>

khasnulin::ArgumentReader::ArgumentReader(const Vector< Token > &tokens):
    tokens_(tokens),
    position_(1)
{
}

khasnulin::UserName khasnulin::ArgumentReader::userName()
{
  return UserName(nextPlain().text);
}

khasnulin::Category khasnulin::ArgumentReader::category()
{
  return Category(nextPlain().text);
}

khasnulin::Difficulty khasnulin::ArgumentReader::difficulty()
{
  return parseDifficulty(nextPlain().text);
}

khasnulin::ArticleName khasnulin::ArgumentReader::articleName()
{
  return ArticleName(nextQuoted().text);
}

khasnulin::Rating khasnulin::ArgumentReader::rating()
{
  std::size_t result = parseUnsignedNumber(nextPlain().text);
  if (result > Rating::maxRating)
  {
    throw std::invalid_argument("command arguments: error while pasing rating. it's more then 10");
  }
  return Rating(static_cast< int >(result));
}

std::size_t khasnulin::ArgumentReader::limit()
{
  std::size_t result = parseUnsignedNumber(nextPlain().text);
  if (result == 0)
  {
    throw std::invalid_argument("command arguments: limit value can't be zero");
  }
  return result;
}

khasnulin::TagSet khasnulin::ArgumentReader::restTags()
{
  TagSet result;
  while (position_ < tokens_.getSize())
  {
    Tag temp(nextPlain().text);
    result.insert({std::move(temp), true});
  }
  return result;
}

std::size_t khasnulin::ArgumentReader::remaining() const noexcept
{
  assert(!tokens_.isEmpty());
  return tokens_.getSize() - position_;
}

bool khasnulin::ArgumentReader::nextIsDifficulty() const noexcept
{
  if (position_ >= tokens_.getSize())
  {
    return false;
  }
  if (tokens_[position_].isQuoted)
  {
    return false;
  }
  try
  {
    parseDifficulty(tokens_[position_].text);
    return true;
  }
  catch (const std::invalid_argument &)
  {
    return false;
  }
}

void khasnulin::ArgumentReader::finish() const
{
  if (position_ < tokens_.getSize())
  {
    throw std::invalid_argument("Command Argument: can't finish, current position is not in the end");
  }
}

const khasnulin::Token &khasnulin::ArgumentReader::nextQuoted()
{
  if (position_ >= tokens_.getSize())
  {
    throw std::invalid_argument("Command Argument: can't get next, current position is in the end");
  }
  const Token &token = tokens_[position_];
  if (!token.isQuoted)
  {
    throw std::invalid_argument("Command Argument: next element hasn't got quoute");
  }
  ++position_;
  return token;
}

const khasnulin::Token &khasnulin::ArgumentReader::nextPlain()
{
  if (position_ >= tokens_.getSize())
  {
    throw std::invalid_argument("Command Argument: can't get next, current position is in the end");
  }
  const Token &token = tokens_[position_];
  if (token.isQuoted)
  {
    throw std::invalid_argument("Command Argument: next element has got quoute");
  }
  ++position_;
  return token;
}

std::size_t khasnulin::parseUnsignedNumber(const std::string &str)
{
  static constexpr std::size_t maxSizeT = std::numeric_limits< size_t >::max();
  std::size_t result = 0;
  if (str.empty())
  {
    throw std::invalid_argument("parsing number: empty string");
  }
  for (char ch : str)
  {
    if (ch >= '0' && ch <= '9')
    {
      std::size_t parsed = static_cast< std::size_t >(ch - '0');
      if (result <= (maxSizeT - parsed) / 10)
      {
        result = result * 10 + parsed;
      }
      else
      {
        throw std::invalid_argument("parsing number: string is too big, overflow");
      }
    }
    else
    {
      throw std::invalid_argument("parsing number: invalid number");
    }
  }
  return result;
}

std::string khasnulin::ArgumentReader::sessionName()
{
  return nextPlain().text;
}
