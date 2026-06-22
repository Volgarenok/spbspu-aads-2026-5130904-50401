#include "cli_messages.hpp"

void malashenko::detail::successMsg(std::ostream& out, const std::string& msg)
{
  out << "[OK] " << msg << '\n';
}


void malashenko::detail::errorMsg(std::ostream& out, const std::string& msg)
{
  out << "[ERROR] " << msg << '\n';
}


void malashenko::detail::warningMsg(std::ostream& out, const std::string& msg)
{
  out << "[WARN] " << msg << '\n';
}

void malashenko::detail::printBanner(std::ostream& out)
{
  out << "                           CLI MESSENGER (not MAX)\n";
  out << "                        TYPE help TO VIEW ALL COMMANDS\n";
  out << '\n';
}
