#ifndef MESSANGER_COMMANDS_HPP
#define MESSANGER_COMMANDS_HPP
#include <istream>
#include "network.hpp"
namespace malashenko
{
  class Messenger {
  public:
    void user(std::istream& in, std::ostream& out);
    void rmuser(std::istream& in, std::ostream& out);
    void ls(std::istream& in, std::ostream& out);
    void send(std::istream& in, std::ostream& out);
    void rmmsg(std::istream& in, std::ostream& out);
    void inbox(std::istream& in, std::ostream& out);
    void outbox(std::istream& in, std::ostream& out);
    void chat(std::istream& in, std::ostream& out);
    void find(std::istream& in, std::ostream& out);
    void clear(std::istream& in, std::ostream& out);
    void mutual(std::istream& in, std::ostream& out);
    void recommend(std::istream& in, std::ostream& out);
    void path(std::istream& in, std::ostream& out);
    void remove_inactive(std::istream& in, std::ostream& out);
    void distance(std::istream& in, std::ostream& out);
    void help(std::istream& in, std::ostream& out);
    void save(std::istream& in, std::ostream& out);
    void load(std::istream& in, std::ostream& out);
  private:
    Network net_;
  };
}

#endif
