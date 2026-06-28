#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include <my_vector/top-it-vector.hpp>
#include <my_queue/queue.hpp>
namespace zubarev
{
  namespace detail
  {
    std::string getCurrentDateTime();
    std::string getCurrentUser();
    Queue< std::string > resolvePath(const std::string& path);
    std::string formatLsColumns(const Vector< std::string >& names, size_t terminalWidth = 80);
    void printBannerColored();

  }
}

#endif
