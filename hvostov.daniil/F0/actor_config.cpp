#include "actor_config.hpp"

hvostov::ActorConfig hvostov::Presets::Knight()
{
  hvostov::ActorConfig config;
  config.health = 200.0f;
  config.resource = 100.0f;
  config.resource_regen = 10.0f;
  config.damage = 25.0f;
  config.defense = 30.0f;
  config.speed = 80.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Hero;
  config.hero_class = hvostov::HeroClass::Knight;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Mage()
{
  hvostov::ActorConfig config;
  config.health = 120.0f;
  config.resource = 200.0f;
  config.resource_regen = 30.0f;
  config.damage = 28.0f;
  config.defense = 10.0f;
  config.speed = 110.0f;
  config.crit_chance = 0.10f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Hero;
  config.hero_class = hvostov::HeroClass::Mage;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Assassin()
{
  hvostov::ActorConfig config;
  config.health = 140.0f;
  config.resource = 120.0f;
  config.resource_regen = 10.0f;
  config.damage = 30.0f;
  config.defense = 15.0f;
  config.speed = 130.0f;
  config.crit_chance = 0.20f;
  config.crit_damage = 1.8f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Hero;
  config.hero_class = hvostov::HeroClass::Assasin;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Paladin()
{
  hvostov::ActorConfig config;
  config.health = 220.0f;
  config.resource = 80.0f;
  config.resource_regen = 8.0f;
  config.damage = 22.0f;
  config.defense = 35.0f;
  config.speed = 85.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.10f;
  config.actor_type = hvostov::ActorType::Hero;
  config.hero_class = hvostov::HeroClass::Palladin;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Priest()
{
  hvostov::ActorConfig config;
  config.health = 150.0f;
  config.resource = 160.0f;
  config.resource_regen = 20.0f;
  config.damage = 15.0f;
  config.defense = 10.0f;
  config.speed = 90.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Hero;
  config.hero_class = hvostov::HeroClass::Priest;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Archer()
{
  hvostov::ActorConfig config;
  config.health = 130.0f;
  config.resource = 100.0f;
  config.resource_regen = 10.0f;
  config.damage = 24.0f;
  config.defense = 15.0f;
  config.speed = 120.0f;
  config.crit_chance = 0.15f;
  config.crit_damage = 1.6f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Hero;
  config.hero_class = hvostov::HeroClass::Archer;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Goblin(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 40.0f + stage * 8;
  config.resource = 0.0f;
  config.damage = 6.0f + stage * 2;
  config.defense = 3.0f + stage * 1;
  config.speed = 90.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Wolf(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 35.0f + stage * 7;
  config.resource = 0.0f;
  config.damage = 8.0f + stage * 2;
  config.defense = 2.0f + stage * 1;
  config.speed = 110.0f;
  config.crit_chance = 0.08f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Skeleton(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 45.0f + stage * 9;
  config.resource = 0.0f;
  config.damage = 7.0f + stage * 2;
  config.defense = 5.0f + stage * 1;
  config.speed = 95.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.05f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Slime(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 50.0f + stage * 10;
  config.resource = 0.0f;
  config.damage = 5.0f + stage * 1;
  config.defense = 8.0f + stage * 2;
  config.speed = 70.0f;
  config.crit_chance = 0.03f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.1f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Bat(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 30.0f + stage * 6;
  config.resource = 0.0f;
  config.damage = 6.0f + stage * 2;
  config.defense = 2.0f + stage * 0.5f;
  config.speed = 120.0f;
  config.crit_chance = 0.1f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Spider(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 40.0f + stage * 8;
  config.resource = 0.0f;
  config.damage = 7.0f + stage * 2;
  config.defense = 4.0f + stage * 1;
  config.speed = 100.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Zombie(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 55.0f + stage * 11;
  config.resource = 0.0f;
  config.damage = 7.0f + stage * 2;
  config.defense = 6.0f + stage * 1;
  config.speed = 75.0f;
  config.crit_chance = 0.03f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.08f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Cultist(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 45.0f + stage * 9;
  config.resource = 50.0f + stage * 10;
  config.damage = 8.0f + stage * 2;
  config.defense = 3.0f + stage * 1;
  config.speed = 95.0f;
  config.crit_chance = 0.08f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Bandit(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 42.0f + stage * 8;
  config.resource = 0.0f;
  config.damage = 9.0f + stage * 2;
  config.defense = 4.0f + stage * 1;
  config.speed = 105.0f;
  config.crit_chance = 0.1f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Imp(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 30.0f + stage * 6;
  config.resource = 40.0f + stage * 8;
  config.damage = 10.0f + stage * 3;
  config.defense = 2.0f + stage * 0.5f;
  config.speed = 115.0f;
  config.crit_chance = 0.12f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Harpy(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 38.0f + stage * 7;
  config.resource = 0.0f;
  config.damage = 7.0f + stage * 2;
  config.defense = 3.0f + stage * 1;
  config.speed = 125.0f;
  config.crit_chance = 0.08f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Mummy(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 50.0f + stage * 10;
  config.resource = 30.0f + stage * 6;
  config.damage = 6.0f + stage * 2;
  config.defense = 7.0f + stage * 1;
  config.speed = 80.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.05f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::OrcWarrior(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 100.0f + stage * 20;
  config.resource = 0.0f;
  config.damage = 18.0f + stage * 4;
  config.defense = 12.0f + stage * 2;
  config.speed = 85.0f;
  config.crit_chance = 0.08f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.05f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::DarkElf(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 80.0f + stage * 16;
  config.resource = 80.0f + stage * 16;
  config.damage = 16.0f + stage * 4;
  config.defense = 8.0f + stage * 1;
  config.speed = 110.0f;
  config.crit_chance = 0.15f;
  config.crit_damage = 1.8f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::StoneGolem(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 150.0f + stage * 30;
  config.resource = 0.0f;
  config.damage = 14.0f + stage * 3;
  config.defense = 20.0f + stage * 4;
  config.speed = 60.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.15f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::FireElemental(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 90.0f + stage * 18;
  config.resource = 100.0f + stage * 20;
  config.damage = 20.0f + stage * 5;
  config.defense = 6.0f + stage * 1;
  config.speed = 100.0f;
  config.crit_chance = 0.1f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::IceElemental(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 100.0f + stage * 20;
  config.resource = 100.0f + stage * 20;
  config.damage = 18.0f + stage * 4;
  config.defense = 10.0f + stage * 2;
  config.speed = 90.0f;
  config.crit_chance = 0.08f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.05f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Thunderbird(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 85.0f + stage * 17;
  config.resource = 60.0f + stage * 12;
  config.damage = 22.0f + stage * 5;
  config.defense = 5.0f + stage * 1;
  config.speed = 130.0f;
  config.crit_chance = 0.12f;
  config.crit_damage = 1.6f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::ShadowAssassin(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 70.0f + stage * 14;
  config.resource = 70.0f + stage * 14;
  config.damage = 25.0f + stage * 6;
  config.defense = 4.0f + stage * 1;
  config.speed = 125.0f;
  config.crit_chance = 0.2f;
  config.crit_damage = 2.0f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.0f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::AncientTreant(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 180.0f + stage * 35;
  config.resource = 40.0f + stage * 8;
  config.damage = 15.0f + stage * 3;
  config.defense = 18.0f + stage * 3;
  config.speed = 65.0f;
  config.crit_chance = 0.05f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.1f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::TrollKing(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 300.0f + stage * 50;
  config.resource = 0.0f;
  config.damage = 30.0f + stage * 6;
  config.defense = 25.0f + stage * 5;
  config.speed = 70.0f;
  config.crit_chance = 0.08f;
  config.crit_damage = 1.5f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.1f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Dragon(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 350.0f + stage * 60;
  config.resource = 150.0f + stage * 30;
  config.damage = 35.0f + stage * 7;
  config.defense = 20.0f + stage * 4;
  config.speed = 85.0f;
  config.crit_chance = 0.1f;
  config.crit_damage = 1.6f;
  config.damage_bonus = 1.0f;
  config.damage_reduction = 0.2f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}

hvostov::ActorConfig hvostov::Presets::Lich(size_t stage)
{
  hvostov::ActorConfig config;
  config.health = 280.0f + stage * 45;
  config.resource = 200.0f + stage * 40;
  config.damage = 28.0f + stage * 5;
  config.defense = 15.0f + stage * 3;
  config.speed = 90.0f;
  config.crit_chance = 0.1f;
  config.crit_damage = 1.6f;
  config.damage_bonus = 1.2f;
  config.damage_reduction = 0.1f;
  config.actor_type = hvostov::ActorType::Monster;
  return config;
}
