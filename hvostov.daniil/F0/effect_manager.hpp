#ifndef EFFECT_MANAGER_HPP
#define EFFECT_MANAGER_HPP

#include <memory>
#include <vector>
#include "effects.hpp"

namespace hvostov {
  class Actor;

  class EffectManager {
  public:
    EffectManager(Actor* owner);
    void addEffect(std::unique_ptr< Effect > effect);
    void update();
    bool hasEffect(EffectType type) const;
    Effect* getEffect(EffectType type) const;
    void removeAllEffects();
    void removeNegativeEffects();

  private:
    Actor* owner_;
    std::vector< std::unique_ptr< Effect > > effects_;
  };

}

#endif
