#include "core_stats.hpp"
#include <algorithm>

float hvostov::ComplexStat::getTotal() const
{
  return (base_ + flat_) * multiply_;
}

float hvostov::BaseStat::getBase() const
{
  return base_;
}

void hvostov::BaseStat::addBase(float value)
{
  base_ += value;
}

void hvostov::BaseStat::setBase(float value)
{
  base_ = value;
}

void hvostov::ComplexStat::addFlat(float value)
{
  flat_ += value;
}

void hvostov::ComplexStat::setFlat(float value)
{
  flat_ = value;
}

float hvostov::ComplexStat::getFlat() const
{
  return flat_;
}

void hvostov::ComplexStat::addMultiply(float value)
{
  multiply_ += value;
}

void hvostov::ComplexStat::setMultiply(float value)
{
  multiply_ = value;
}

float hvostov::ComplexStat::getMultiply() const
{
  return multiply_;
}

hvostov::ComplexStat& hvostov::CoreStats::getHealth()
{
  return health_;
}

const hvostov::ComplexStat& hvostov::CoreStats::getHealth() const
{
  return health_;
}

hvostov::ComplexStat& hvostov::CoreStats::getDefense()
{
  return defense_;
}

const hvostov::ComplexStat& hvostov::CoreStats::getDefense() const
{
  return defense_;
}

hvostov::ComplexStat& hvostov::CoreStats::getDamage()
{
  return damage_;
}

const hvostov::ComplexStat& hvostov::CoreStats::getDamage() const
{
  return damage_;
}

hvostov::ComplexStat& hvostov::CoreStats::getSpeed()
{
  return speed_;
}

const hvostov::ComplexStat& hvostov::CoreStats::getSpeed() const
{
  return speed_;
}

hvostov::ComplexStat& hvostov::CoreStats::getResource()
{
  return resource_;
}

const hvostov::ComplexStat& hvostov::CoreStats::getResource() const
{
  return resource_;
}

hvostov::BaseStat& hvostov::CoreStats::getDamageBonus()
{
  return damage_bonus_;
}

const hvostov::BaseStat& hvostov::CoreStats::getDamageBonus() const
{
  return damage_bonus_;
}

hvostov::BaseStat& hvostov::CoreStats::getCritDamage()
{
  return crit_damage_;
}

const hvostov::BaseStat& hvostov::CoreStats::getCritDamage() const
{
  return crit_damage_;
}

hvostov::BaseStat& hvostov::CoreStats::getCritChance()
{
  return crit_chance_;
}

const hvostov::BaseStat& hvostov::CoreStats::getCritChance() const
{
  return crit_chance_;
}

hvostov::BaseStat& hvostov::CoreStats::getDamageReduction()
{
  return damage_reduction_;
}

const hvostov::BaseStat& hvostov::CoreStats::getDamageReduction() const
{
  return damage_reduction_;
}

float hvostov::CoreStats::calculatePhysicalResistance() const
{
  float defense = defense_.getTotal();
  return defense / (defense + 100.0f);
}

float& hvostov::CoreStats::getCurrentHealth()
{
  return current_health_;
}

const float& hvostov::CoreStats::getCurrentHealth() const
{
  return current_health_;
}

float& hvostov::CoreStats::getCurrentResource()
{
  return current_resource_;
}

const float& hvostov::CoreStats::getCurrentResource() const
{
  return current_resource_;
}

float hvostov::CoreStats::getShield() const
{
  return shield_;
}

void hvostov::CoreStats::setShield(float value)
{
  shield_ = std::max(0.0f, value);
}

void hvostov::CoreStats::addShield(float value)
{
  shield_ += value;
}

void hvostov::CoreStats::removeShield(float value)
{
  shield_ = std::max(0.0f, shield_ - value);
}

bool hvostov::CoreStats::isInvincible() const
{
  return invincible_;
}

void hvostov::CoreStats::setInvincible(bool value)
{
  invincible_ = value;
}

bool hvostov::CoreStats::isStunned() const
{
  return stunned_;
}

void hvostov::CoreStats::setStunned(bool value)
{
  stunned_ = value;
}

bool hvostov::CoreStats::getIsDead() const
{
  return is_dead_;
}

void hvostov::CoreStats::setIsDead(bool is_dead)
{
  is_dead_ = is_dead;
}

float hvostov::CoreStats::getResourceRegen() const
{
  return resource_regen_;
}

void hvostov::CoreStats::setResourceRegen(float value)
{
  resource_regen_ = value;
}
