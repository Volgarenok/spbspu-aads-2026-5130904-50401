#ifndef TYPES_HPP
#define TYPES_HPP

namespace hvostov {
  enum class ActorType {
    Hero,
    Monster,
    Npc
  };
  enum class ScaleType {
    Damage,
    Health,
    Defense,
    Resource
  };

  enum class HeroClass {
    NoClass,
    Knight,
    Mage,
    Assasin,
    Palladin,
    Priest,
    Archer
  };

  enum class StatType {
    Health,
    Damage,
    Defense,
    Speed,
    CritChance,
    CritDamage,
    Resource
  };

  enum class EffectType {
    Poisoned,
    Stunned,
    Blinded,
    Accurate,
    Nailing,
    Bleeding,
    Parry,
    DamageBuff,
    Burn,
    Slow,
    Stun,
    Taunt,
    DefenseBuff,
    SpeedBuff,
    Invincibility,
    DamageAmplify,
    HealOverTime,
    Shield
  };

  enum class SkillTarget {
    Enemy,
    Ally,
    Self,
    AllEnemies,
    AllAllies
  };
  enum class RewardQuality {
    Common,
    Rare,
    Epic,
    Legendary
  };
}

#endif
