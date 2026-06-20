#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <vector>
#include "actor.hpp"
#include "hero_account.hpp"

namespace hvostov {
  using cmd_t = void (*)(std::istream&, std::ostream&, HeroAccount&);
  using admin_cmd_t = void (*)(std::istream&, std::ostream&);
  using modify_cmd_t = void (*)(std::istream&, std::ostream&);

  class Game {
  public:
    void run();

  private:
    void mainMenu();
    void adminLoop();
    void modifyLoop();
    void processGame();

    HeroAccount account_;
    std::vector< std::unique_ptr< Actor > > heroes_;
    std::vector< std::unique_ptr< Actor > > enemies_;
  };
}

#endif
