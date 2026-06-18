#ifndef DATE_UTILS_HPP
#define DATE_UTILS_HPP

#include <string>

namespace chernov {
  bool isValidDate(const std::string & dateStr);
  std::string toStorageFormat(const std::string & dateStr);
  std::string toDisplayFormat(const std::string & storage);
}

#endif
