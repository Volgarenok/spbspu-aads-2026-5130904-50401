#include "reward.hpp"
#include <memory>
#include <string>
#include "random.hpp"

hvostov::Reward::Reward(const std::string& description):
  description_(description)
{}

std::string hvostov::Reward::getDescription() const
{
  return description_;
}

hvostov::HealthReward::HealthReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount)) + " HP"),
  amount_(amount)
{}

void hvostov::HealthReward::apply(Hero& hero) const
{
  hero.getStats().getHealth().addFlat(amount_);
  hero.getStats().getCurrentHealth() += amount_;
}

hvostov::ResourceReward::ResourceReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount)) + " MP"),
  amount_(amount)
{}

void hvostov::ResourceReward::apply(Hero& hero) const
{
  hero.getStats().getResource().addFlat(amount_);
  hero.getStats().getCurrentResource() += amount_;
}

hvostov::DamageReward::DamageReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount)) + " Damage"),
  amount_(amount)
{}

void hvostov::DamageReward::apply(Hero& hero) const
{
  hero.getStats().getDamage().addFlat(amount_);
}

hvostov::DefenseReward::DefenseReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount)) + " Defense"),
  amount_(amount)
{}

void hvostov::DefenseReward::apply(Hero& hero) const
{
  hero.getStats().getDefense().addFlat(amount_);
}

hvostov::SpeedReward::SpeedReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount)) + " Speed"),
  amount_(amount)
{}

void hvostov::SpeedReward::apply(Hero& hero) const
{
  hero.getStats().getSpeed().addBase(amount_);
}

hvostov::CritChanceReward::CritChanceReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount * 100)) + "% Crit Chance"),
  amount_(amount)
{}

void hvostov::CritChanceReward::apply(Hero& hero) const
{
  hero.getStats().getCritChance().addBase(amount_);
}

hvostov::CritDamageReward::CritDamageReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount * 100)) + "% Crit Damage"),
  amount_(amount)
{}

void hvostov::CritDamageReward::apply(Hero& hero) const
{
  hero.getStats().getCritDamage().addBase(amount_);
}

hvostov::SkillLevelReward::SkillLevelReward(size_t skill_index, size_t levels):
  Reward("Upgrade " + std::to_string(levels) + " lvl"),
  skill_index_(skill_index),
  levels_(levels)
{}

void hvostov::SkillLevelReward::apply(Hero& hero) const
{
  if (skill_index_ < hero.getSkillManager().getSkillCount() && !hero.getSkillManager().isSkillLocked(skill_index_))
    hero.getSkillManager().addLevelsToSkill(skill_index_, levels_);
}

hvostov::PercentHealthReward::PercentHealthReward(float percent):
  Reward("+" + std::to_string(static_cast< int >(percent)) + "% Max HP"),
  percent_(percent)
{}

void hvostov::PercentHealthReward::apply(Hero& hero) const
{
  float old_max = hero.getStats().getHealth().getTotal();
  hero.getStats().getHealth().addMultiply(percent_ / 100.0f);
  float new_max = hero.getStats().getHealth().getTotal();
  float increase = new_max - old_max;
  hero.getStats().getCurrentHealth() += increase;
  if (hero.getStats().getCurrentHealth() > new_max)
    hero.getStats().getCurrentHealth() = new_max;
}

hvostov::PercentDamageReward::PercentDamageReward(float percent):
  Reward("+" + std::to_string(static_cast< int >(percent)) + "% Damage"),
  percent_(percent)
{}

void hvostov::PercentDamageReward::apply(Hero& hero) const
{
  hero.getStats().getDamage().addMultiply(percent_ / 100.0f);
}

hvostov::PercentDefenseReward::PercentDefenseReward(float percent):
  Reward("+" + std::to_string(static_cast< int >(percent)) + "% Defense"),
  percent_(percent)
{}

void hvostov::PercentDefenseReward::apply(Hero& hero) const
{
  hero.getStats().getDefense().addMultiply(percent_ / 100.0f);
}

hvostov::PercentSpeedReward::PercentSpeedReward(float percent):
  Reward("+" + std::to_string(static_cast< int >(percent)) + "% Speed"),
  percent_(percent)
{}

void hvostov::PercentSpeedReward::apply(Hero& hero) const
{
  hero.getStats().getSpeed().addMultiply(percent_ / 100.0f);
}

hvostov::PercentResourceReward::PercentResourceReward(float percent):
  Reward("+" + std::to_string(static_cast< int >(percent)) + "% Max MP"),
  percent_(percent)
{}

