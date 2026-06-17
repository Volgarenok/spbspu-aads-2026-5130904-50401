#include "commands.hpp"
#include <fstream>
#include <limits>
#include <cctype>

vasyakin::Date readDate(std::istream& in)
{
  std::string token;
  if (!(in >> token))
  {
    throw std::runtime_error("Invalid date");
  }

  return vasyakin::Date::fromString(token);
}

std::string readQuotedToken(std::istream& in)
{
  std::string token;
  in >> token;

  if (token.empty())
  {
    return token;
  }

  if (token.front() == '"')
  {
    token = token.substr(1);

    if (!token.empty() && token.back() == '"')
    {
      return token.substr(0, token.size() - 1);
    }

    std::string rest;
    std::getline(in, rest, '"');
    token += rest;
  }

  return token;
}

std::string escapeSpaces(const std::string& s)
{
  std::string result = s;
  for (char& c : result)
  {
    if (c == ' ') c = '_';
  }

  return result;
}

std::string unescapeSpaces(const std::string& s)
{
  std::string result = s;
  for (char& c : result)
  {
    if (c == '_') c = ' ';
  }

  return result;
}
