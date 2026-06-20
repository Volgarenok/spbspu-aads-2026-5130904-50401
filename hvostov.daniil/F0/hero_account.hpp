#ifndef HERO_ACCOUNT_HPP
#define HERO_ACCOUNT_HPP

#include <memory>
#include <string>
#include <vector>
#include "hero.hpp"
#include "types.hpp"

namespace hvostov {

  struct DungeonProgress {
    std::string dungeon_name;
    size_t attempts = 0;
    size_t completions = 0;
  };

  struct UnitSaveData {
    std::string name = "";
    HeroClass hero_class;
    float current_health = 100.0f;
    float current_resource = 50.0f;

    float base_health = 100.0f;
    float flat_health = 0.0f;
    float health_multiply = 1.0f;

    float base_damage = 20.0f;
    float flat_damage = 0.0f;
    float damage_multiply = 1.0f;

    float base_defense = 10.0f;
    float flat_defense = 0.0f;
    float defense_multiply = 1.0f;

    float base_speed = 100.0f;
    float flat_speed = 0.0f;
    float speed_multiply = 1.0f;

    float base_resource = 100.0f;
    float flat_resource = 0.0f;
    float resource_multiply = 1.0f;

    float crit_chance = 0.05f;
    float crit_damage = 1.5f;
    float damage_bonus = 0.0f;
    float damage_reduction = 0.0f;

    void applyTo(Hero& hero) const;
    void loadFrom(const Hero& hero);
  };

  struct HeroAccount {
    std::string account_name;
    size_t current_stage = 1;
    HeroAccount() = default;
    HeroAccount(const std::string& name);
    HeroAccount(HeroAccount&&) = default;
    HeroAccount& operator=(HeroAccount&&) = default;

    std::vector< UnitSaveData > party;
    std::vector< std::unique_ptr< Hero > > party_ptrs;

    void applyToHeroes();
    void loadFromHeroes();

    std::vector< DungeonProgress > dungeon_stats;
    std::string active_dungeon;
    size_t current_dungeon_floor = 1;
    bool dungeon_started = false;

    DungeonProgress* findDungeonStats(const std::string& name);
    void chooseDungeon(const std::string& dungeon_name);
    void startDungeon();
    void completeDungeon();
    void failDungeon();
    void abandonDungeon();
    void resetHeroes();

    bool hasDungeon() const
    {
      return !active_dungeon.empty();
    }

    bool isDungeonStarted() const
    {
      return dungeon_started;
    }
  };

}

#endif
