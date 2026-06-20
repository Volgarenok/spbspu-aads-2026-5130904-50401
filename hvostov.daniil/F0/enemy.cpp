#include "enemy.hpp"
#include <iostream>
#include "actor_config.hpp"
#include "skills.hpp"

hvostov::Enemy::Enemy(const hvostov::ActorConfig& config):
  hvostov::Actor(config)
{}

void hvostov::Enemy::die()
{
  std::cout << name_ << " has been defeated!\n";
  getStats().setIsDead(true);
}

std::string hvostov::Enemy::getName() const
{
  return name_;
}

void hvostov::Enemy::setName(const std::string& name)
{
  name_ = name;
}

size_t hvostov::Enemy::getStage() const
{
  return stage_;
}

void hvostov::Enemy::setStage(size_t stage)
{
  stage_ = stage;
}

hvostov::Goblin::Goblin(size_t stage):
  hvostov::Enemy(hvostov::Presets::Goblin(stage))
{
  name_ = "Goblin";
  stage_ = stage;
  setupSkills();
}

void hvostov::Goblin::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

std::string hvostov::Goblin::getType() const
{
  return "Goblin";
}

hvostov::Wolf::Wolf(size_t stage):
  hvostov::Enemy(hvostov::Presets::Wolf(stage))
{
  name_ = "Wolf";
  stage_ = stage;
  setupSkills();
}

void hvostov::Wolf::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

std::string hvostov::Wolf::getType() const
{
  return "Wolf";
}

hvostov::Skeleton::Skeleton(size_t stage):
  hvostov::Enemy(hvostov::Presets::Skeleton(stage))
{
  name_ = "Skeleton";
  stage_ = stage;
  setupSkills();
}

void hvostov::Skeleton::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

std::string hvostov::Skeleton::getType() const
{
  return "Skeleton";
}

hvostov::Slime::Slime(size_t stage):
  hvostov::Enemy(hvostov::Presets::Slime(stage))
{
  name_ = "Slime";
  stage_ = stage;
  setupSkills();
}

void hvostov::Slime::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

std::string hvostov::Slime::getType() const
{
  return "Slime";
}

hvostov::Bat::Bat(size_t stage):
  hvostov::Enemy(hvostov::Presets::Bat(stage))
{
  name_ = "Bat";
  stage_ = stage;
  setupSkills();
}

void hvostov::Bat::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

std::string hvostov::Bat::getType() const
{
  return "Bat";
}

hvostov::Spider::Spider(size_t stage):
  hvostov::Enemy(hvostov::Presets::Spider(stage))
{
  name_ = "Spider";
  stage_ = stage;
  setupSkills();
}

void hvostov::Spider::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

std::string hvostov::Spider::getType() const
{
  return "Spider";
}

hvostov::Zombie::Zombie(size_t stage):
  hvostov::Enemy(hvostov::Presets::Zombie(stage))
{
  name_ = "Zombie";
  stage_ = stage;
  setupSkills();
}

void hvostov::Zombie::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(attack));
}

std::string hvostov::Zombie::getType() const
{
  return "Zombie";
}

hvostov::Cultist::Cultist(size_t stage):
  hvostov::Enemy(hvostov::Presets::Cultist(stage))
{
  name_ = "Cultist";
  stage_ = stage;
  setupSkills();
}

void hvostov::Cultist::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto curse = std::make_unique< hvostov::Curse >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(curse));
}

std::string hvostov::Cultist::getType() const
{
  return "Cultist";
}

hvostov::Bandit::Bandit(size_t stage):
  hvostov::Enemy(hvostov::Presets::Bandit(stage))
{
  name_ = "Bandit";
  stage_ = stage;
  setupSkills();
}

void hvostov::Bandit::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto backstab = std::make_unique< hvostov::Backstab >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(backstab));
}

std::string hvostov::Bandit::getType() const
{
  return "Bandit";
}

hvostov::Imp::Imp(size_t stage):
  hvostov::Enemy(hvostov::Presets::Imp(stage))
{
  name_ = "Imp";
  stage_ = stage;
  setupSkills();
}

void hvostov::Imp::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto fireball = std::make_unique< hvostov::Fireball >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(fireball));
}

std::string hvostov::Imp::getType() const
{
  return "Imp";
}

