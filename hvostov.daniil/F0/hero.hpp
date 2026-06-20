#ifndef HERO_HPP
#define HERO_HPP

#include <string>
#include "actor.hpp"
#include "actor_config.hpp"
#include "types.hpp"

namespace hvostov {

  class Hero : public Actor {
  public:
    Hero(const ActorConfig& config);
    virtual ~Hero() = default;

    void die() override;
    HeroClass getHeroClass() const;

    virtual void setupSkills() = 0;
    virtual std::string getClassName() const = 0;
    std::string getName() const override;

  protected:
    HeroClass hero_class_;
  };

  class Knight : public Hero {
  public:
    Knight();
    void setupSkills() override;
    std::string getClassName() const override;
  };

  class Mage : public Hero {
  public:
    Mage();
    void setupSkills() override;
    std::string getClassName() const override;
  };

  class Assassin : public Hero {
  public:
    Assassin();
    void setupSkills() override;
    std::string getClassName() const override;
  };

  class Paladin : public Hero {
  public:
    Paladin();
    void setupSkills() override;
    std::string getClassName() const override;
  };

  class Priest : public Hero {
  public:
    Priest();
    void setupSkills() override;
    std::string getClassName() const override;
  };

  class Archer : public Hero {
  public:
    Archer();
    void setupSkills() override;
    std::string getClassName() const override;
  };
}

#endif
