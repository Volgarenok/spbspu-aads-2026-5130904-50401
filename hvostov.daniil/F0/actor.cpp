#include "actor.hpp"
#include "damage_manager.hpp"
#include "effect_manager.hpp"
#include "skill_manager.hpp"
#include "types.hpp"

hvostov::CoreStats& hvostov::Actor::getStats()
{
  return stats_;
}

hvostov::EffectManager& hvostov::Actor::getEffectManager()
{
  return effect_manager_;
}

hvostov::SkillManager& hvostov::Actor::getSkillManager()
{
  return skill_manager_;
}

const hvostov::CoreStats& hvostov::Actor::getStats() const
{
  return stats_;
}

const hvostov::EffectManager& hvostov::Actor::getEffectManager() const
{
  return effect_manager_;
}

const hvostov::SkillManager& hvostov::Actor::getSkillManager() const
{
  return skill_manager_;
}

hvostov::DamageManager& hvostov::Actor::getDamageManager()
{
  return damage_manager_;
}

const hvostov::DamageManager& hvostov::Actor::getDamageManager() const
{
  return damage_manager_;
}

hvostov::ActorType hvostov::Actor::getActorType() const
{
  return actor_type_;
}

hvostov::Actor::Actor(const ActorConfig& config):
  stats_(),
  effect_manager_(this),
  skill_manager_(this),
  damage_manager_(this),
  actor_type_(config.actor_type)
{
  getStats().getHealth().setBase(config.health);
  getStats().getResource().setBase(config.resource);
  getStats().getDefense().setBase(config.defense);
  getStats().getSpeed().setBase(config.speed);
  getStats().getDamage().setBase(config.damage);
  getStats().getCritChance().setBase(config.crit_chance);
  getStats().getCritDamage().setBase(config.crit_damage);
  getStats().getDamageBonus().setBase(config.damage_bonus);
  getStats().getDamageReduction().setBase(config.damage_reduction);
  getStats().getCurrentHealth() = config.health;
  getStats().setResourceRegen(config.resource_regen);
}