hvostov::Harpy::Harpy(size_t stage):
  hvostov::Enemy(hvostov::Presets::Harpy(stage))
{
  name_ = "Harpy";
  stage_ = stage;
  setupSkills();
}

void hvostov::Harpy::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto evasion = std::make_unique< hvostov::SpeedBoost >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(evasion));
}

std::string hvostov::Harpy::getType() const
{
  return "Harpy";
}

hvostov::Mummy::Mummy(size_t stage):
  hvostov::Enemy(hvostov::Presets::Mummy(stage))
{
  name_ = "Mummy";
  stage_ = stage;
  setupSkills();
}

void hvostov::Mummy::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto curse = std::make_unique< hvostov::Curse >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(curse));
}

std::string hvostov::Mummy::getType() const
{
  return "Mummy";
}

hvostov::OrcWarrior::OrcWarrior(size_t stage):
  hvostov::Enemy(hvostov::Presets::OrcWarrior(stage))
{
  name_ = "Orc Warrior";
  stage_ = stage;
  setupSkills();
}

void hvostov::OrcWarrior::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto heavy = std::make_unique< hvostov::KnightSlash >();
  auto taunt = std::make_unique< hvostov::Taunt >();
  auto shield = std::make_unique< hvostov::ShieldBash >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(heavy));
  getSkillManager().addSkill(std::move(taunt));
  getSkillManager().addSkill(std::move(shield));
}

std::string hvostov::OrcWarrior::getType() const
{
  return "Orc Warrior";
}

hvostov::DarkElf::DarkElf(size_t stage):
  hvostov::Enemy(hvostov::Presets::DarkElf(stage))
{
  name_ = "Dark Elf";
  stage_ = stage;
  setupSkills();
}

void hvostov::DarkElf::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto poison = std::make_unique< hvostov::PoisonDagger >();
  auto shadow = std::make_unique< hvostov::ShadowStep >();
  auto backstab = std::make_unique< hvostov::Backstab >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(poison));
  getSkillManager().addSkill(std::move(shadow));
  getSkillManager().addSkill(std::move(backstab));
}

std::string hvostov::DarkElf::getType() const
{
  return "Dark Elf";
}

hvostov::StoneGolem::StoneGolem(size_t stage):
  hvostov::Enemy(hvostov::Presets::StoneGolem(stage))
{
  name_ = "Stone Golem";
  stage_ = stage;
  setupSkills();
}

void hvostov::StoneGolem::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto shield = std::make_unique< hvostov::ShieldBash >();
  auto protect = std::make_unique< hvostov::KnightProtection >();
  auto taunt = std::make_unique< hvostov::Taunt >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(protect));
  getSkillManager().addSkill(std::move(taunt));
}

std::string hvostov::StoneGolem::getType() const
{
  return "Stone Golem";
}

hvostov::FireElemental::FireElemental(size_t stage):
  hvostov::Enemy(hvostov::Presets::FireElemental(stage))
{
  name_ = "Fire Elemental";
  stage_ = stage;
  setupSkills();
}

void hvostov::FireElemental::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto fireball = std::make_unique< hvostov::Fireball >();
  auto meteor = std::make_unique< hvostov::MageUltimate1 >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(fireball));
  getSkillManager().addSkill(std::move(meteor));
}

std::string hvostov::FireElemental::getType() const
{
  return "Fire Elemental";
}

hvostov::IceElemental::IceElemental(size_t stage):
  hvostov::Enemy(hvostov::Presets::IceElemental(stage))
{
  name_ = "Ice Elemental";
  stage_ = stage;
  setupSkills();
}

void hvostov::IceElemental::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto ice = std::make_unique< hvostov::IceShard >();
  auto blizzard = std::make_unique< hvostov::IceShard >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(ice));
  getSkillManager().addSkill(std::move(blizzard));
}

std::string hvostov::IceElemental::getType() const
{
  return "Ice Elemental";
}

hvostov::Thunderbird::Thunderbird(size_t stage):
  hvostov::Enemy(hvostov::Presets::Thunderbird(stage))
{
  name_ = "Thunderbird";
  stage_ = stage;
  setupSkills();
}

void hvostov::Thunderbird::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto lightning = std::make_unique< hvostov::LightningStrike >();
  auto speed = std::make_unique< hvostov::SpeedBoost >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(lightning));
  getSkillManager().addSkill(std::move(speed));
}

