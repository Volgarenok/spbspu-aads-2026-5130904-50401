#ifndef DAMAGE_MANAGER_HPP
#define DAMAGE_MANAGER_HPP

#include "skills.hpp"

namespace hvostov {

  class Actor;

  class DamageManager {
  public:
    DamageManager(Actor* owner);

    float calculateOutputDamage(AttackSkill* skill);
    float calculateInputDamage(float attack);
    bool handleAttack(float attack, Actor* attacker);
    void takeDamage(float damage);

    void heal(float amount);
    void setManaShield(float value);
    void revive(float percent);

  private:
    Actor* owner_;
  };

}

#endif
