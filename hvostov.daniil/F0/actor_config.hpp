#ifndef ACTOR_CONFIG_HPP
#define ACTOR_CONFIG_HPP

#include <cstddef>
#include "types.hpp"

namespace hvostov {

  struct ActorConfig {
    float health = 100.0f;
    float resource = 100.0f;
    float resource_regen = 10.0f;
    float damage = 10.0f;
    float defense = 5.0f;
    float speed = 100.0f;
    float crit_chance = 0.05f;
    float crit_damage = 1.5f;
    float damage_bonus = 1.0f;
    float damage_reduction = 0.0f;
    ActorType actor_type = ActorType::Hero;
    HeroClass hero_class = HeroClass::NoClass;

    ActorConfig() = default;
  };

  namespace Presets {

    ActorConfig Knight();
    ActorConfig Mage();
    ActorConfig Assassin();
    ActorConfig Paladin();
    ActorConfig Saint();
    ActorConfig Priest();
    ActorConfig Archer();
    ActorConfig Samurai();
    ActorConfig Engineer();
    ActorConfig Hunter();

    ActorConfig Goblin(size_t stage = 1);
    ActorConfig Wolf(size_t stage = 1);
    ActorConfig Skeleton(size_t stage = 1);
    ActorConfig Slime(size_t stage = 1);
    ActorConfig Bat(size_t stage = 1);
    ActorConfig Spider(size_t stage = 1);
    ActorConfig Zombie(size_t stage = 1);
    ActorConfig Cultist(size_t stage = 1);
    ActorConfig Bandit(size_t stage = 1);
    ActorConfig Imp(size_t stage = 1);
    ActorConfig Harpy(size_t stage = 1);
    ActorConfig Mummy(size_t stage = 1);

    ActorConfig OrcWarrior(size_t stage = 1);
    ActorConfig DarkElf(size_t stage = 1);
    ActorConfig StoneGolem(size_t stage = 1);
    ActorConfig FireElemental(size_t stage = 1);
    ActorConfig IceElemental(size_t stage = 1);
    ActorConfig Thunderbird(size_t stage = 1);
    ActorConfig ShadowAssassin(size_t stage = 1);
    ActorConfig AncientTreant(size_t stage = 1);

    ActorConfig TrollKing(size_t stage = 1);
    ActorConfig Dragon(size_t stage = 1);
    ActorConfig Lich(size_t stage = 1);

  }

}

#endif