void hvostov::PercentResourceReward::apply(Hero& hero) const
{
  float old = hero.getStats().getResource().getTotal();
  hero.getStats().getResource().addMultiply(percent_ / 100.0f);
  float inc = hero.getStats().getResource().getTotal() - old;
  hero.getStats().getCurrentResource() += inc;
}

hvostov::RegenReward::RegenReward(float amount):
  Reward("+" + std::to_string(static_cast< int >(amount)) + " MP Regen"),
  amount_(amount)
{}

void hvostov::RegenReward::apply(Hero& hero) const
{
  float current = hero.getStats().getResourceRegen();
  hero.getStats().setResourceRegen(current + amount_);
}

hvostov::UnlockSkillReward::UnlockSkillReward(bool include_ults):
  Reward("Unlock new skill"),
  include_ults_(include_ults)
{}

void hvostov::UnlockSkillReward::apply(Hero& hero) const
{
  std::vector< size_t > locked;
  size_t count = hero.getSkillManager().getSkillCount();
  size_t limit = include_ults_ ? count : (count > 2 ? count - 2 : count);

  for (size_t i = 0; i < limit; i++) {
    if (hero.getSkillManager().isSkillLocked(i))
      locked.push_back(i);
  }

  if (locked.empty()) {
    hero.getSkillManager().addLevelsToSkill(0, 1);
    return;
  }

  hero.getSkillManager().unlockSkill(locked[Random::getInt(0, static_cast< int >(locked.size()) - 1)]);
}

hvostov::MultiSkillLevelReward::MultiSkillLevelReward(size_t levels):
  Reward("+" + std::to_string(levels) + " lvl to random skill"),
  levels_(levels)
{}

void hvostov::MultiSkillLevelReward::apply(Hero& hero) const
{
  std::vector< size_t > unlocked;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++)
    if (!hero.getSkillManager().isSkillLocked(i))
      unlocked.push_back(i);
  if (unlocked.empty())
    return;
  hero.getSkillManager().addLevelsToSkill(unlocked[Random::getInt(0, static_cast< int >(unlocked.size()) - 1)],
                                          levels_);
}

hvostov::FullHealReward::FullHealReward(float pct):
  Reward("Heal " + std::to_string(static_cast< int >(pct * 100)) + "% HP"),
  percent_(pct)
{}

void hvostov::FullHealReward::apply(Hero& hero) const
{
  hero.getDamageManager().heal(hero.getStats().getHealth().getTotal() * percent_);
}

std::vector< std::unique_ptr< hvostov::Reward > > hvostov::RewardFactory::generateCommonRewards(int floor)
{
  std::vector< std::unique_ptr< Reward > > rewards;
  float b = 1.0f + (static_cast< float >(floor) - 1) * 0.15f;

  std::vector< int > types = {0, 1, 2, 3};
  int count = Random::getInt(3, 4);
  for (int i = 0; i < count && !types.empty(); i++) {
    int idx = Random::getInt(0, static_cast< int >(types.size()) - 1);
    int type = types[idx];
    types.erase(types.begin() + idx);
    switch (type) {
      case 0:
        rewards.push_back(std::make_unique< HealthReward >(20.0f * b));
        break;
      case 1:
        rewards.push_back(std::make_unique< DamageReward >(8.0f * b));
        break;
      case 2:
        rewards.push_back(std::make_unique< DefenseReward >(6.0f * b));
        break;
      case 3:
        rewards.push_back(std::make_unique< SpeedReward >(5.0f * b));
        break;
    }
  }
  return rewards;
}

std::vector< std::unique_ptr< hvostov::Reward > > hvostov::RewardFactory::generateRareRewards(const Hero&, int floor)
{
  std::vector< std::unique_ptr< Reward > > rewards;
  float b = 1.0f + (static_cast< float >(floor) - 1) * 0.2f;

  std::vector< int > types = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  int count = Random::getInt(4, 5);
  for (int i = 0; i < count && !types.empty(); i++) {
    int idx = Random::getInt(0, static_cast< int >(types.size()) - 1);
    int type = types[idx];
    types.erase(types.begin() + idx);
    switch (type) {
      case 0:
        rewards.push_back(std::make_unique< HealthReward >(40.0f * b));
        break;
      case 1:
        rewards.push_back(std::make_unique< DamageReward >(15.0f * b));
        break;
      case 2:
        rewards.push_back(std::make_unique< DefenseReward >(12.0f * b));
        break;
      case 3:
        rewards.push_back(std::make_unique< SpeedReward >(10.0f * b));
        break;
      case 4:
        rewards.push_back(std::make_unique< PercentHealthReward >(10.0f));
        break;
      case 5:
        rewards.push_back(std::make_unique< PercentDamageReward >(8.0f));
        break;
      case 6:
        rewards.push_back(std::make_unique< FullHealReward >(0.25f));
        break;
      case 7:
        rewards.push_back(std::make_unique< CritChanceReward >(0.03f));
        break;
      case 8:
        rewards.push_back(std::make_unique< ResourceReward >(15.0f * b));
        break;
    }
  }
  return rewards;
}

