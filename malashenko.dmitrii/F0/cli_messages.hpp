#ifndef CLI_MESSAGES_HPP
#define CLI_MESSAGES_HPP
#include <iostream>
#include <string>

namespace malashenko
{
  namespace detail
  {
    void successMsg(std::ostream& out, const std::string& msg);
    void errorMsg(std::ostream& out, const std::string& msg);
    void warningMsg(std::ostream& out, const std::string& msg);
    void printBanner(std::ostream& out);

  }
}

#endif
