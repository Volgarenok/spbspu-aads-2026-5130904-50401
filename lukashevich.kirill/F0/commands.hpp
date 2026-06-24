#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>

#include "matrix-table.hpp"

namespace lukashevich {
  void executeCommand(const std::string & line, MatrixTable & table, std::ostream & out);
}

#endif
