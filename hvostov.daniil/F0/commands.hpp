#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <istream>
#include "dungeon_admin.hpp"
#include "hero_account.hpp"

namespace hvostov {
  namespace mmc {
    void newAccount(std::istream& in, std::ostream& out, HeroAccount& account);
    void showAccount(std::istream& in, std::ostream& out, HeroAccount& account);
    void showAccounts(std::istream& in, std::ostream& out, HeroAccount& account);
    void login(std::istream& in, std::ostream& out, HeroAccount& account);
    void deleteAccount(std::istream& in, std::ostream& out, HeroAccount& account);
    void newHero(std::istream& in, std::ostream& out, HeroAccount& account);
    void joinWorld(std::istream& in, std::ostream& out, HeroAccount& account);
    void exit(std::istream& in, std::ostream& out, HeroAccount& account);
    void chooseDungeon(std::istream& in, std::ostream& out, HeroAccount& account);
    void abandonDungeon(std::istream& in, std::ostream& out, HeroAccount& account);
    void showDungeonStats(std::istream& in, std::ostream& out, HeroAccount& account);
    void showDungeons(std::istream& in, std::ostream& out, HeroAccount& account);
    void showCommands(std::istream&, std::ostream&, HeroAccount&);
  }

  namespace pg {
    void exit();
    void save(HeroAccount& account);
    void exitToMenu(HeroAccount& account);
    void showHeroStats(std::istream&, std::ostream&, HeroAccount&);
    void showBattleCommands(std::istream&, std::ostream&, HeroAccount&);
  }

  namespace adm {
    DungeonAdmin& getDungeonAdmin();
    bool isModifyActive();
    void adminMode(std::istream& in, std::ostream& out, HeroAccount& account);
    void exitAdminMode(std::istream& in, std::ostream& out);

    void dungeonCreate(std::istream& in, std::ostream& out);
    void dungeonGenerate(std::istream& in, std::ostream& out);
    void dungeonDelete(std::istream& in, std::ostream& out);
    void dungeonShow(std::istream& in, std::ostream& out);
    void dungeonShowActive(std::istream&, std::ostream& out);
    void modifyMode(std::istream& in, std::ostream& out);
    void exitModifyMode(std::istream& in, std::ostream& out);

    void addStage(std::istream& in, std::ostream& out);
    void removeStage(std::istream& in, std::ostream& out);
    void addPath(std::istream& in, std::ostream& out);
    void removePath(std::istream& in, std::ostream& out);
    void showStage(std::istream& in, std::ostream& out);
    void showPath(std::istream& in, std::ostream& out);
    void addMonster(std::istream& in, std::ostream& out);
    void removeMonster(std::istream& in, std::ostream& out);
    void changeReward(std::istream& in, std::ostream& out);
    void showMonsterTypes(std::istream& in, std::ostream& out);
    void showAdminCommands(std::istream&, std::ostream&);
    void showModifyCommands(std::istream&, std::ostream&);
  }
}

#endif