std::string hvostov::Thunderbird::getType() const
{
  return "Thunderbird";
}

hvostov::ShadowAssassin::ShadowAssassin(size_t stage):
  hvostov::Enemy(hvostov::Presets::ShadowAssassin(stage))
{
  name_ = "Shadow Assassin";
  stage_ = stage;
  setupSkills();
}

void hvostov::ShadowAssassin::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto backstab = std::make_unique< hvostov::Backstab >();
  auto poison = std::make_unique< hvostov::PoisonDagger >();
  auto assassinate = std::make_unique< hvostov::AssassinUltimate2 >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(backstab));
  getSkillManager().addSkill(std::move(poison));
  getSkillManager().addSkill(std::move(assassinate));
}

std::string hvostov::ShadowAssassin::getType() const
{
  return "Shadow Assassin";
}

hvostov::AncientTreant::AncientTreant(size_t stage):
  hvostov::Enemy(hvostov::Presets::AncientTreant(stage))
{
  name_ = "Ancient Treant";
  stage_ = stage;
  setupSkills();
}

void hvostov::AncientTreant::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto heal = std::make_unique< hvostov::Heal >();
  auto protect = std::make_unique< hvostov::KnightProtection >();
  auto ultimate = std::make_unique< hvostov::PaladinUltimate1 >();
  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(heal));
  getSkillManager().addSkill(std::move(protect));
  getSkillManager().addSkill(std::move(ultimate));
}

std::string hvostov::AncientTreant::getType() const
{
  return "Ancient Treant";
}

hvostov::TrollKing::TrollKing(size_t stage):
  hvostov::Enemy(hvostov::Presets::TrollKing(stage))
{
  name_ = "Troll King";
  stage_ = stage;
  setupSkills();
}

void hvostov::TrollKing::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto heavy = std::make_unique< hvostov::KnightSlash >();
  auto taunt = std::make_unique< hvostov::Taunt >();
  auto shield = std::make_unique< hvostov::ShieldBash >();
  auto ult1 = std::make_unique< hvostov::KnightUltimate1 >();
  auto ult2 = std::make_unique< hvostov::KnightUltimate2 >();

  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(heavy));
  getSkillManager().addSkill(std::move(taunt));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(ult1));
  getSkillManager().addSkill(std::move(ult2));
}

std::string hvostov::TrollKing::getType() const
{
  return "Troll King";
}

hvostov::Dragon::Dragon(size_t stage):
  hvostov::Enemy(hvostov::Presets::Dragon(stage))
{
  name_ = "Dragon";
  stage_ = stage;
  setupSkills();
}

void hvostov::Dragon::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto fireball = std::make_unique< hvostov::Fireball >();
  auto lightning = std::make_unique< hvostov::LightningStrike >();
  auto shield = std::make_unique< hvostov::ManaShield >();
  auto ult1 = std::make_unique< hvostov::MageUltimate1 >();
  auto ult2 = std::make_unique< hvostov::MageUltimate2 >();

  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(fireball));
  getSkillManager().addSkill(std::move(lightning));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(ult1));
  getSkillManager().addSkill(std::move(ult2));
}

std::string hvostov::Dragon::getType() const
{
  return "Dragon";
}

hvostov::Lich::Lich(size_t stage):
  hvostov::Enemy(hvostov::Presets::Lich(stage))
{
  name_ = "Lich";
  stage_ = stage;
  setupSkills();
}

void hvostov::Lich::setupSkills()
{
  auto attack = std::make_unique< hvostov::BaseAttack >();
  auto curse = std::make_unique< hvostov::Curse >();
  auto heal = std::make_unique< hvostov::Heal >();
  auto holy = std::make_unique< hvostov::HolySmite >();
  auto ult1 = std::make_unique< hvostov::SaintUltimate1 >();
  auto ult2 = std::make_unique< hvostov::SaintUltimate2 >();

  getSkillManager().addSkill(std::move(attack));
  getSkillManager().addSkill(std::move(curse));
  getSkillManager().addSkill(std::move(heal));
  getSkillManager().addSkill(std::move(holy));
  getSkillManager().addSkill(std::move(ult1));
  getSkillManager().addSkill(std::move(ult2));
}

std::string hvostov::Lich::getType() const
{
  return "Lich";
}
