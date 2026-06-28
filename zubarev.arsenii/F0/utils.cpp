#include "utils.hpp"
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdlib>

std::string zubarev::detail::getCurrentDateTime()
{

  std::chrono::time_point< std::chrono::system_clock > now = std::chrono::system_clock::now();

  std::time_t now_time = std::chrono::system_clock::to_time_t(now);

  std::tm* now_tm = std::localtime(&now_time);

  std::stringstream ss;
  ss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");

  return ss.str();
}

std::string zubarev::detail::getCurrentUser()
{
  char* user = nullptr;

#if defined(_WIN32) || defined(_WIN64)
  user = std::getenv("USERNAME");
#else
  user = std::getenv("USER");
#endif

  if (user != nullptr) {
    return std::string(user);
  }

  return "root";
}

zubarev::Queue< std::string > zubarev::detail::resolvePath(const std::string& path)
{
  Queue< std::string > dirs;
  std::string cur_str = "";

  for (char c : path) {
    if (c == '/') {
      if (!cur_str.empty()) {
        if (cur_str != "~") {
          dirs.push(cur_str);
        }
        cur_str.clear();
      }
    } else {
      cur_str += c;
    }
  }

  if (!cur_str.empty() && cur_str != "~") {
    dirs.push(cur_str);
  }

  return dirs;
}

std::string zubarev::detail::formatLsColumns(const Vector< std::string >& names, size_t terminalWidth)
{
  if (names.isEmpty()) {
    return "";
  }

  size_t maxLen = 0;
  for (auto name = names.begin(); name != names.end(); ++name) {
    maxLen = std::max(maxLen, name->size());
  }

  const size_t colWidth = maxLen + 2;

  size_t columns = terminalWidth / colWidth;
  if (columns == 0) {
    columns = 1;
  }

  columns = std::min(columns, names.getSize());

  size_t rows = (names.getSize() + columns - 1) / columns;

  std::string result;

  for (size_t row = 0; row < rows; ++row) {
    for (size_t col = 0; col < columns; ++col) {
      size_t index = row + col * rows;

      if (index >= names.getSize()) {
        continue;
      }

      result += names[index];

      bool lastPrintedColumn = (col == columns - 1) || (row + (col + 1) * rows >= names.getSize());

      if (!lastPrintedColumn) {
        result.append(colWidth - names[index].size(), ' ');
      }
    }

    result += '\n';
  }

  return result;
}
void zubarev::detail::printBannerColored()
{
  const std::string RESET = "\033[0m";
  const std::string WHITE = "\033[97m";
  const std::string BOLD = "\033[1m";

  std::cout << BOLD << WHITE;
  std::cout << "███████╗██╗██╗     ███████╗"
            << "███████╗██╗   ██╗███████╗"
            << "████████╗███████╗███╗   ███╗\n";
  std::cout << "██╔════╝██║██║     ██╔════╝"
            << "██╔════╝╚██╗ ██╔╝██╔════╝"
            << "╚══██╔══╝██╔════╝████╗ ████║\n";
  std::cout << "█████╗  ██║██║     █████╗  "
            << "███████╗ ╚████╔╝ ███████╗"
            << "   ██║   █████╗  ██╔████╔██║\n";
  std::cout << "██╔══╝  ██║██║     ██╔══╝  "
            << "╚════██║  ╚██╔╝  ╚════██║"
            << "   ██║   ██╔══╝  ██║╚██╔╝██║\n";
  std::cout << "██║     ██║███████╗███████╗"
            << "███████║   ██║   ███████║"
            << "   ██║   ███████╗██║ ╚═╝ ██║\n";
  std::cout << "╚═╝     ╚═╝╚══════╝╚══════╝"
            << "╚══════╝   ╚═╝   ╚══════╝"
            << "   ╚═╝   ╚══════╝╚═╝     ╚═╝\n";
  std::cout << RESET << "\n";

  std::cout << "        Custom Virtual File System\n";
  std::cout << "        Powered by Robin Hood Hashing\n";
  std::cout << "        Built with custom data structures\n";
  std::cout << "        and handcrafted container implementations\n\n";

  std::cout << "        Type 'help' to view all commands\n\n";
}
