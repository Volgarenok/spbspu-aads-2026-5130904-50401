#ifndef YALOVSKY_F0_FILE_IO_HPP
#define YALOVSKY_F0_FILE_IO_HPP

#include <iosfwd>

namespace yalovsky
{
  class GraphStorage;

  void writeStorage(std::ostream& output, const GraphStorage& storage);
  void readStorage(std::istream& input, GraphStorage& storage);
}

#endif
