#ifndef CORE_STATS_HPP
#define CORE_STATS_HPP

namespace hvostov {

  struct BaseStat {
    float base_ = 1.0f;
    void addBase(float value);
    void setBase(float value);
    float getBase() const;
  };

  struct ComplexStat : public BaseStat {
    float flat_ = 0.0f;
    float multiply_ = 1.0f;

    void addFlat(float value);
    void setFlat(float value);
    float getFlat() const;
    void addMultiply(float value);
    void setMultiply(float value);
    float getMultiply() const;
    float getTotal() const;
  };

  class CoreStats {
  public:
    ComplexStat& getHealth();
    const ComplexStat& getHealth() const;
    ComplexStat& getDefense();
    const ComplexStat& getDefense() const;
    ComplexStat& getDamage();
    const ComplexStat& getDamage() const;
    ComplexStat& getSpeed();
    const ComplexStat& getSpeed() const;
    ComplexStat& getResource();
    const ComplexStat& getResource() const;

    BaseStat& getDamageBonus();
    const BaseStat& getDamageBonus() const;
    BaseStat& getCritDamage();
    const BaseStat& getCritDamage() const;
    BaseStat& getCritChance();
    const BaseStat& getCritChance() const;
    BaseStat& getDamageReduction();
    const BaseStat& getDamageReduction() const;

    float& getCurrentHealth();
    const float& getCurrentHealth() const;
    float& getCurrentResource();
    const float& getCurrentResource() const;

    float getShield() const;
    float getResourceRegen() const;
    void setResourceRegen(float value);
    void setShield(float value);
    void addShield(float value);
    void removeShield(float value);

    bool isInvincible() const;
    void setInvincible(bool value);
    void setIsDead(bool value);
    bool getIsDead() const;
    bool isStunned() const;
    void setStunned(bool value);

    float calculatePhysicalResistance() const;

  private:
    ComplexStat health_;
    float current_health_;
    ComplexStat resource_;
    float current_resource_;
    ComplexStat defense_;
    ComplexStat damage_;
    ComplexStat speed_;
    BaseStat crit_damage_;
    BaseStat crit_chance_;
    BaseStat damage_bonus_;
    BaseStat damage_reduction_;

    float shield_ = 0.0f;
    bool invincible_ = false;
    bool stunned_ = false;
    bool is_dead_ = false;
    float resource_regen_ = 10.0f;
  };
}

#endif
