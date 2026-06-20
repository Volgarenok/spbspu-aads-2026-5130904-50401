#include "damage_manager.hpp"
#include "actor.hpp"
#include "core_stats.hpp"
#include "effects.hpp"
#include "random.hpp"

hvostov::DamageManager::DamageManager(hvostov::Actor* owner):
  owner_(owner)
{}

bool hvostov::DamageManager::handleAttack(float attack, hvostov::Actor* attacker)
{
  float damage = calculateInputDamage(attack);
  auto effect = owner_->getEffectManager().getEffect(hvostov::EffectType::Parry);
  if (effect) {
    hvostov::ParryEffect* parry = dynamic_cast< hvostov::ParryEffect* >(effect);
    if (parry && hvostov::Random::getFloat(0.0f, 1.0f) < parry->getParryChance()) {
      if (owner_ != attacker) {
        owner_->getSkillManager().useSkill(0, attacker);
      }
    }
  }
  takeDamage(damage);
  return true;
}

float hvostov::DamageManager::calculateInputDamage(float attack)
{
  float damage = attack;
  float phys_res = owner_->getStats().calculatePhysicalResistance();
  float damage_reduction = owner_->getStats().getDamageReduction().getBase();
  damage *= 1.0f - phys_res;
  damage *= 1.0f - damage_reduction;
  if (damage < 0) {
    damage *= (-1);
  }
  return damage;
}

float hvostov::DamageManager::calculateOutputDamage(hvostov::AttackSkill* skill)
{
  float damage = 0.0f;
  switch (skill->getScaleType()) {
    case hvostov::ScaleType::Damage:
      damage = owner_->getStats().getDamage().getTotal();
      break;
    case hvostov::ScaleType::Health:
      damage = owner_->getStats().getHealth().getTotal();
      break;
    case hvostov::ScaleType::Defense:
      damage = owner_->getStats().getDefense().getTotal();
      break;
    case hvostov::ScaleType::Resource:
      damage = owner_->getStats().getResource().getTotal();
      break;
  }
  damage *= skill->getDamageMultiplier();
  damage += skill->getFlatDamage();
  if (hvostov::Random::getFloat(0.0f, 1.0f) < owner_->getStats().getCritChance().getBase()) {
    damage *= owner_->getStats().getCritDamage().getBase();
  }
  float damage_bonus = owner_->getStats().getDamageBonus().getBase();
  if (damage_bonus < 0) {
    return 0.0f;
  }
  damage *= damage_bonus;
  return damage;
}

void hvostov::DamageManager::takeDamage(float damage)
{
  if (damage <= 0)
    return;
  auto& stats = owner_->getStats();
  if (stats.isInvincible()) {
    return;
  }
  if (stats.getShield() > 0) {
    float shield_absorb = std::min(damage, stats.getShield());
    stats.setShield(stats.getShield() - shield_absorb);
    damage -= shield_absorb;
  }
  if (damage <= 0) {
    return;
  }
  damage *= (1 - stats.getDamageReduction().getBase());
  if (stats.getCurrentHealth() - damage <= 0) {
    stats.getCurrentHealth() = 0;
    owner_->die();
  } else {
    stats.getCurrentHealth() -= damage;
  }
}

void hvostov::DamageManager::heal(float amount)
{
  if (amount <= 0)
    return;

  auto& stats = owner_->getStats();
  float max_health = stats.getHealth().getTotal();
  float new_health = stats.getCurrentHealth() + amount;
  stats.getCurrentHealth() = std::min(max_health, new_health);
}

void hvostov::DamageManager::revive(float percent)
{
  auto& stats = owner_->getStats();
  float max_health = stats.getHealth().getTotal();
  stats.getCurrentHealth() = max_health * percent;
  stats.setInvincible(false);
  stats.setStunned(false);
}
