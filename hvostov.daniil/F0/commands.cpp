#include "commands.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <dirent.h>
#include "actor.hpp"
#include "dungeon.hpp"
#include "dungeon_admin.hpp"
#include "enemy_factory.hpp"
#include "hero_account.hpp"
#include "hero_factory.hpp"
#include "save_manager.hpp"
#include "types.hpp"

void hvostov::mmc::newHero(std::istream& in, std::ostream& out, hvostov::HeroAccount& account)
{
  std::string name, class_name;
  in >> std::quoted(name) >> class_name;
  if (account.party.size() >= 4) {
    throw std::logic_error("You have 4 heroes, this is maximum party size");
  }
  hvostov::HeroClass teammate_class = hvostov::getHeroClassByString(class_name);
  if (teammate_class == hvostov::HeroClass::NoClass) {
    out << "Class: " << class_name << " doesnt exist\n";
    return;
  }
  std::unique_ptr< hvostov::Hero > new_teammate_ptr = hvostov::createHeroByClass(teammate_class);
  hvostov::UnitSaveData new_teammate_data;
  new_teammate_data.loadFrom(*new_teammate_ptr);
  new_teammate_data.name = name;
  account.party.push_back(new_teammate_data);
  account.party_ptrs.push_back(std::move(new_teammate_ptr));
  out << "Now in your party: " << account.party.size() << " heroes\n";
  hvostov::SaveManager::save(account);
}

void hvostov::mmc::chooseDungeon(std::istream& in, std::ostream&, hvostov::HeroAccount& account)
{
  if (account.account_name.empty()) {
    throw std::logic_error("Please login first");
  }
  std::string name;
  in >> std::quoted(name);
  if (name.empty()) {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  account.chooseDungeon(name);
  if (account.hasDungeon()) {
    hvostov::SaveManager::save(account);
  }
}

void hvostov::mmc::abandonDungeon(std::istream&, std::ostream&, hvostov::HeroAccount& account)
{
  if (account.account_name.empty()) {
    throw std::logic_error("Please login first");
  }
  account.abandonDungeon();
  hvostov::SaveManager::save(account);
}

void hvostov::mmc::showDungeonStats(std::istream&, std::ostream& out, hvostov::HeroAccount& account)
{
  if (account.dungeon_stats.empty()) {
    out << "No dungeon statistics\n";
    return;
  }
  out << "DUNGEON STATISTICS:\n";
  out << std::string(40, '-') << "\n";
  for (const auto& stat : account.dungeon_stats) {
    out << "  " << stat.dungeon_name << ": " << stat.attempts << " attempts, " << stat.completions << " completions";
    if (account.active_dungeon == stat.dungeon_name) {
      out << (account.isDungeonStarted() ? " *ACTIVE*" : " *CHOSEN*");
    }
    out << "\n";
  }
}

void hvostov::mmc::showDungeons(std::istream&, std::ostream& out, hvostov::HeroAccount&)
{
  DIR* dir = opendir("dungeons");
  if (!dir) {
    out << "No dungeons found (dungeons/ folder missing)\n";
    return;
  }

  std::vector< std::string > dungeons;
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;
    if (name.length() > 4 && name.substr(name.length() - 4) == ".dng") {
      dungeons.push_back(name.substr(0, name.length() - 4));
    }
  }
  closedir(dir);

  if (dungeons.empty()) {
    out << "No dungeons available\n";
    return;
  }

  out << "AVAILABLE DUNGEONS:\n";
  out << std::string(40, '-') << "\n";
  for (size_t i = 0; i < dungeons.size(); i++) {
    out << "  " << (i + 1) << ". " << dungeons[i] << "\n";
  }
}

void hvostov::mmc::newAccount(std::istream& in, std::ostream& out, hvostov::HeroAccount& account)
{
  std::string name;
  in >> std::quoted(name);

  if (hvostov::SaveManager::heroExists(name)) {
    out << "Account already exists\n";
    return;
  }
  if (name == "") {
    out << "Please, enter the name\n";
    return;
  }
  account.account_name = name;
  account.party.clear();
  account.party_ptrs.clear();
  hvostov::SaveManager::save(account);
}

