#ifndef MESSANGER_COMMANDS_HPP
#define MESSANGER_COMMANDS_HPP
#include <iostream>
#include "network.hpp"
namespace malashenko
{
  class Messanger {
  public:
    void user(std::iostream& in, std::ostream& out);
    void rmuser(std::iostream& in, std::ostream& out);
    void ls(std::iostream& in, std::ostream& out);
    void send(std::iostream& in, std::ostream& out);
    void rmmsg(std::iostream& in, std::ostream& out);
    void inbox(std::iostream& in, std::ostream& out);
    void outbox(std::iostream& in, std::ostream& out);
    void chat(std::iostream& in, std::ostream& out);
    void find(std::iostream& in, std::ostream& out);
    void clear(std::iostream& in, std::ostream& out);
    void mutual(std::iostream& in, std::ostream& out);
    void recommend(std::iostream& in, std::ostream& out);
    void path(std::iostream& in, std::ostream& out);
    void remove_inactive(std::iostream& in, std::ostream& out);
    void distance(std::iostream& in, std::ostream& out);
  private:
    Network net_;
  };
}

#endif