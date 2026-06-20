#ifndef ENEMY_FACTORY_HPP
#define ENEMY_FACTORY_HPP

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <robin_hood_hash_table.hpp>
#include "enemy.hpp"

namespace hvostov {

  inline const RobinHoodHashTable< std::string, std::function< std::unique_ptr< Enemy >(size_t) > >&
  getEnemyFactoryMap()
  {
    static const RobinHoodHashTable< std::string, std::function< std::unique_ptr< Enemy >(size_t) > > map = {
        {"Goblin", [](size_t lvl) { return std::make_unique< Goblin >(lvl); }},
        {"Wolf", [](size_t lvl) { return std::make_unique< Wolf >(lvl); }},
        {"Skeleton", [](size_t lvl) { return std::make_unique< Skeleton >(lvl); }},
        {"Slime", [](size_t lvl) { return std::make_unique< Slime >(lvl); }},
        {"Bat", [](size_t lvl) { return std::make_unique< Bat >(lvl); }},
        {"Spider", [](size_t lvl) { return std::make_unique< Spider >(lvl); }},
        {"Zombie", [](size_t lvl) { return std::make_unique< Zombie >(lvl); }},
        {"Cultist", [](size_t lvl) { return std::make_unique< Cultist >(lvl); }},
        {"Bandit", [](size_t lvl) { return std::make_unique< Bandit >(lvl); }},
        {"Imp", [](size_t lvl) { return std::make_unique< Imp >(lvl); }},
        {"Harpy", [](size_t lvl) { return std::make_unique< Harpy >(lvl); }},
        {"Mummy", [](size_t lvl) { return std::make_unique< Mummy >(lvl); }},
        {"Orc Warrior", [](size_t lvl) { return std::make_unique< OrcWarrior >(lvl); }},
        {"Dark Elf", [](size_t lvl) { return std::make_unique< DarkElf >(lvl); }},
        {"Stone Golem", [](size_t lvl) { return std::make_unique< StoneGolem >(lvl); }},
        {"Fire Elemental", [](size_t lvl) { return std::make_unique< FireElemental >(lvl); }},
        {"Ice Elemental", [](size_t lvl) { return std::make_unique< IceElemental >(lvl); }},
        {"Thunderbird", [](size_t lvl) { return std::make_unique< Thunderbird >(lvl); }},
        {"Shadow Assassin", [](size_t lvl) { return std::make_unique< ShadowAssassin >(lvl); }},
        {"Ancient Treant", [](size_t lvl) { return std::make_unique< AncientTreant >(lvl); }},
        {"Troll King", [](size_t lvl) { return std::make_unique< TrollKing >(lvl); }},
        {"Dragon", [](size_t lvl) { return std::make_unique< Dragon >(lvl); }},
        {"Lich", [](size_t lvl) { return std::make_unique< Lich >(lvl); }}};
    return map;
  }

  inline std::unique_ptr< Enemy > createEnemyByType(const std::string& type, size_t level)
  {
    const auto& map = getEnemyFactoryMap();
    auto it = map.find(type);
    return (it != map.end()) ? it->second(level) : nullptr;
  }

  inline std::vector< std::string > getAvailableEnemyTypes()
  {
    std::vector< std::string > types;
    for (const auto& pair : getEnemyFactoryMap()) {
      types.push_back(pair.first);
    }
    return types;
  }

  inline bool isValidEnemyType(const std::string& type)
  {
    return getEnemyFactoryMap().find(type) != getEnemyFactoryMap().end();
  }

}

#endif