void hvostov::mmc::showAccount(std::istream& in, std::ostream& out, hvostov::HeroAccount&)
{
  std::string name;
  in >> std::quoted(name);

  if (!hvostov::SaveManager::heroExists(name)) {
    out << "Hero not found\n";
    return;
  }

  auto account = hvostov::SaveManager::load(name);

  out << "Name: " << account.account_name << "\n";
  out << "Your heroes:\n";
  for (size_t i = 0; i < account.party.size(); i++) {
    out << account.party[i].name << " " << hvostov::getClassNameByHeroClass(account.party[i].hero_class) << "\n";
  }
  out << "Stage: " << account.current_stage << "\n";
}

void hvostov::mmc::showAccounts(std::istream& in, std::ostream& out, hvostov::HeroAccount&)
{
  (void)in;
  auto heroes = hvostov::SaveManager::findSavedHeroes();

  if (heroes.empty()) {
    out << "No heroes\n";
    return;
  }

  for (const auto& name : heroes) {
    out << name << "\n";
  }
}

void hvostov::mmc::login(std::istream& in, std::ostream& out, hvostov::HeroAccount& account)
{
  std::string name;
  in >> std::quoted(name);

  if (!hvostov::SaveManager::heroExists(name)) {
    out << "Account not found\n";
    return;
  }

  account = hvostov::SaveManager::load(name);

  out << "Welcome, " << name << "\n";
}

void hvostov::mmc::deleteAccount(std::istream& in, std::ostream& out, hvostov::HeroAccount&)
{
  std::string name;
  in >> std::quoted(name);

  if (!hvostov::SaveManager::heroExists(name)) {
    out << "Account not found\n";
    return;
  }

  hvostov::SaveManager::deleteSave(name);
  out << "Account deleted\n";
}

void hvostov::mmc::joinWorld(std::istream&, std::ostream& out, hvostov::HeroAccount& account)
{
  if (account.account_name == "") {
    out << "Please, enter in the account or create new before play the game\n";
    return;
  }
  if (!account.hasDungeon()) {
    std::cout << "No dungeon chosen. Use 'choose-dungeon <name>'\n";
    return;
  }
  out << "Weclome to dungeon, " << account.account_name << "\n";
  out << "Your " << account.party.size() << " heroes:\n";
  for (size_t i = 0; i < account.party.size(); i++) {
    hvostov::UnitSaveData hero_data = account.party[i];
    out << i + 1 << ") " << hero_data.name << " " << hvostov::getClassNameByHeroClass(hero_data.hero_class) << "\n";
    out << "current hp: " << hero_data.current_health << "\n";
    out << "current resource: " << hero_data.current_resource << "\n";
  }
}

void hvostov::mmc::exit(std::istream&, std::ostream&, hvostov::HeroAccount&)
{
  std::cout << "Exiting game...\n";
  std::exit(0);
}

void hvostov::pg::exit()
{
  std::cout << "Exiting game...\n";
  std::exit(0);
}

void hvostov::pg::save(hvostov::HeroAccount& account)
{
  hvostov::SaveManager::save(account);
  std::cout << "Game saved successfully!\n";
}

void hvostov::pg::exitToMenu(hvostov::HeroAccount& account)
{
  hvostov::SaveManager::save(account);
  std::cout << "Returning to main menu...\n";
}

static bool admin_active = false;
static bool modify_active = false;
static hvostov::DungeonAdmin dungeonAdmin;

hvostov::DungeonAdmin& hvostov::adm::getDungeonAdmin()
{
  return dungeonAdmin;
}

static void autoSave()
{
  if (dungeonAdmin.activeDungeon()) {
    dungeonAdmin.saveDungeon(dungeonAdmin.activeDungeonName());
  }
}

