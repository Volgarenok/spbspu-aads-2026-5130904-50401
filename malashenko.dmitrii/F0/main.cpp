#include <iostream>
#include "messenger_commands.hpp"
#include "cuckoo_hash_table.hpp"
#include "cli_messages.hpp"
int main()
{
  using namespace malashenko;



  using cmd_t = void (Messenger::*)(std::istream&, std::ostream&);
  using str_t = std::string;

  CuckooHashTable< str_t, cmd_t, HmacHash< str_t >, SipHasher< str_t >, Equal< str_t > > commands;

  commands.insert("user", &Messenger::user);
  commands.insert("rmuser", &Messenger::rmuser);
  commands.insert("ls", &Messenger::ls);
  commands.insert("send", &Messenger::send);
  commands.insert("rmmsg", &Messenger::rmmsg);
  commands.insert("find", &Messenger::find);
  commands.insert("chat", &Messenger::chat);
  commands.insert("inbox", &Messenger::inbox);
  commands.insert("outbox", &Messenger::outbox);
  commands.insert("clear", &Messenger::clear);
  commands.insert("path", &Messenger::path);
  commands.insert("distance", &Messenger::distance);
  commands.insert("recommend", &Messenger::recommend);
  commands.insert("remove-inactive", &Messenger::remove_inactive);
  commands.insert("mutual", &Messenger::mutual);
  commands.insert("help", &Messenger::help);
  commands.insert("save", &Messenger::save);
  commands.insert("load", &Messenger::load);


  detail::printBanner(std::cout);
  str_t cmd;
  Messenger messenger;

  while (std::cin >> cmd)
  {
    try
    {
      (messenger.*commands.at(cmd))(std::cin, std::cout);
    }
    catch(const std::invalid_argument& e)
    {
      detail::errorMsg(std::cout, e.what());
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    catch (...)
    {
      detail::warningMsg(std::cout, "Invalid command");
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

  }

  return 0;
}

