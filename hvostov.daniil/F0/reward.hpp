#ifndef REWARD_HPP
#define REWARD_HPP

#include <string>
#include <vector>
#include <memory>
#include "hero.hpp"

namespace hvostov {

  class Reward {
  public:
    Reward(const std::string& description);
    virtual ~Reward() = default;
    std::string getDescription() const;
    virtual void apply(Hero& hero) const = 0;

  protected:
    std::string description_;
  };

  class HealthReward : public Reward {
  public:
    HealthReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class ResourceReward : public Reward {
  public:
    ResourceReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class DamageReward : public Reward {
  public:
    DamageReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class DefenseReward : public Reward {
  public:
    DefenseReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class SpeedReward : public Reward {
  public:
    SpeedReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class CritChanceReward : public Reward {
  public:
    CritChanceReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class CritDamageReward : public Reward {
  public:
    CritDamageReward(float amount);
    void apply(Hero& hero) const override;

  private:
    float amount_;
  };

  class SkillLevelReward : public Reward {
  public:
    SkillLevelReward(size_t skill_index, size_t levels);
    void apply(Hero& hero) const override;

  private:
    size_t skill_index_;
    size_t levels_;
  };

  class PercentHealthReward : public Reward {
  public:
    PercentHealthReward(float percent);
    void apply(Hero& hero) const override;

  private:
    float percent_;
  };

  class PercentDamageReward : public Reward {
  public:
    PercentDamageReward(float percent);
    void apply(Hero& hero) const override;

  private:
    float percent_;
  };

  class PercentDefenseReward : public Reward {
  public:
    PercentDefenseReward(float percent);
    void apply(Hero& hero) const override;

  private:
    float percent_;
  };

  class PercentSpeedReward : public Reward {
  public:
    PercentSpeedReward(float percent);
    void apply(Hero& hero) const override;

  private:
    float percent_;
  };

  class UnlockSkillReward : public Reward {
    bool include_ults_;

  public:
    UnlockSkillReward(bool include_ults = true);
    void apply(Hero& hero) const override;
  };

  class MultiSkillLevelReward : public Reward {
  public:
    MultiSkillLevelReward(size_t levels);
    void apply(Hero& hero) const override;

  private:
    size_t levels_;
  };

  class FullHealReward : public Reward {
  public:
    FullHealReward(float pct);
    void apply(Hero& hero) const override;

  private:
    float percent_;
  };

  class RewardFactory {
  public:
    static std::vector< std::unique_ptr< Reward > > generateCommonRewards(int floor);
    static std::vector< std::unique_ptr< Reward > > generateRareRewards(const Hero& hero, int floor);
    static std::vector< std::unique_ptr< Reward > > generateEpicRewards(const Hero& hero, int floor);
    static std::vector< std::unique_ptr< Reward > > generateLegendaryRewards(const Hero& hero, int floor);
  };

  class PercentResourceReward : public Reward {
    float percent_;

  public:
    PercentResourceReward(float percent);
    void apply(Hero& hero) const override;
  };

  class RegenReward : public Reward {
    float amount_;

  public:
    RegenReward(float amount);
    void apply(Hero& hero) const override;
  };
}

#endif