void hvostov::adm::adminMode(std::istream&, std::ostream& out, hvostov::HeroAccount&)
{
  admin_active = true;
  out << "\n╔════════════════════════════════╗\n";
  out << "║      ADMIN MODE ACTIVE         ║\n";
  out << "╠════════════════════════════════╣\n";
  out << "║show-commands                   ║\n";
  out << "║create-dungeon <name> <floors>  ║\n";
  out << "║generate-dungeon <name> <floors>║\n";
  out << "║modify-dungeon <name>           ║\n";
  out << "║delete-dungeon <name>           ║\n";
  out << "║show-dungeon <name>             ║\n";
  out << "║show-dungeons                   ║\n";
  out << "║exit-admin-mode                 ║\n";
  out << "║exit                            ║\n";
  out << "╚════════════════════════════════╝\n";
}

void hvostov::adm::exitAdminMode(std::istream&, std::ostream& out)
{
  admin_active = false;
  modify_active = false;
  out << "ADMIN MODE DISABLED\n";
}

void hvostov::adm::dungeonCreate(std::istream& in, std::ostream& out)
{
  std::string name;
  size_t floors;
  in >> std::quoted(name) >> floors;

  if (name.empty() || floors == 0) {
    throw std::runtime_error("<INVALID COMMAND> — use: create-dungeon <name> <floors>");
  }

  if (dungeonAdmin.createDungeon(name, floors)) {
    auto* d = dungeonAdmin.activeDungeon();
    if (d) {
      for (size_t i = 0; i < floors; i++) {
        d->addFloor();
        for (size_t j = 0; j < 3; j++) {
          d->addRoom(i, "Path " + std::to_string(j + 1));
        }
      }
      d->save(name);
    }
    out << "DUNGEON CREATED: " << name << " (" << floors << " floors, " << (floors * 3) << " paths)\n";
  }
}

void hvostov::adm::dungeonShowActive(std::istream&, std::ostream& out)
{
  auto* d = hvostov::adm::getDungeonAdmin().activeDungeon();
  if (d)
    out << d->showFull();
  else
    out << "No active dungeon\n";
}

void hvostov::adm::dungeonGenerate(std::istream& in, std::ostream& out)
{
  std::string name;
  size_t floors;
  in >> std::quoted(name) >> floors;

  if (name.empty() || floors == 0) {
    throw std::runtime_error("<INVALID COMMAND> — use: generate-dungeon <name> <floors>");
  }

  if (dungeonAdmin.generateDungeon(name, floors)) {
    autoSave();
    out << "DUNGEON GENERATED: " << name << " (" << floors << " floors)\n";
  }
}

void hvostov::adm::dungeonDelete(std::istream& in, std::ostream& out)
{
  std::string name;
  in >> std::quoted(name);
  if (name.empty()) {
    throw std::runtime_error("<INVALID COMMAND> — enter dungeon name");
  }
  if (dungeonAdmin.deleteDungeon(name)) {
    out << "DUNGEON DELETED: " << name << "\n";
  }
}

void hvostov::adm::dungeonShow(std::istream& in, std::ostream& out)
{
  std::string name;
  in >> std::quoted(name);
  if (name.empty()) {
    throw std::runtime_error("<INVALID COMMAND> — enter dungeon name");
  }
  hvostov::Dungeon temp;
  if (temp.load(name)) {
    out << temp.showFull();
  } else {
    out << "<INVALID COMMAND> — dungeon not found\n";
  }
}

void hvostov::adm::modifyMode(std::istream& in, std::ostream& out)
{
  std::string name;
  in >> std::quoted(name);
  if (name.empty()) {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!dungeonAdmin.loadDungeon(name)) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  modify_active = true;
  out << "\n╔═══════════════";
  out << "═════════════════╗\n";
  out << "║ MODIFY MODE: " << std::setw(18) << std::left << name << "║\n";
  out << "╠════════════════════════════════╣\n";
  out << "║show-commands                   ║\n";
  out << "║add-stage <stage>               ║\n";
  out << "║remove-stage <stage>            ║\n";
  out << "║add-path <stage> [desc]         ║\n";
  out << "║remove-path <stage> <path>      ║\n";
  out << "║show-stage <stage>              ║\n";
  out << "║show-path <stage> <path>        ║\n";
  out << "║add-monster <st> <p> <type> <lv>║\n";
  out << "║remove-monster <st> <p> <idx>   ║\n";
  out << "║change-reward <st> <p>          ║\n";
  out << "║  <common|rare|epic|legendary>  ║\n";
  out << "║show-monsters                   ║\n";
  out << "║show-dungeon                    ║\n";
  out << "║exit-modify-mode                ║\n";
  out << "║exit                            ║\n";
  out << "╚════════════════════════════════╝\n";
}

