#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

#include "library.hpp"

namespace burukov
{
  std::string readToken(std::istream& in);

  void parsingAddTitle(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingAddCopy(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingLend(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingReturn(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingDemandTitle(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingDemandGenre(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingSliceTitle(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingSliceGenre(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingRecommend(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingDeadStock(std::istream& in, std::ostream& out, LibraryManager& lib);
  void parsingDemandBalance(std::istream& in, std::ostream& out, LibraryManager& lib);
}

#endif
