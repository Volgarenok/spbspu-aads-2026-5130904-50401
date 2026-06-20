#include "hero.hpp"
#include <iostream>
#include "actor_config.hpp"
#include "skills.hpp"

hvostov::Hero::Hero(const ActorConfig& config):
  Actor(config)
{
  hero_class_ = config.hero_class;
}

void hvostov::Hero::die()
{
  getStats().setIsDead(true);
  std::cout << getClassName() << " has died!\n";
}

hvostov::HeroClass hvostov::Hero::getHeroClass() const
{
  return hero_class_;
}

hvostov::Knight::Knight():
  Hero(Presets::Knight())
{
  getStats().getCurrentResource() = getStats().getResource().getTotal();
  setupSkills();
  hero_class_ = Presets::Knight().hero_class;
}

std::string hvostov::Knight::getClassName() const
{
  return "Knight";
}

void hvostov::Knight::setupSkills()
{
  auto base = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(base));
  auto slash = std::make_unique< KnightSlash >();
  auto shield = std::make_unique< ShieldBash >();
  auto ult1 = std::make_unique< KnightUltimate1 >();

  getSkillManager().addSkill(std::move(slash));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(ult1));

  auto taunt = std::make_unique< Taunt >();
  auto protection = std::make_unique< KnightProtection >();
  auto ult2 = std::make_unique< KnightUltimate2 >();

  getSkillManager().addLockedSkill(std::move(taunt));
  getSkillManager().addLockedSkill(std::move(protection));
  getSkillManager().addLockedSkill(std::move(ult2));
}

hvostov::Mage::Mage():
  Hero(Presets::Mage())
{
  getStats().getCurrentResource() = getStats().getResource().getTotal();
  setupSkills();
  hero_class_ = Presets::Mage().hero_class;
}

std::string hvostov::Mage::getClassName() const
{
  return "Mage";
}

void hvostov::Mage::setupSkills()
{
  auto base = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(base));
  auto fireball = std::make_unique< Fireball >();
  auto ice = std::make_unique< IceShard >();
  auto lightning = std::make_unique< LightningStrike >();

  getSkillManager().addSkill(std::move(fireball));
  getSkillManager().addSkill(std::move(ice));
  getSkillManager().addSkill(std::move(lightning));

  auto mana_shield = std::make_unique< ManaShield >();
  auto ult1 = std::make_unique< MageUltimate1 >();
  auto ult2 = std::make_unique< MageUltimate2 >();

  getSkillManager().addLockedSkill(std::move(mana_shield));
  getSkillManager().addLockedSkill(std::move(ult1));
  getSkillManager().addLockedSkill(std::move(ult2));
}

hvostov::Assassin::Assassin():
  Hero(Presets::Assassin())
{
  getStats().getCurrentResource() = getStats().getResource().getTotal();
  setupSkills();
  hero_class_ = Presets::Assassin().hero_class;
}

std::string hvostov::Assassin::getClassName() const
{
  return "Assassin";
}

void hvostov::Assassin::setupSkills()
{
  auto base = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(base));
  auto backstab = std::make_unique< Backstab >();
  auto poison = std::make_unique< PoisonDagger >();
  auto shadow = std::make_unique< ShadowStep >();

  getSkillManager().addSkill(std::move(backstab));
  getSkillManager().addSkill(std::move(poison));
  getSkillManager().addSkill(std::move(shadow));

  auto evasion = std::make_unique< SpeedBoost >();
  auto ult1 = std::make_unique< AssassinUltimate1 >();
  auto ult2 = std::make_unique< AssassinUltimate2 >();

  getSkillManager().addLockedSkill(std::move(evasion));
  getSkillManager().addLockedSkill(std::move(ult1));
  getSkillManager().addLockedSkill(std::move(ult2));
}

hvostov::Paladin::Paladin():
  Hero(Presets::Paladin())
{
  getStats().getCurrentResource() = getStats().getResource().getTotal();
  setupSkills();
  hero_class_ = Presets::Paladin().hero_class;
}

std::string hvostov::Paladin::getClassName() const
{
  return "Paladin";
}

void hvostov::Paladin::setupSkills()
{
  auto base = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(base));
  auto holy = std::make_unique< HolyStrike >();
  auto shield = std::make_unique< DivineShield >();
  auto light = std::make_unique< HolyLight >();

  getSkillManager().addSkill(std::move(holy));
  getSkillManager().addSkill(std::move(shield));
  getSkillManager().addSkill(std::move(light));

  auto aura = std::make_unique< AuraOfProtection >();
  auto ult1 = std::make_unique< PaladinUltimate1 >();
  auto ult2 = std::make_unique< PaladinUltimate2 >();

  getSkillManager().addLockedSkill(std::move(aura));
  getSkillManager().addLockedSkill(std::move(ult1));
  getSkillManager().addLockedSkill(std::move(ult2));
}

hvostov::Priest::Priest():
  Hero(Presets::Priest())
{
  getStats().getCurrentResource() = getStats().getResource().getTotal();
  setupSkills();
  hero_class_ = Presets::Priest().hero_class;
}

std::string hvostov::Priest::getClassName() const
{
  return "Priest";
}

void hvostov::Priest::setupSkills()
{
  auto base = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(base));
  auto smite = std::make_unique< Smite >();
  auto heal = std::make_unique< HealPrayer >();
  auto shield = std::make_unique< ShieldOfFaith >();

  getSkillManager().addSkill(std::move(smite));
  getSkillManager().addSkill(std::move(heal));
  getSkillManager().addSkill(std::move(shield));

  auto renew = std::make_unique< Renew >();
  auto ult1 = std::make_unique< PriestUltimate1 >();
  auto ult2 = std::make_unique< PriestUltimate2 >();

  getSkillManager().addLockedSkill(std::move(renew));
  getSkillManager().addLockedSkill(std::move(ult1));
  getSkillManager().addLockedSkill(std::move(ult2));
}

hvostov::Archer::Archer():
  Hero(Presets::Archer())
{
  getStats().getCurrentResource() = getStats().getResource().getTotal();
  setupSkills();
  hero_class_ = Presets::Archer().hero_class;
}

std::string hvostov::Archer::getClassName() const
{
  return "Archer";
}

void hvostov::Archer::setupSkills()
{
  auto base = std::make_unique< hvostov::BaseAttack >();
  getSkillManager().addSkill(std::move(base));
  auto quick = std::make_unique< QuickShot >();
  auto aimed = std::make_unique< AimedShot >();
  auto piercing = std::make_unique< PiercingArrow >();

  getSkillManager().addSkill(std::move(quick));
  getSkillManager().addSkill(std::move(aimed));
  getSkillManager().addSkill(std::move(piercing));

  auto hawk = std::make_unique< HawkEye >();
  auto ult1 = std::make_unique< ArcherUltimate1 >();
  auto ult2 = std::make_unique< ArcherUltimate2 >();

  getSkillManager().addLockedSkill(std::move(hawk));
  getSkillManager().addLockedSkill(std::move(ult1));
  getSkillManager().addLockedSkill(std::move(ult2));
}

std::string hvostov::Hero::getName() const
{
  return getClassName();
}
