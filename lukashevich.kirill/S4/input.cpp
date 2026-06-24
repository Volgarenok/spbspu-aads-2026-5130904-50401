#include "input.hpp"

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <istream>
#include <stdexcept>

lukashevich::TokenReader::TokenReader(const std::string & line):
  line_(line),
  position_(0)
{}

bool lukashevich::TokenReader::read(std::string & token)
{
  skipSpaces();
  if (position_ == line_.size())
  {
    return false;
  }
  const std::size_t begin = position_;
  while (position_ < line_.size() && !isSpace(line_[position_]))
  {
    ++position_;
  }
  token.assign(line_, begin, position_ - begin);
  return true;
}

bool lukashevich::TokenReader::empty() const
{
  std::size_t position = position_;
  while (position < line_.size() && isSpace(line_[position]))
  {
    ++position;
  }
  return position == line_.size();
}

bool lukashevich::TokenReader::isSpace(char symbol)
{
  return symbol == ' ' || symbol == '\t' || symbol == '\r';
}

void lukashevich::TokenReader::skipSpaces()
{
  while (position_ < line_.size() && isSpace(line_[position_]))
  {
    ++position_;
  }
}

int lukashevich::parseInt(const std::string & token)
{
  if (token.empty())
  {
    throw std::invalid_argument("Empty integer token");
  }
  errno = 0;
  char * end = nullptr;
  const long value = std::strtol(token.c_str(), &end, 10);
  if (errno == ERANGE || end == token.c_str() || *end != '\0')
  {
    throw std::invalid_argument("Invalid integer key");
  }
  if (value < INT_MIN || value > INT_MAX)
  {
    throw std::out_of_range("Integer key is out of range");
  }
  return static_cast< int >(value);
}

void lukashevich::inputDataset(std::istream & input, DatasetTable & dataSets)
{
  std::string line;
  while (std::getline(input, line))
  {
    TokenReader reader(line);
    std::string name;
    if (!reader.read(name))
    {
      continue;
    }

    DataSet dataSet;
    std::string keyToken;
    while (reader.read(keyToken))
    {
      std::string value;
      if (!reader.read(value))
      {
        throw std::invalid_argument("Dataset line has no value for a key");
      }
      const int key = parseInt(keyToken);
      dataSet.push(key, value);
    }
    dataSets.set(name, dataSet);
  }

  if (!input.eof() && input.fail())
  {
    throw std::runtime_error("Failed to read datasets file");
  }
}
