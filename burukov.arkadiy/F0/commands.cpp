#include "commands.hpp"
#include <stdexcept>

std::string burukov::readToken(std::istream& in)
{
  char c = '\0';
  while (in.get(c))
  {
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
    {
      continue;
    }
    if (c == '"')
    {
      std::string result;
      while (in.get(c))
      {
        if (c == '"')
        {
          return result;
        }
        result += c;
      }
      throw std::runtime_error("unclosed quote");
    }
    std::string result;
    result += c;
    while (in.get(c))
    {
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
      {
        return result;
      }
      result += c;
    }
    return result;
  }
  throw std::runtime_error("unexpected eof");
}

void burukov::parsingAddTitle(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string title = readToken(in);
  const std::string author = readToken(in);
  int year = 0;
  if (!(in >> year))
  {
    throw std::runtime_error("invalid year");
  }
  const std::string genre = readToken(in);
  lib.addTitle(title, author, year, genre);
  out << "<ADDED>\n";
}

void burukov::parsingAddCopy(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string title = readToken(in);
  const std::string copyId = readToken(in);
  lib.addCopy(title, copyId);
  out << "<OK>\n";
}

void burukov::parsingLend(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string title = readToken(in);
  const std::string reader = readToken(in);
  lib.lend(title, reader);
  out << "<ISSUED>\n";
}

void burukov::parsingReturn(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string copyId = readToken(in);
  lib.returnCopy(copyId);
  out << "<RETURNED>\n";
}

void burukov::parsingDemandTitle(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string title = readToken(in);
  int period = 0;
  if (!(in >> period))
  {
    throw std::runtime_error("invalid period");
  }
  lib.demandModelTitle(out, title, period);
}

void burukov::parsingDemandGenre(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string genre = readToken(in);
  int period = 0;
  if (!(in >> period))
  {
    throw std::runtime_error("invalid period");
  }
  lib.demandModelGenre(out, genre, period);
}

void burukov::parsingSliceTitle(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string title = readToken(in);
  int period = 0;
  if (!(in >> period))
  {
    throw std::runtime_error("invalid period");
  }
  lib.sliceTitle(out, title, period);
}

void burukov::parsingSliceGenre(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string genre = readToken(in);
  int period = 0;
  if (!(in >> period))
  {
    throw std::runtime_error("invalid period");
  }
  lib.sliceGenre(out, genre, period);
}

void burukov::parsingRecommend(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  const std::string title = readToken(in);
  size_t k = 0;
  if (!(in >> k))
  {
    throw std::runtime_error("invalid k");
  }
  lib.recommend(out, title, k);
}

void burukov::parsingDeadStock(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  int period = 0;
  double threshold = 0.0;
  if (!(in >> period >> threshold))
  {
    throw std::runtime_error("invalid input");
  }
  lib.deadStock(out, period, threshold);
}

void burukov::parsingDemandBalance(std::istream& in, std::ostream& out, LibraryManager& lib)
{
  int period = 0;
  if (!(in >> period))
  {
    throw std::runtime_error("invalid period");
  }
  lib.demandBalance(out, period);
}
