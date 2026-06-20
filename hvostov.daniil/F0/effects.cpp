#include "effects.hpp"
#include <memory>
#include "actor.hpp"

hvostov::Effect::Effect(hvostov::EffectType type, size_t duration):
  type_(type),
  duration_(duration)
{}

void hvostov::Effect::onApply(hvostov::Actor* owner)
{
  doOnApply(owner);
}

void hvostov::Effect::onTick(hvostov::Actor* owner)
{
  doOnTick(owner);
  reduceDuration();
}

void hvostov::Effect::onRemove(hvostov::Actor* owner)
{
  doOnRemove(owner);
}

bool hvostov::Effect::isExpired() const
{
  return duration_ == 0;
}

void hvostov::Effect::reduceDuration()
{
  if (duration_ > 0)
    duration_--;
}

hvostov::EffectType hvostov::Effect::getEffectType() const
{
  return type_;
}

float hvostov::Effect::getDuration() const
{
  return static_cast< float >(duration_);
}

void hvostov::Effect::setDuration(size_t value)
{
  duration_ = value;
}

void hvostov::Effect::doOnStack(hvostov::Effect*)
{}

hvostov::NailingEffect::NailingEffect(size_t duration, float speed_reduction):
  hvostov::Effect(hvostov::EffectType::Nailing, duration),
  speed_reduction_(speed_reduction)
{}

hvostov::NailingEffect::NailingEffect():
  hvostov::NailingEffect(3, 20.0f)
{}

bool hvostov::NailingEffect::isStackable() const
{
  return true;
}

void hvostov::NailingEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::NailingEffect::clone() const
{
  return std::make_unique< hvostov::NailingEffect >(*this);
}

void hvostov::NailingEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().getSpeed().addBase(-speed_reduction_);
}

void hvostov::NailingEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::NailingEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().getSpeed().addBase(speed_reduction_);
}

hvostov::BleedingEffect::BleedingEffect(size_t duration, float percentage_damage, float flat_damage):
  hvostov::Effect(hvostov::EffectType::Bleeding, duration),
  percentage_damage_per_move_(percentage_damage),
  flat_damage_per_move_(flat_damage)
{}

hvostov::BleedingEffect::BleedingEffect():
  hvostov::BleedingEffect(3, 0.03f, 10.0f)
{}

bool hvostov::BleedingEffect::isStackable() const
{
  return true;
}

void hvostov::BleedingEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::BleedingEffect::clone() const
{
  return std::make_unique< hvostov::BleedingEffect >(*this);
}

void hvostov::BleedingEffect::doOnApply(hvostov::Actor*)
{}

void hvostov::BleedingEffect::doOnTick(hvostov::Actor* owner)
{
  owner->getDamageManager().takeDamage(flat_damage_per_move_);
  owner->getDamageManager().takeDamage(owner->getStats().getHealth().getTotal() * percentage_damage_per_move_);
}

void hvostov::BleedingEffect::doOnRemove(hvostov::Actor*)
{}

hvostov::ParryEffect::ParryEffect(size_t duration, float parry_chance):
  hvostov::Effect(hvostov::EffectType::Parry, duration),
  parry_chance_(parry_chance)
{}

hvostov::ParryEffect::ParryEffect():
  hvostov::ParryEffect(3, 0.5f)
{}

bool hvostov::ParryEffect::isStackable() const
{
  return false;
}

void hvostov::ParryEffect::doOnStack(hvostov::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< hvostov::Effect > hvostov::ParryEffect::clone() const
{
  return std::make_unique< hvostov::ParryEffect >(*this);
}

void hvostov::ParryEffect::doOnApply(hvostov::Actor*)
{}

void hvostov::ParryEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::ParryEffect::doOnRemove(hvostov::Actor*)
{}

float hvostov::ParryEffect::getParryChance() const
{
  return parry_chance_;
}

hvostov::DamageBuffEffect::DamageBuffEffect(size_t duration, float buff):
  hvostov::Effect(hvostov::EffectType::DamageBuff, duration),
  damage_buff_(buff)
{}

hvostov::DamageBuffEffect::DamageBuffEffect():
  hvostov::DamageBuffEffect(3, 0.2f)
{}

bool hvostov::DamageBuffEffect::isStackable() const
{
  return true;
}

void hvostov::DamageBuffEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::DamageBuffEffect::clone() const
{
  return std::make_unique< hvostov::DamageBuffEffect >(*this);
}

void hvostov::DamageBuffEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().getDamage().addMultiply(damage_buff_);
}

