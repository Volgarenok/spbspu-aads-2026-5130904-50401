#ifndef SKILLS_HPP
#define SKILLS_HPP

#include <memory>
#include <string>
#include <vector>
#include "effects.hpp"
#include "types.hpp"

namespace hvostov {

  class Actor;

  class Skill {
  public:
    void processSkill(Actor* owner, Actor* target);
    void processSkill(Actor* owner, std::vector< Actor* > targets);
    virtual ~Skill() = default;

    std::string getSkillName() const;
    std::string getDescription() const;
    void addSkillLevel(size_t value);
    size_t getSkillLevel() const;

    virtual float getResourceCost() const
    {
      return resource_cost_;
    }

    void setResourceCost(float cost)
    {
      resource_cost_ = cost;
    }

    virtual SkillTarget getTargetType() const
    {
      return SkillTarget::Enemy;
    }

  protected:
    std::string skill_name_;
    std::string description_;
    size_t skill_level_ = 1;
    float resource_cost_ = 0.0f;
    float level_scale_ = 0.01f;

    virtual void doProcessSkill(Actor* owner, Actor* target) = 0;
  };

  class AttackSkill : public Skill {
  public:
    float getDamageMultiplier() const;
    float getFlatDamage() const;
    ScaleType getScaleType() const;
    float getLevelDamageScale() const;

  protected:
    float damage_multiplier_ = 1.0f;
    float flat_damage_ = 0.0f;
    ScaleType scale_type_ = ScaleType::Damage;
    float level_damage_scale_ = 0.05f;
  };

  class BaseAttack : public AttackSkill {
  public:
    BaseAttack();
    BaseAttack(float damage_multiplier, float flat_damage, ScaleType scale_type);

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightSlash : public AttackSkill {
  public:
    KnightSlash();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ShieldBash : public AttackSkill {
  public:
    ShieldBash();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Taunt : public Skill {
  public:
    Taunt();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightProtection : public Skill {
  public:
    KnightProtection();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightUltimate1 : public AttackSkill {
  public:
    KnightUltimate1();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class KnightUltimate2 : public Skill {
  public:
    KnightUltimate2();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Self;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Fireball : public AttackSkill {
  public:
    Fireball();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class IceShard : public AttackSkill {
  public:
    IceShard();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class LightningStrike : public AttackSkill {
  public:
    LightningStrike();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ManaShield : public Skill {
  public:
    ManaShield();
    ManaShield(float shield_amount, size_t duration);

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Self;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
    float shield_amount_;
    size_t duration_;
  };

  class MageUltimate1 : public AttackSkill {
  public:
    MageUltimate1();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::AllEnemies;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class MageUltimate2 : public Skill {
  public:
    MageUltimate2();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Backstab : public AttackSkill {
  public:
    Backstab();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PoisonDagger : public AttackSkill {
  public:
    PoisonDagger();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    std::unique_ptr< Effect > effect_;
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ShadowStep : public Skill {
  public:
    ShadowStep();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Self;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class SpeedBoost : public Skill {
  public:
    SpeedBoost();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Self;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AssassinUltimate1 : public AttackSkill {
  public:
    AssassinUltimate1();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AssassinUltimate2 : public AttackSkill {
  public:
    AssassinUltimate2();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HolyStrike : public AttackSkill {
  public:
    HolyStrike();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class DivineShield : public Skill {
  public:
    DivineShield();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HolyLight : public Skill {
  public:
    HolyLight();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AuraOfProtection : public Skill {
  public:
    AuraOfProtection();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::AllAllies;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PaladinUltimate1 : public AttackSkill {
  public:
    PaladinUltimate1();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::AllEnemies;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PaladinUltimate2 : public AttackSkill {
  public:
    PaladinUltimate2();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HolySmite : public AttackSkill {
  public:
    HolySmite();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Heal : public Skill {
  public:
    Heal();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Purify : public Skill {
  public:
    Purify();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class SaintUltimate1 : public Skill {
  public:
    SaintUltimate1();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class SaintUltimate2 : public Skill {
  public:
    SaintUltimate2();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::AllAllies;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Smite : public AttackSkill {
  public:
    Smite();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HealPrayer : public Skill {
  public:
    HealPrayer();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ShieldOfFaith : public Skill {
  public:
    ShieldOfFaith();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Renew : public Skill {
  public:
    Renew();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PriestUltimate1 : public Skill {
  public:
    PriestUltimate1();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Ally;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PriestUltimate2 : public Skill {
  public:
    PriestUltimate2();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::AllAllies;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class QuickShot : public AttackSkill {
  public:
    QuickShot();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class AimedShot : public AttackSkill {
  public:
    AimedShot();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class PiercingArrow : public AttackSkill {
  public:
    PiercingArrow();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class HawkEye : public Skill {
  public:
    HawkEye();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Self;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ArcherUltimate1 : public AttackSkill {
  public:
    ArcherUltimate1();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::AllEnemies;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class ArcherUltimate2 : public AttackSkill {
  public:
    ArcherUltimate2();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class DemonMark : public Skill {
  public:
    DemonMark();

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
  };

  class Curse : public Skill {
  public:
    Curse();
    Curse(float damage_multiplier, int duration);

    SkillTarget getTargetType() const override
    {
      return SkillTarget::Enemy;
    }

  private:
    void doProcessSkill(Actor* owner, Actor* target) override;
    float damage_multiplier_;
    int duration_;
  };
}

#endif
