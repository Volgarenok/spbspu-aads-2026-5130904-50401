#include "hero_account.hpp"
#include <iostream>
#include "hero_factory.hpp"
#include "dungeon.hpp"

void hvostov::UnitSaveData::applyTo(Hero& hero) const
{
  auto& stats = hero.getStats();

  stats.getHealth().setBase(base_health);
  stats.getHealth().addFlat(flat_health);
  stats.getHealth().setMultiply(health_multiply);

  stats.getDamage().setBase(base_damage);
  stats.getDamage().addFlat(flat_damage);
  stats.getDamage().setMultiply(damage_multiply);

  stats.getDefense().setBase(base_defense);
  stats.getDefense().addFlat(flat_defense);
  stats.getDefense().setMultiply(defense_multiply);

  stats.getSpeed().setBase(base_speed);
  stats.getSpeed().addFlat(flat_speed);
  stats.getSpeed().setMultiply(speed_multiply);

  stats.getResource().setBase(base_resource);
  stats.getResource().addFlat(flat_resource);
  stats.getResource().setMultiply(resource_multiply);

  stats.getCritChance().setBase(crit_chance);
  stats.getCritDamage().setBase(crit_damage);
  stats.getDamageBonus().setBase(damage_bonus);
  stats.getDamageReduction().setBase(damage_reduction);

  stats.getCurrentHealth() = current_health;
  stats.getCurrentResource() = current_resource;
}

void hvostov::UnitSaveData::loadFrom(const Hero& hero)
{
  const auto& stats = hero.getStats();

  hero_class = hero.getHeroClass();
  current_health = stats.getCurrentHealth();
  current_resource = stats.getCurrentResource();

  base_health = stats.getHealth().getBase();
  flat_health = stats.getHealth().flat_;
  health_multiply = stats.getHealth().multiply_;

  base_damage = stats.getDamage().getBase();
  flat_damage = stats.getDamage().flat_;
  damage_multiply = stats.getDamage().multiply_;

  base_defense = stats.getDefense().getBase();
  flat_defense = stats.getDefense().flat_;
  defense_multiply = stats.getDefense().multiply_;

  base_speed = stats.getSpeed().getBase();
  flat_speed = stats.getSpeed().flat_;
  speed_multiply = stats.getSpeed().multiply_;

  base_resource = stats.getResource().getBase();
  flat_resource = stats.getResource().flat_;
  resource_multiply = stats.getResource().multiply_;

  crit_chance = stats.getCritChance().getBase();
  crit_damage = stats.getCritDamage().getBase();
  damage_bonus = stats.getDamageBonus().getBase();
  damage_reduction = stats.getDamageReduction().getBase();
}

void hvostov::HeroAccount::applyToHeroes()
{
  for (size_t i = 0; i < party.size() && i < party_ptrs.size(); i++) {
    if (party_ptrs[i]) {
      party[i].applyTo(*party_ptrs[i]);
    }
  }
}

void hvostov::HeroAccount::loadFromHeroes()
{
  for (size_t i = 0; i < party.size() && i < party_ptrs.size(); i++) {
    if (party_ptrs[i]) {
      party[i].loadFrom(*party_ptrs[i]);
    }
  }
}

hvostov::HeroAccount::HeroAccount(const std::string& name):
  account_name(name)
{}

hvostov::DungeonProgress* hvostov::HeroAccount::findDungeonStats(const std::string& name)
{
  for (auto& stat : dungeon_stats) {
    if (stat.dungeon_name == name)
      return &stat;
  }
  return nullptr;
}

void hvostov::HeroAccount::chooseDungeon(const std::string& dungeon_name)
{
  Dungeon temp;
  if (!temp.load(dungeon_name)) {
    std::cout << "<INVALID COMMAND> — dungeon not found\n";
    return;
  }

  if (dungeon_started) {
    std::cout << "Already in dungeon: " << active_dungeon << "\n";
    std::cout << "Use 'abandon-dungeon' first\n";
    return;
  }

  active_dungeon = dungeon_name;
  dungeon_started = false;
  std::cout << "DUNGEON CHOSEN: " << dungeon_name << "\n";
  std::cout << "Type 'join-world' to start\n";
}

void hvostov::HeroAccount::startDungeon()
{
  if (active_dungeon.empty()) {
    std::cout << "No dungeon chosen.\n";
    return;
  }

  if (dungeon_started) {
    std::cout << "CONTINUING DUNGEON: " << active_dungeon << "\n";
    std::cout << "Current floor: " << current_dungeon_floor << "\n";
    return;
  }

  resetHeroes();

  auto* stats = findDungeonStats(active_dungeon);
  if (!stats) {
    DungeonProgress new_stat;
    new_stat.dungeon_name = active_dungeon;
    new_stat.attempts = 1;
    dungeon_stats.push_back(new_stat);
  } else {
    stats->attempts++;
  }

  current_dungeon_floor = 1;
  dungeon_started = true;

  std::cout << "DUNGEON STARTED: " << active_dungeon << "\n";
}

void hvostov::HeroAccount::completeDungeon()
{
  auto* stats = findDungeonStats(active_dungeon);
  if (stats) {
    stats->completions++;
  }

  std::cout << "DUNGEON COMPLETED: " << active_dungeon << "\n";

  active_dungeon = "";
  current_dungeon_floor = 1;
  dungeon_started = false;
}

void hvostov::HeroAccount::failDungeon()
{
  std::cout << "DUNGEON FAILED — heroes reset\n";

  active_dungeon = "";
  current_dungeon_floor = 1;
  dungeon_started = false;

  resetHeroes();
}

void hvostov::HeroAccount::abandonDungeon()
{
  if (active_dungeon.empty()) {
    std::cout << "No dungeon chosen or active\n";
    return;
  }

  if (dungeon_started) {
    std::cout << "ABANDONING ACTIVE DUNGEON: " << active_dungeon << "\n";
    loadFromHeroes();
    failDungeon();
  } else {
    std::cout << "ABANDONING CHOSEN DUNGEON: " << active_dungeon << "\n";
    active_dungeon = "";
    dungeon_started = false;
  }
}

void hvostov::HeroAccount::resetHeroes()
{
  party_ptrs.clear();
  for (auto& data : party) {
    auto hero = createHeroByClass(data.hero_class);
    party_ptrs.push_back(std::move(hero));
  }
}