void hvostov::DamageBuffEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::DamageBuffEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().getDamage().addMultiply(-damage_buff_);
}

float hvostov::DamageBuffEffect::getDamageBuff() const
{
  return damage_buff_;
}

hvostov::BurnEffect::BurnEffect(size_t duration, float damage_per_tick):
  hvostov::Effect(hvostov::EffectType::Burn, duration),
  damage_per_tick_(damage_per_tick)
{}

hvostov::BurnEffect::BurnEffect():
  hvostov::BurnEffect(3, 15.0f)
{}

bool hvostov::BurnEffect::isStackable() const
{
  return true;
}

void hvostov::BurnEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::BurnEffect::clone() const
{
  return std::make_unique< hvostov::BurnEffect >(*this);
}

void hvostov::BurnEffect::doOnApply(hvostov::Actor*)
{}

void hvostov::BurnEffect::doOnTick(hvostov::Actor* owner)
{
  owner->getDamageManager().takeDamage(damage_per_tick_);
}

void hvostov::BurnEffect::doOnRemove(hvostov::Actor*)
{}

hvostov::SlowEffect::SlowEffect(size_t duration, float speed_multiplier):
  hvostov::Effect(hvostov::EffectType::Slow, duration),
  speed_multiplier_(speed_multiplier)
{}

hvostov::SlowEffect::SlowEffect():
  hvostov::SlowEffect(3, 0.5f)
{}

bool hvostov::SlowEffect::isStackable() const
{
  return false;
}

void hvostov::SlowEffect::doOnStack(hvostov::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< hvostov::Effect > hvostov::SlowEffect::clone() const
{
  return std::make_unique< hvostov::SlowEffect >(*this);
}

void hvostov::SlowEffect::doOnApply(hvostov::Actor* owner)
{
  original_speed_ = owner->getStats().getSpeed().getTotal();
  owner->getStats().getSpeed().setBase(original_speed_ * speed_multiplier_);
}

void hvostov::SlowEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::SlowEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().getSpeed().setBase(original_speed_);
}

hvostov::StunEffect::StunEffect(size_t duration):
  hvostov::Effect(hvostov::EffectType::Stun, duration)
{}

hvostov::StunEffect::StunEffect():
  hvostov::StunEffect(1)
{}

bool hvostov::StunEffect::isStackable() const
{
  return false;
}

void hvostov::StunEffect::doOnStack(hvostov::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< hvostov::Effect > hvostov::StunEffect::clone() const
{
  return std::make_unique< hvostov::StunEffect >(*this);
}

void hvostov::StunEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().setStunned(true);
}

void hvostov::StunEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::StunEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().setStunned(false);
}

hvostov::TauntEffect::TauntEffect(size_t duration, Actor* taunter):
  Effect(EffectType::Taunt, duration),
  taunter_(taunter)
{}

hvostov::TauntEffect::TauntEffect():
  hvostov::TauntEffect(2)
{}

bool hvostov::TauntEffect::isStackable() const
{
  return false;
}