std::vector< std::unique_ptr< hvostov::Reward > > hvostov::RewardFactory::generateEpicRewards(const Hero& hero,
                                                                                              int floor)
{
  (void)floor;
  std::vector< std::unique_ptr< Reward > > rewards;

  bool found_locked = false;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (i >= hero.getSkillManager().getSkillCount() - 2)
      continue;
    if (hero.getSkillManager().isSkillLocked(i)) {
      found_locked = true;
      break;
    }
  }

  if (found_locked)
    rewards.push_back(std::make_unique< UnlockSkillReward >(false));
  else
    rewards.push_back(std::make_unique< MultiSkillLevelReward >(3));

  std::vector< int > types = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  int count = Random::getInt(3, 4);
  for (int i = 0; i < count && !types.empty(); i++) {
    int idx = Random::getInt(0, static_cast< int >(types.size()) - 1);
    int type = types[idx];
    types.erase(types.begin() + idx);
    switch (type) {
      case 0:
        rewards.push_back(std::make_unique< PercentHealthReward >(20.0f));
        break;
      case 1:
        rewards.push_back(std::make_unique< PercentDamageReward >(15.0f));
        break;
      case 2:
        rewards.push_back(std::make_unique< FullHealReward >(0.5f));
        break;
      case 3:
        rewards.push_back(std::make_unique< CritChanceReward >(0.05f));
        break;
      case 4:
        rewards.push_back(std::make_unique< CritDamageReward >(0.15f));
        break;
      case 5:
        rewards.push_back(std::make_unique< PercentDefenseReward >(12.0f));
        break;
      case 6:
        rewards.push_back(std::make_unique< PercentSpeedReward >(10.0f));
        break;
      case 7:
        rewards.push_back(std::make_unique< MultiSkillLevelReward >(2));
        break;
      case 8:
        rewards.push_back(std::make_unique< RegenReward >(5.0f));
        break;
    }
  }
  return rewards;
}

std::vector< std::unique_ptr< hvostov::Reward > > hvostov::RewardFactory::generateLegendaryRewards(const Hero& hero,
                                                                                                   int floor)
{
  (void)floor;
  std::vector< std::unique_ptr< Reward > > rewards;

  bool found_locked = false;
  for (size_t i = 0; i < hero.getSkillManager().getSkillCount(); i++) {
    if (hero.getSkillManager().isSkillLocked(i)) {
      found_locked = true;
      break;
    }
  }

  if (found_locked)
    rewards.push_back(std::make_unique< UnlockSkillReward >(true));
  else {
    size_t ult = hero.getSkillManager().getSkillCount() - 1;
    if (!hero.getSkillManager().isSkillLocked(ult))
      rewards.push_back(std::make_unique< SkillLevelReward >(ult, 3));
    else
      rewards.push_back(std::make_unique< MultiSkillLevelReward >(3));
  }

  std::vector< int > types = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int count = Random::getInt(4, 5);
  for (int i = 0; i < count && !types.empty(); i++) {
    int idx = Random::getInt(0, static_cast< int >(types.size()) - 1);
    int type = types[idx];
    types.erase(types.begin() + idx);
    switch (type) {
      case 0:
        rewards.push_back(std::make_unique< PercentHealthReward >(35.0f));
        break;
      case 1:
        rewards.push_back(std::make_unique< PercentDamageReward >(25.0f));
        break;
      case 2:
        rewards.push_back(std::make_unique< FullHealReward >(1.0f));
        break;
      case 3:
        rewards.push_back(std::make_unique< CritDamageReward >(0.3f));
        break;
      case 4:
        rewards.push_back(std::make_unique< PercentDefenseReward >(20.0f));
        break;
      case 5:
        rewards.push_back(std::make_unique< PercentSpeedReward >(15.0f));
        break;
      case 6:
        rewards.push_back(std::make_unique< CritChanceReward >(0.08f));
        break;
      case 7:
        rewards.push_back(std::make_unique< MultiSkillLevelReward >(4));
        break;
      case 8:
        rewards.push_back(std::make_unique< PercentResourceReward >(25.0f));
        break;
      case 9:
        rewards.push_back(std::make_unique< RegenReward >(10.0f));
        break;
    }
  }
  return rewards;
}
