#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "actor.hpp"

namespace hvostov {

  class Enemy : public Actor {
  public:
    Enemy(const ActorConfig& config);
    virtual ~Enemy() = default;
    void die() override;
    virtual std::string getType() const = 0;

    virtual std::string getName() const override;

    void setName(const std::string& name);
    size_t getStage() const;
    void setStage(size_t stage);

  protected:
    std::string name_;
    size_t stage_ = 1;
  };

  class Goblin : public Enemy {
  public:
    Goblin(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Wolf : public Enemy {
  public:
    Wolf(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Skeleton : public Enemy {
  public:
    Skeleton(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Slime : public Enemy {
  public:
    Slime(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Bat : public Enemy {
  public:
    Bat(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Spider : public Enemy {
  public:
    Spider(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Zombie : public Enemy {
  public:
    Zombie(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Cultist : public Enemy {
  public:
    Cultist(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Bandit : public Enemy {
  public:
    Bandit(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Imp : public Enemy {
  public:
    Imp(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Harpy : public Enemy {
  public:
    Harpy(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Mummy : public Enemy {
  public:
    Mummy(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class OrcWarrior : public Enemy {
  public:
    OrcWarrior(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class DarkElf : public Enemy {
  public:
    DarkElf(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class StoneGolem : public Enemy {
  public:
    StoneGolem(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class FireElemental : public Enemy {
  public:
    FireElemental(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class IceElemental : public Enemy {
  public:
    IceElemental(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Thunderbird : public Enemy {
  public:
    Thunderbird(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class ShadowAssassin : public Enemy {
  public:
    ShadowAssassin(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class AncientTreant : public Enemy {
  public:
    AncientTreant(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class TrollKing : public Enemy {
  public:
    TrollKing(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Dragon : public Enemy {
  public:
    Dragon(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

  class Lich : public Enemy {
  public:
    Lich(size_t stage = 1);
    void setupSkills();
    std::string getType() const override;
  };

}

#endif