void hvostov::TauntEffect::doOnStack(hvostov::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< hvostov::Effect > hvostov::TauntEffect::clone() const
{
  return std::make_unique< hvostov::TauntEffect >(*this);
}

void hvostov::TauntEffect::doOnApply(hvostov::Actor*)
{}

void hvostov::TauntEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::TauntEffect::doOnRemove(hvostov::Actor*)
{}

hvostov::DefenseBuffEffect::DefenseBuffEffect(size_t duration, float multiplier):
  hvostov::Effect(hvostov::EffectType::DefenseBuff, duration),
  multiplier_(multiplier)
{}

hvostov::DefenseBuffEffect::DefenseBuffEffect():
  hvostov::DefenseBuffEffect(3, 1.3f)
{}

bool hvostov::DefenseBuffEffect::isStackable() const
{
  return true;
}

void hvostov::DefenseBuffEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::DefenseBuffEffect::clone() const
{
  return std::make_unique< hvostov::DefenseBuffEffect >(*this);
}

void hvostov::DefenseBuffEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().getDefense().addMultiply(multiplier_ - 1.0f);
}

void hvostov::DefenseBuffEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::DefenseBuffEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().getDefense().addMultiply(-(multiplier_ - 1.0f));
}

float hvostov::DefenseBuffEffect::getMultiplier() const
{
  return multiplier_;
}

hvostov::SpeedBuffEffect::SpeedBuffEffect(size_t duration, float multiplier):
  hvostov::Effect(hvostov::EffectType::SpeedBuff, duration),
  multiplier_(multiplier)
{}

hvostov::SpeedBuffEffect::SpeedBuffEffect():
  hvostov::SpeedBuffEffect(3, 1.3f)
{}

bool hvostov::SpeedBuffEffect::isStackable() const
{
  return true;
}

void hvostov::SpeedBuffEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::SpeedBuffEffect::clone() const
{
  return std::make_unique< hvostov::SpeedBuffEffect >(*this);
}

void hvostov::SpeedBuffEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().getSpeed().addMultiply(multiplier_ - 1.0f);
}

void hvostov::SpeedBuffEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::SpeedBuffEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().getSpeed().addMultiply(-(multiplier_ - 1.0f));
}

float hvostov::SpeedBuffEffect::getMultiplier() const
{
  return multiplier_;
}

hvostov::InvincibilityEffect::InvincibilityEffect(size_t duration):
  hvostov::Effect(hvostov::EffectType::Invincibility, duration)
{}

hvostov::InvincibilityEffect::InvincibilityEffect():
  hvostov::InvincibilityEffect(2)
{}

bool hvostov::InvincibilityEffect::isStackable() const
{
  return false;
}

void hvostov::InvincibilityEffect::doOnStack(hvostov::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< hvostov::Effect > hvostov::InvincibilityEffect::clone() const
{
  return std::make_unique< hvostov::InvincibilityEffect >(*this);
}

void hvostov::InvincibilityEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().setInvincible(true);
}

void hvostov::InvincibilityEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::InvincibilityEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().setInvincible(false);
}

hvostov::DamageAmplifyEffect::DamageAmplifyEffect(size_t duration, float multiplier):
  hvostov::Effect(hvostov::EffectType::DamageAmplify, duration),
  multiplier_(multiplier)
{}

hvostov::DamageAmplifyEffect::DamageAmplifyEffect():
  hvostov::DamageAmplifyEffect(3, 1.5f)
{}

bool hvostov::DamageAmplifyEffect::isStackable() const
{
  return true;
}

void hvostov::DamageAmplifyEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::DamageAmplifyEffect::clone() const
{
  return std::make_unique< hvostov::DamageAmplifyEffect >(*this);
}

void hvostov::DamageAmplifyEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().getDamageBonus().addBase(multiplier_);
}

void hvostov::DamageAmplifyEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::DamageAmplifyEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().getDamageBonus().addBase(-multiplier_);
}

float hvostov::DamageAmplifyEffect::getMultiplier() const
{
  return multiplier_;
}

hvostov::HealOverTimeEffect::HealOverTimeEffect(size_t duration, float percent_per_tick):
  hvostov::Effect(hvostov::EffectType::HealOverTime, duration),
  percent_per_tick_(percent_per_tick)
{}

