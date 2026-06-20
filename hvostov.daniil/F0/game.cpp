#include "game.hpp"
#include <exception>
#include <iostream>
#include <limits>
#include <robin_hood_hash_table.hpp>
#include "battle_system.hpp"
#include "commands.hpp"
#include "dungeon.hpp"
#include "hero_account.hpp"
#include "save_manager.hpp"
#include "skill_manager.hpp"

void hvostov::Game::run()
{
  mainMenu();
}

void hvostov::Game::mainMenu()
{
  std::cout << "Welcome to terminal hvostov!\n";
  std::cout << "You can start new game or load previous\n";
  std::cout << "┌────────────────────"
            << "─────────────────────"
            << "────────────────────┐\n";
  std::cout << "│                   TERMINAL RPG                              │\n";
  std::cout << "├────────────────────"
            << "─────────────────────"
            << "────────────────────┤\n";
  std::cout << "│  new-account <name>       - Create new account              │\n";
  std::cout << "│  show-accounts            - List all accounts               │\n";
  std::cout << "│  show-account <name>      - Show account info               │\n";
  std::cout << "│  delete-account <name>    - Delete account                  │\n";
  std::cout << "│  login <name>             - Login to account                │\n";
  std::cout << "│  new-hero <class>         - Create new hero                 │\n";
  std::cout << "│  choose-dungeon <name>    - Select dungeon                  │\n";
  std::cout << "│  show-dungeons            - List available dungeons         │\n";
  std::cout << "│  abandon-dungeon          - Abandon current dungeon         │\n";
  std::cout << "│  dungeon-stats            - Show dungeon statistics         │\n";
  std::cout << "│  join-world               - Start/continue dungeon          │\n";
  std::cout << "│  admin-mode               - Enter admin mode                │\n";
  std::cout << "│  show-commands            - Show this help                  │\n";
  std::cout << "│  exit                     - Exit game                       │\n";
  std::cout << "└────────────────────"
            << "─────────────────────"
            << "────────────────────┘\n";
  std::cout << "> ";
  RobinHoodHashTable< std::string, hvostov::cmd_t > cmds;
  cmds["new-account"] = hvostov::mmc::newAccount;
  cmds["show-accounts"] = hvostov::mmc::showAccounts;
  cmds["show-account"] = hvostov::mmc::showAccount;
  cmds["delete-account"] = hvostov::mmc::deleteAccount;
  cmds["login"] = hvostov::mmc::login;
  cmds["new-hero"] = hvostov::mmc::newHero;
  cmds["choose-dungeon"] = hvostov::mmc::chooseDungeon;
  cmds["show-dungeons"] = hvostov::mmc::showDungeons;
  cmds["abandon-dungeon"] = hvostov::mmc::abandonDungeon;
  cmds["dungeon-stats"] = hvostov::mmc::showDungeonStats;
  cmds["join-world"] = hvostov::mmc::joinWorld;
  cmds["exit"] = hvostov::mmc::exit;
  cmds["show-commands"] = hvostov::mmc::showCommands;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.find(cmd) != cmds.end()) {
        cmds.at(cmd)(std::cin, std::cout, account_);
        if (cmd == "join-world" && account_.account_name != "") {
          processGame();
        }
      } else if (cmd == "admin-mode") {
        hvostov::adm::adminMode(std::cin, std::cout, account_);
        adminLoop();
      } else {
        std::cout << "<INVALID COMMAND>\n";
        auto toignore = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(toignore, '\n');
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << "\n";
      std::cin.clear();
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

void hvostov::Game::adminLoop()
{
  RobinHoodHashTable< std::string, hvostov::admin_cmd_t > cmds;
  cmds["exit-admin-mode"] = hvostov::adm::exitAdminMode;
  cmds["create-dungeon"] = hvostov::adm::dungeonCreate;
  cmds["generate-dungeon"] = hvostov::adm::dungeonGenerate;
  cmds["delete-dungeon"] = hvostov::adm::dungeonDelete;
  cmds["show-dungeon"] = hvostov::adm::dungeonShow;
  cmds["exit"] = [](std::istream&, std::ostream&) { std::exit(0); };
  cmds["show-dungeons"] = [](std::istream& in, std::ostream& out) {
    hvostov::HeroAccount dummy;
    hvostov::mmc::showDungeons(in, out, dummy);
  };
  cmds["show-commands"] = hvostov::adm::showAdminCommands;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.find(cmd) != cmds.end()) {
        cmds.at(cmd)(std::cin, std::cout);
        if (cmd == "exit-admin-mode")
          return;
      } else if (cmd == "modify-dungeon") {
        hvostov::adm::modifyMode(std::cin, std::cout);
        if (hvostov::adm::isModifyActive())
          modifyLoop();
      } else {
        std::cout << "<INVALID COMMAND>\n";
        auto toignore = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(toignore, '\n');
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << "\n";
      std::cin.clear();
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

void hvostov::Game::modifyLoop()
{
  RobinHoodHashTable< std::string, hvostov::modify_cmd_t > cmds;
  cmds["exit-modify-mode"] = hvostov::adm::exitModifyMode;
  cmds["add-stage"] = hvostov::adm::addStage;
  cmds["remove-stage"] = hvostov::adm::removeStage;
  cmds["add-path"] = hvostov::adm::addPath;
  cmds["remove-path"] = hvostov::adm::removePath;
  cmds["show-stage"] = hvostov::adm::showStage;
  cmds["show-path"] = hvostov::adm::showPath;
  cmds["add-monster"] = hvostov::adm::addMonster;
  cmds["remove-monster"] = hvostov::adm::removeMonster;
  cmds["change-reward"] = hvostov::adm::changeReward;
  cmds["show-monsters"] = hvostov::adm::showMonsterTypes;
  cmds["show-dungeon"] = hvostov::adm::dungeonShowActive;
  cmds["show-commands"] = hvostov::adm::showModifyCommands;
  cmds["show-dungeons"] = [](std::istream& in, std::ostream& out) {
    hvostov::HeroAccount dummy;
    hvostov::mmc::showDungeons(in, out, dummy);
  };
  cmds["exit"] = [](std::istream&, std::ostream&) { std::exit(0); };

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.find(cmd) != cmds.end()) {
        cmds.at(cmd)(std::cin, std::cout);
        if (cmd == "exit-modify-mode")
          return;
      } else {
        std::cout << "<INVALID COMMAND>\n";
        auto toignore = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(toignore, '\n');
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << "\n";
      std::cin.clear();
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
}

void hvostov::Game::processGame()
{
  if (!account_.hasDungeon()) {
    std::cout << "No dungeon selected\n";
    return;
  }

  account_.startDungeon();

  hvostov::Dungeon dungeon;
  if (!dungeon.load(account_.active_dungeon)) {
    std::cout << "Failed to load dungeon\n";
    return;
  }

  dungeon.start();

  for (size_t i = 1; i < account_.current_dungeon_floor; i++) {
    dungeon.nextFloor();
  }

  while (!dungeon.isCompleted()) {
    auto* floor = dungeon.currentFloor();
    if (!floor)
      break;

    std::cout << dungeon.showProgress();
    std::cout << dungeon.showRoomChoice();

    std::cout << "Path> ";
    std::string input;
    std::cin >> input;

    if (input == "exit-to-menu") {
      account_.loadFromHeroes();
      hvostov::SaveManager::save(account_);
      std::cout << "Returning to menu...\n";
      return;
    }
    if (input == "exit") {
      std::cout << "Exiting game...\n";
      std::exit(0);
    }
    if (input == "hero-stats") {
      hvostov::pg::showHeroStats(std::cin, std::cout, account_);
      continue;
    }
    if (input == "show-commands") {
      std::cout << "  <path_number>  - Choose path\n";
      std::cout << "  hero-stats     - Show hero stats\n";
      std::cout << "  exit-to-menu   - Exit to menu\n";
      std::cout << "  exit           - Exit game\n";
      continue;
    }

    int path_choice = 0;
    try {
      path_choice = std::stoi(input);
    } catch (...) {
      std::cout << "<INVALID>\n";
      continue;
    }

    if (path_choice < 1 || path_choice > static_cast< int >(floor->getRoomCount())) {
      std::cout << "<INVALID PATH>\n";
      continue;
    }

    dungeon.chooseRoom(path_choice - 1);
    auto* room = dungeon.currentRoom();

    if (room->hasMonsters()) {
      hvostov::BattleSystem battle;

      for (auto& hero_ptr : account_.party_ptrs) {
        if (!hero_ptr->getStats().getIsDead())
          battle.addHero(hero_ptr.get());
      }
      for (auto& enemy_ptr : room->monsters) {
        battle.addEnemy(enemy_ptr.get());
      }

      battle.startBattle();
      std::cout << battle.showBattleStatus();

      while (!battle.isBattleOver()) {
        auto* current = battle.currentActor();
        if (!current)
          break;

        if (current->getActorType() == hvostov::ActorType::Hero) {
          auto& sm = current->getSkillManager();

          for (size_t i = 0; i < sm.getSkillCount(); i++) {
            if (!sm.isSkillLocked(i)) {
              float cost = sm.getSkillCost(i);
              std::string target = "";
              switch (sm.getSkillTargetType(i)) {
                case hvostov::SkillTarget::Enemy:
                  target = "[Enemy]";
                  break;
                case hvostov::SkillTarget::Ally:
                  target = "[Ally]";
                  break;
                case hvostov::SkillTarget::Self:
                  target = "[Self]";
                  break;
                case hvostov::SkillTarget::AllEnemies:
                  target = "[All]";
                  break;
                case hvostov::SkillTarget::AllAllies:
                  target = "[All]";
                  break;
              }
              std::cout << "  " << i << ". " << sm.getSkillName(i) << " " << target;
              if (cost > 0)
                std::cout << " [" << cost << " MP]";
              std::cout << " -- " << sm.getSkillDescription(i) << "\n";
            }
          }

          std::cout << "Skill> ";
          std::cin >> input;

          if (input == "exit-to-menu") {
            account_.loadFromHeroes();
            hvostov::SaveManager::save(account_);
            std::cout << "Returning to menu...\n";
            return;
          }
          if (input == "exit") {
            std::cout << "Exiting game...\n";
            std::exit(0);
          }
          if (input == "hero-stats") {
            hvostov::pg::showHeroStats(std::cin, std::cout, account_);
            continue;
          }
          if (input == "show-commands") {
            std::cout << "  <skill_id>     - Use skill\n";
            std::cout << "  hero-stats     - Show hero stats\n";
            std::cout << "  exit-to-menu   - Exit to menu\n";
            std::cout << "  exit           - Exit game\n";
            continue;
          }

          int skill_idx = 0;
          try {
            skill_idx = std::stoi(input);
          } catch (...) {
            std::cout << "<INVALID>\n";
            continue;
          }

          if (skill_idx < 0 || skill_idx >= static_cast< int >(sm.getSkillCount()) || sm.isSkillLocked(skill_idx)) {
            std::cout << "<INVALID SKILL>\n";
            continue;
          }

          auto target_type = sm.getSkillTargetType(skill_idx);
          size_t target_idx = 0;

          if (target_type == hvostov::SkillTarget::Enemy || target_type == hvostov::SkillTarget::AllEnemies) {
            auto targets = battle.getAliveEnemies();
            if (targets.size() > 1) {
              std::cout << "Target> ";
              std::cin >> input;

              if (input == "exit-to-menu") {
                account_.loadFromHeroes();
                hvostov::SaveManager::save(account_);
                std::cout << "Returning to menu...\n";
                return;
              }
              if (input == "exit") {
                std::cout << "Exiting game...\n";
                std::exit(0);
              }
              if (input == "show-commands") {
                std::cout << "  <target_id>    - Choose target\n";
                std::cout << "  exit-to-menu   - Exit to menu\n";
                std::cout << "  exit           - Exit game\n";
                continue;
              }

              try {
                target_idx = std::stoi(input);
              } catch (...) {
                std::cout << "<INVALID>\n";
                continue;
              }
              if (target_idx >= targets.size()) {
                std::cout << "<INVALID TARGET>\n";
                continue;
              }
            }
          } else if (target_type == hvostov::SkillTarget::Ally || target_type == hvostov::SkillTarget::AllAllies) {
            auto targets = battle.getAliveHeroes();
            if (targets.size() > 1) {
              std::cout << "Target> ";
              std::cin >> input;

              if (input == "exit-to-menu") {
                account_.loadFromHeroes();
                hvostov::SaveManager::save(account_);
                std::cout << "Returning to menu...\n";
                return;
              }
              if (input == "exit") {
                std::cout << "Exiting game...\n";
                std::exit(0);
              }
              if (input == "show-commands") {
                std::cout << "  <target_id>    - Choose target\n";
                std::cout << "  exit-to-menu   - Exit to menu\n";
                std::cout << "  exit           - Exit game\n";
                continue;
              }

              try {
                target_idx = std::stoi(input);
              } catch (...) {
                std::cout << "<INVALID>\n";
                continue;
              }
              if (target_idx >= targets.size()) {
                std::cout << "<INVALID TARGET>\n";
                continue;
              }
            }
          }

          battle.executeHeroAction(skill_idx, target_idx, target_type);
        } else {
          battle.executeEnemyAction();
        }

        std::cout << battle.showBattleStatus();
      }

      if (battle.heroesWon()) {
        std::cout << "VICTORY!\n";
        battle.endBattle();
        dungeon.clearRoom();

        account_.current_dungeon_floor = dungeon.currentFloorIndex() + 2;

        int floor_num = static_cast< int >(dungeon.currentFloorIndex() + 1);

        for (auto& hero_ptr : account_.party_ptrs) {
          if (hero_ptr->getStats().getIsDead()) {
            if (room->reward_quality == hvostov::RewardQuality::Legendary) {
              std::cout << "\n" << hero_ptr->getName() << " is dead. Legendary reward may revive!\n";
            } else {
              std::cout << "\n" << hero_ptr->getName() << " is dead. No reward.\n";
              continue;
            }
          }

          std::cout << "\nREWARD FOR " << hero_ptr->getName() << ":\n";

          std::vector< std::unique_ptr< hvostov::Reward > > rewards;
          switch (room->reward_quality) {
            case hvostov::RewardQuality::Common:
              rewards = hvostov::RewardFactory::generateCommonRewards(floor_num);
              break;
            case hvostov::RewardQuality::Rare:
              rewards = hvostov::RewardFactory::generateRareRewards(*hero_ptr, floor_num);
              break;
            case hvostov::RewardQuality::Epic:
              rewards = hvostov::RewardFactory::generateEpicRewards(*hero_ptr, floor_num);
              break;
            case hvostov::RewardQuality::Legendary:
              rewards = hvostov::RewardFactory::generateLegendaryRewards(*hero_ptr, floor_num);
              break;
          }

          if (!rewards.empty()) {
            while (true) {
              std::cout << "CHOOSE REWARD:\n";
              for (size_t i = 0; i < rewards.size(); i++)
                std::cout << "  " << (i + 1) << ". " << rewards[i]->getDescription() << "\n";
              std::cout << "Reward> ";
              std::cin >> input;

              if (input == "exit-to-menu") {
                account_.loadFromHeroes();
                hvostov::SaveManager::save(account_);
                std::cout << "Returning to menu...\n";
                return;
              }
              if (input == "exit") {
                std::cout << "Exiting game...\n";
                std::exit(0);
              }
              if (input == "show-commands") {
                std::cout << "  <reward_number> - Choose reward\n";
                std::cout << "  exit-to-menu    - Exit to menu\n";
                std::cout << "  exit            - Exit game\n";
                continue;
              }

              int rc = 0;
              try {
                rc = std::stoi(input);
              } catch (...) {
                std::cout << "<INVALID>\n";
                continue;
              }
              if (rc >= 1 && rc <= static_cast< int >(rewards.size())) {
                rewards[static_cast< size_t >(rc - 1)]->apply(*hero_ptr);
                break;
              }
              std::cout << "<INVALID REWARD>\n";
            }
          }
        }

        account_.loadFromHeroes();
        hvostov::SaveManager::save(account_);
      } else {
        std::cout << "DEFEAT!\n";
        battle.endBattle();
        account_.loadFromHeroes();
        account_.failDungeon();
        hvostov::SaveManager::save(account_);
        return;
      }
    } else {
      std::cout << "Empty room, moving on...\n";
    }

    if (!dungeon.isLastFloor()) {
      dungeon.nextFloor();
    } else {
      std::cout << "DUNGEON COMPLETED!\n";
      account_.completeDungeon();
      account_.loadFromHeroes();
      hvostov::SaveManager::save(account_);
      return;
    }
  }
}
