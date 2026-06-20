#ifndef HERO_FACTORY_HPP
#define HERO_FACTORY_HPP

#include <memory>
#include <string>
#include <cctype>
#include <robin_hood_hash_table.hpp>
#include "hero.hpp"
#include "types.hpp"

namespace hvostov {

  inline const RobinHoodHashTable< std::string, HeroClass >& getHeroClassMap()
  {
    static const RobinHoodHashTable< std::string, HeroClass > map = {
        {"knight", HeroClass::Knight},     {"mage", HeroClass::Mage},     {"assasin", HeroClass::Assasin},
        {"palladin", HeroClass::Palladin}, {"priest", HeroClass::Priest}, {"archer", HeroClass::Archer}};
    return map;
  }

  inline const RobinHoodHashTable< HeroClass, std::string >& getHeroClassNameMap()
  {
    static const RobinHoodHashTable< HeroClass, std::string > map = {
        {HeroClass::NoClass, "NoClass"}, {HeroClass::Knight, "Knight"},     {HeroClass::Mage, "Mage"},
        {HeroClass::Assasin, "Assasin"}, {HeroClass::Palladin, "Palladin"}, {HeroClass::Priest, "Priest"},
        {HeroClass::Archer, "Archer"}};
    return map;
  }

  inline std::unique_ptr< Hero > createHeroByClass(HeroClass heroClass)
  {
    switch (heroClass) {
      case HeroClass::Knight:
        return std::make_unique< Knight >();
      case HeroClass::Mage:
        return std::make_unique< Mage >();
      case HeroClass::Assasin:
        return std::make_unique< Assassin >();
      case HeroClass::Palladin:
        return std::make_unique< Paladin >();
      case HeroClass::Priest:
        return std::make_unique< Priest >();
      case HeroClass::Archer:
        return std::make_unique< Archer >();
      default:
        return std::make_unique< Knight >();
    }
  }

  inline HeroClass getHeroClassByString(std::string str)
  {
    for (auto& c : str)
      c = std::tolower(c);
    const auto& map = getHeroClassMap();
    auto it = map.find(str);
    return (it != map.end()) ? it->second : HeroClass::NoClass;
  }

  inline std::string getClassNameByHeroClass(HeroClass hero_class)
  {
    const auto& map = getHeroClassNameMap();
    auto it = map.find(hero_class);
    return (it != map.end()) ? it->second : "Unknown";
  }

}

#endif