hvostov::HealOverTimeEffect::HealOverTimeEffect():
  hvostov::HealOverTimeEffect(3, 0.1f)
{}

bool hvostov::HealOverTimeEffect::isStackable() const
{
  return true;
}

void hvostov::HealOverTimeEffect::doOnStack(hvostov::Effect* effect)
{
  hvostov::Effect::doOnStack(effect);
}

std::unique_ptr< hvostov::Effect > hvostov::HealOverTimeEffect::clone() const
{
  return std::make_unique< hvostov::HealOverTimeEffect >(*this);
}

void hvostov::HealOverTimeEffect::doOnApply(hvostov::Actor*)
{}

void hvostov::HealOverTimeEffect::doOnTick(hvostov::Actor* owner)
{
  owner->getDamageManager().heal(owner->getStats().getHealth().getTotal() * percent_per_tick_);
}

void hvostov::HealOverTimeEffect::doOnRemove(hvostov::Actor*)
{}

hvostov::ShieldEffect::ShieldEffect(size_t duration, float shield_amount):
  hvostov::Effect(hvostov::EffectType::Shield, duration),
  shield_amount_(shield_amount)
{}

hvostov::ShieldEffect::ShieldEffect():
  hvostov::ShieldEffect(3, 100.0f)
{}

bool hvostov::ShieldEffect::isStackable() const
{
  return false;
}

void hvostov::ShieldEffect::doOnStack(hvostov::Effect* effect)
{
  setDuration(std::max(getDuration(), effect->getDuration()));
}

std::unique_ptr< hvostov::Effect > hvostov::ShieldEffect::clone() const
{
  return std::make_unique< hvostov::ShieldEffect >(*this);
}

void hvostov::ShieldEffect::doOnApply(hvostov::Actor* owner)
{
  owner->getStats().addShield(shield_amount_);
}

void hvostov::ShieldEffect::doOnTick(hvostov::Actor*)
{}

void hvostov::ShieldEffect::doOnRemove(hvostov::Actor* owner)
{
  owner->getStats().removeShield(shield_amount_);
}

bool hvostov::NailingEffect::isHarmful() const
{
  return true;
}

bool hvostov::BleedingEffect::isHarmful() const
{
  return true;
}

bool hvostov::BurnEffect::isHarmful() const
{
  return true;
}

bool hvostov::SlowEffect::isHarmful() const
{
  return true;
}

bool hvostov::StunEffect::isHarmful() const
{
  return true;
}

bool hvostov::TauntEffect::isHarmful() const
{
  return true;
}

bool hvostov::DamageAmplifyEffect::isHarmful() const
{
  return true;
}

bool hvostov::ParryEffect::isHarmful() const
{
  return false;
}

bool hvostov::DamageBuffEffect::isHarmful() const
{
  return false;
}

bool hvostov::DefenseBuffEffect::isHarmful() const
{
  return false;
}

bool hvostov::SpeedBuffEffect::isHarmful() const
{
  return false;
}

bool hvostov::ShieldEffect::isHarmful() const
{
  return false;
}

bool hvostov::HealOverTimeEffect::isHarmful() const
{
  return false;
}

bool hvostov::InvincibilityEffect::isHarmful() const
{
  return false;
}

hvostov::CritChanceBuffEffect::CritChanceBuffEffect(size_t duration, float amount):
  Effect(EffectType::DamageBuff, duration),
  amount_(amount)
{}

hvostov::CritChanceBuffEffect::CritChanceBuffEffect():
  CritChanceBuffEffect(3, 0.15f)
{}

std::unique_ptr< hvostov::Effect > hvostov::CritChanceBuffEffect::clone() const
{
  return std::make_unique< CritChanceBuffEffect >(*this);
}

void hvostov::CritChanceBuffEffect::doOnApply(Actor* owner)
{
  owner->getStats().getCritChance().addBase(amount_);
}

void hvostov::CritChanceBuffEffect::doOnRemove(Actor* owner)
{
  owner->getStats().getCritChance().addBase(-amount_);
}