void hvostov::adm::exitModifyMode(std::istream&, std::ostream& out)
{
  if (dungeonAdmin.activeDungeon()) {
    dungeonAdmin.saveDungeon(dungeonAdmin.activeDungeonName());
    out << "DUNGEON SAVED\n";
  }
  modify_active = false;
  out << "EXIT MODIFY MODE\n";
}

void hvostov::adm::addStage(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t index;
  in >> index;
  if (index == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    size_t idx = d->insertFloor(index - 1);
    for (size_t j = 0; j < 3; j++) {
      d->addRoom(idx, "Path " + std::to_string(j + 1));
    }
    autoSave();
    out << "STAGE ADDED at position " << index << " (3 paths default)\n";
  }
}

void hvostov::adm::removeStage(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage;
  in >> stage;
  if (stage == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (d && d->removeFloor(stage - 1)) {
    autoSave();
    out << "STAGE REMOVED: " << stage << "\n";
  } else {
    out << "<INVALID COMMAND> — stage not found\n";
  }
}

void hvostov::adm::addPath(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage;
  std::string desc;

  in >> stage;
  if (stage == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  in >> std::quoted(desc);
  if (!desc.empty() && desc[0] == ' ')
    desc = desc.substr(1);
  if (desc.empty())
    desc = "Path";

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    size_t idx = d->addRoom(stage - 1, desc);
    autoSave();
    out << "PATH ADDED: stage " << stage << ", path " << (idx + 1) << "\n";
  }
}

void hvostov::adm::removePath(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage, path;
  in >> stage >> path;
  if (stage == 0 || path == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (d && d->removeRoom(stage - 1, path - 1)) {
    autoSave();
    out << "PATH REMOVED: stage " << stage << ", path " << path << "\n";
  } else {
    throw std::runtime_error("<INVALID COMMAND>");
  }
}

void hvostov::adm::showStage(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage;
  in >> stage;
  if (stage == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto* d = dungeonAdmin.activeDungeon();
  if (!d)
    return;

  auto* f = d->floor(stage - 1);
  if (!f) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  out << "STAGE " << stage << " (" << f->getRoomCount() << " paths):\n";
  out << std::string(40, '-') << "\n";
  for (size_t i = 0; i < f->getRoomCount(); i++) {
    auto* r = f->room(i);
    out << "Path " << (i + 1) << ": " << (r->description.empty() ? "—" : r->description) << "\n";
    out << "  Monsters: ";
    if (r->monsters.empty()) {
      out << "none\n";
    } else {
      out << "\n";
      for (size_t k = 0; k < r->monsters.size(); k++) {
        out << "    " << (k + 1) << ". " << r->monsters[k]->getType() << " (Lvl " << r->monsters[k]->getStage()
            << ")\n";
      }
    }
    out << "  Reward: ";
    switch (r->reward_quality) {
      case hvostov::RewardQuality::Common:
        out << "Common";
        break;
      case hvostov::RewardQuality::Rare:
        out << "Rare";
        break;
      case hvostov::RewardQuality::Epic:
        out << "Epic";
        break;
      case hvostov::RewardQuality::Legendary:
        out << "Legendary";
        break;
    }
    out << "\n\n";
  }
}

void hvostov::adm::showPath(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage, path;
  in >> stage >> path;
  if (stage == 0 || path == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (!d)
    return;

  auto* f = d->floor(stage - 1);
  if (!f) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  auto* r = f->room(path - 1);
  if (!r) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  out << "Stage " << stage << ", Path " << path << "\n";
  out << std::string(40, '-') << "\n";
  out << "Description: " << (r->description.empty() ? "—" : r->description) << "\n";
  out << "Monsters: ";
  if (r->monsters.empty()) {
    out << "none\n";
  } else {
    out << "\n";
    for (size_t i = 0; i < r->monsters.size(); i++) {
      out << "  " << (i + 1) << ". " << r->monsters[i]->getType() << " (Lvl " << r->monsters[i]->getStage() << ")\n";
    }
  }
  out << "Reward: ";
  switch (r->reward_quality) {
    case hvostov::RewardQuality::Common:
      out << "Common";
      break;
    case hvostov::RewardQuality::Rare:
      out << "Rare";
      break;
    case hvostov::RewardQuality::Epic:
      out << "Epic";
      break;
    case hvostov::RewardQuality::Legendary:
      out << "Legendary";
      break;
  }
  out << "\n";
}

void hvostov::adm::addMonster(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage, path, level;
  std::string type;
  in >> stage >> path >> std::quoted(type) >> level;
  if (stage == 0 || path == 0 || level == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!hvostov::isValidEnemyType(type)) {
    out << "Use 'show-monsters' to see available types\n";
    throw std::runtime_error("<INVALID COMMAND>");
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    d->spawnMonster(stage - 1, path - 1, type, level);
    autoSave();
    out << "MONSTER ADDED: " << type << " (Lvl " << level << ")"
        << " to stage " << stage << ", path " << path << "\n";
  }
}

void hvostov::adm::removeMonster(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage, path, index;
  in >> stage >> path >> index;
  if (stage == 0 || path == 0 || index == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    d->removeMonster(stage - 1, path - 1, index - 1);
    autoSave();
    out << "MONSTER REMOVED: index " << index << " from stage " << stage << ", path " << path << "\n";
  }
}

void hvostov::adm::changeReward(std::istream& in, std::ostream& out)
{
  if (!modify_active) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  size_t stage, path;
  std::string quality;
  in >> stage >> path >> quality;
  if (stage == 0 || path == 0) {
    throw std::runtime_error("<INVALID COMMAND>");
  }

  hvostov::RewardQuality rq;
  if (quality == "legendary" || quality == "Legendary") {
    rq = hvostov::RewardQuality::Legendary;
  } else if (quality == "epic" || quality == "Epic") {
    rq = hvostov::RewardQuality::Epic;
  } else if (quality == "rare" || quality == "Rare" || quality == "RARE") {
    rq = hvostov::RewardQuality::Rare;
  } else if (quality == "common" || quality == "Common" || quality == "COMMON") {
    rq = hvostov::RewardQuality::Common;
  } else {
    out << "<INVALID COMMAND> — use 'common', 'rare', 'epic' or 'legendary'\n";
    return;
  }

  auto* d = dungeonAdmin.activeDungeon();
  if (d) {
    d->setRewardQuality(stage - 1, path - 1, rq);
    autoSave();
    out << "REWARD CHANGED: stage " << stage << ", path " << path << " -> " << quality << "\n";
  }
}

void hvostov::adm::showMonsterTypes(std::istream&, std::ostream& out)
{
  out << "AVAILABLE MONSTER TYPES:\n";
  out << std::string(40, '-') << "\n";
  auto types = hvostov::getAvailableEnemyTypes();
  for (size_t i = 0; i < types.size(); i++) {
    out << "  " << std::setw(2) << (i + 1) << ". " << types[i] << "\n";
  }
}

void hvostov::mmc::showCommands(std::istream&, std::ostream& out, hvostov::HeroAccount&)
{
  out << "MAIN MENU COMMANDS:\n";
  out << "  new-account <name>\n";
  out << "  show-accounts\n";
  out << "  show-account <name>\n";
  out << "  delete-account <name>\n";
  out << "  login <name>\n";
  out << "  new-hero <class>\n";
  out << "  choose-dungeon <name>\n";
  out << "  show-dungeons\n";
  out << "  abandon-dungeon\n";
  out << "  dungeon-stats\n";
  out << "  join-world\n";
  out << "  admin-mode\n";
  out << "  show-commands\n";
  out << "  exit\n";
}

void hvostov::adm::showAdminCommands(std::istream&, std::ostream& out)
{
  out << "ADMIN COMMANDS:\n";
  out << "  create-dungeon <name> <floors>\n";
  out << "  generate-dungeon <name> <floors>\n";
  out << "  modify-dungeon <name>\n";
  out << "  delete-dungeon <name>\n";
  out << "  show-dungeon <name>\n";
  out << "  show-dungeons\n";
  out << "  show-commands\n";
  out << "  exit-admin-mode\n";
  out << "  exit\n";
}

void hvostov::adm::showModifyCommands(std::istream&, std::ostream& out)
{
  out << "MODIFY COMMANDS:\n";
  out << "  add-stage <pos>\n";
  out << "  remove-stage <stage>\n";
  out << "  add-path <stage> <desc>\n";
  out << "  remove-path <stage> <path>\n";
  out << "  show-stage <stage>\n";
  out << "  show-path <stage> <path>\n";
  out << "  add-monster <st> <p> <type> <lvl>\n";
  out << "  remove-monster <st> <p> <idx>\n";
  out << "  change-reward <st> <p> <common|rare|epic|legendary>\n";
  out << "  show-monsters\n";
  out << "  show-dungeon\n";
  out << "  show-commands\n";
  out << "  exit-modify-mode\n";
  out << "  exit\n";
}

void hvostov::pg::showBattleCommands(std::istream&, std::ostream& out, hvostov::HeroAccount&)
{
  out << "BATTLE COMMANDS:\n";
  out << "  <skill_id> <target_id>  - Use skill on target\n";
  out << "  save                    - Save game\n";
  out << "  exit-to-menu            - Exit to main menu\n";
  out << "  show-commands           - Show this help\n";
  out << "  hero-stats              - Show all hero stats\n";
}

bool hvostov::adm::isModifyActive()
{
  return modify_active;
}

void hvostov::pg::showHeroStats(std::istream&, std::ostream& out, hvostov::HeroAccount& account)
{
  for (auto& hero_ptr : account.party_ptrs) {
    auto& s = hero_ptr->getStats();
    out << "\n" << hero_ptr->getName() << ":\n";
    out << "  HP:    " << static_cast< int >(s.getCurrentHealth()) << "/"
        << static_cast< int >(s.getHealth().getTotal()) << "\n";
    out << "  MP:    " << static_cast< int >(s.getCurrentResource()) << "/"
        << static_cast< int >(s.getResource().getTotal()) << "\n";
    out << "  REGEN: " << static_cast< int >(s.getResourceRegen()) << "/turn\n";
    out << "  ATK:   " << static_cast< int >(s.getDamage().getTotal()) << "\n";
    out << "  DEF:   " << static_cast< int >(s.getDefense().getTotal()) << "\n";
    out << "  SPD:   " << static_cast< int >(s.getSpeed().getTotal()) << "\n";
    out << "  CRIT:  " << static_cast< int >(s.getCritChance().getBase() * 100) << "%\n";
    out << "  CRITD: " << static_cast< int >(s.getCritDamage().getBase() * 100) << "%\n";
    out << "  BONUS: " << static_cast< int >(s.getDamageBonus().getBase() * 100) << "%\n";
    out << "  REDUC: " << static_cast< int >(s.getDamageReduction().getBase() * 100) << "%\n";
    out << "  SKILLS:\n";
    auto& sm = hero_ptr->getSkillManager();
    for (size_t i = 0; i < sm.getSkillCount(); i++) {
      out << "    " << i << ". " << sm.getSkillName(i) << " (Lvl " << sm.getSkillLevel(i) << ")";
      if (sm.isSkillLocked(i))
        out << " [LOCKED]";
      out << " — " << sm.getSkillDescription(i) << "\n";
    }
  }
}
