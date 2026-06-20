#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include <cstddef>
#include <memory>
#include "types.hpp"

namespace hvostov {
  class Actor;

  class Effect {
  public:
    Effect(EffectType type, size_t duration);
    virtual ~Effect() = default;

    void onApply(Actor* owner);
    void onTick(Actor* owner);
    void onRemove(Actor* owner);
    bool isExpired() const;
    virtual bool isStackable() const = 0;
    virtual void doOnStack(Effect* effect);
    virtual bool isHarmful() const = 0;
    float getDuration() const;
    EffectType getEffectType() const;
    virtual std::unique_ptr< Effect > clone() const = 0;

  protected:
    void reduceDuration();
    void setDuration(size_t value);

  private:
    EffectType type_;
    size_t duration_ = 3;

    virtual void doOnApply(Actor* owner) = 0;
    virtual void doOnTick(Actor* owner) = 0;
    virtual void doOnRemove(Actor* owner) = 0;
  };

  class NailingEffect : public Effect {
  public:
    NailingEffect(size_t duration, float speed_reduction);
    NailingEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float speed_reduction_ = 20.0f;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class BleedingEffect : public Effect {
  public:
    BleedingEffect(size_t duration, float percentage_damage_per_move, float flat_damage_per_move);
    BleedingEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float percentage_damage_per_move_ = 0.03f;
    float flat_damage_per_move_ = 10.0f;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class ParryEffect : public Effect {
  public:
    ParryEffect(size_t duration, float parry_chance);
    ParryEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;
    float getParryChance() const;

  private:
    float parry_chance_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class DamageBuffEffect : public Effect {
  public:
    DamageBuffEffect(size_t duration, float buff);
    DamageBuffEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;
    float getDamageBuff() const;

  private:
    float damage_buff_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class BurnEffect : public Effect {
  public:
    BurnEffect(size_t duration, float damage_per_tick);
    BurnEffect();
    bool isStackable() const override;
    bool isHarmful() const override;
    void doOnStack(Effect* effect) override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float damage_per_tick_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class SlowEffect : public Effect {
  public:
    SlowEffect(size_t duration, float speed_multiplier);
    SlowEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float speed_multiplier_;
    float original_speed_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class StunEffect : public Effect {
  public:
    StunEffect(size_t duration);
    StunEffect();
    bool isHarmful() const override;
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    std::unique_ptr< Effect > clone() const override;

  private:
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class TauntEffect : public Effect {
  public:
    TauntEffect(size_t duration, Actor* taunter = nullptr);
    TauntEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;

    Actor* getTaunter() const
    {
      return taunter_;
    }

  private:
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
    Actor* taunter_ = nullptr;
  };

  class DefenseBuffEffect : public Effect {
  public:
    DefenseBuffEffect(size_t duration, float multiplier);
    DefenseBuffEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;
    float getMultiplier() const;

  private:
    float multiplier_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class SpeedBuffEffect : public Effect {
  public:
    SpeedBuffEffect(size_t duration, float multiplier);
    SpeedBuffEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;
    float getMultiplier() const;

  private:
    float multiplier_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class InvincibilityEffect : public Effect {
  public:
    InvincibilityEffect(size_t duration);
    InvincibilityEffect();
    bool isStackable() const override;
    bool isHarmful() const override;
    void doOnStack(Effect* effect) override;
    std::unique_ptr< Effect > clone() const override;

  private:
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class DamageAmplifyEffect : public Effect {
  public:
    DamageAmplifyEffect(size_t duration, float multiplier);
    DamageAmplifyEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;
    float getMultiplier() const;

  private:
    float multiplier_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class HealOverTimeEffect : public Effect {
  public:
    HealOverTimeEffect(size_t duration, float percent_per_tick);
    HealOverTimeEffect();
    bool isHarmful() const override;
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float percent_per_tick_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class ShieldEffect : public Effect {
  public:
    ShieldEffect(size_t duration, float shield_amount);
    ShieldEffect();
    bool isStackable() const override;
    void doOnStack(Effect* effect) override;
    bool isHarmful() const override;
    std::unique_ptr< Effect > clone() const override;

  private:
    float shield_amount_;
    void doOnApply(Actor* owner) override;
    void doOnTick(Actor* owner) override;
    void doOnRemove(Actor* owner) override;
  };

  class CritChanceBuffEffect : public Effect {
  public:
    CritChanceBuffEffect(size_t duration, float amount);
    CritChanceBuffEffect();

    bool isStackable() const override
    {
      return true;
    }

    bool isHarmful() const override
    {
      return false;
    }

    std::unique_ptr< Effect > clone() const override;
    void doOnApply(Actor* owner) override;

    void doOnTick(Actor*) override
    {}

    void doOnRemove(Actor* owner) override;

  private:
    float amount_;
  };
}

#endif
