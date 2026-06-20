#ifndef SKILL_MANAGER_HPP
#define SKILL_MANAGER_HPP

#include <memory>
#include <string>
#include <vector>
#include "skills.hpp"

namespace hvostov {
  class Actor;

  class SkillManager {
  public:
    SkillManager(Actor* owner);

    void addSkill(std::unique_ptr< Skill > skill);
    void addLockedSkill(std::unique_ptr< Skill > skill);
    void unlockSkill(size_t index);
    bool isSkillLocked(size_t index) const;
    std::string getSkillName(size_t index) const;
    float getSkillCost(size_t index) const;
    std::string getSkillDescription(size_t index) const;
    SkillTarget getSkillTargetType(size_t index) const;
    void useSkill(size_t id) const;
    void useSkill(size_t id, Actor* target) const;
    void useSkill(size_t id, const std::vector< Actor* >& targets) const;

    size_t getSkillLevel(size_t index) const;

    void addLevelsToSkill(size_t id, size_t levels);
    size_t getSkillCount() const;

  private:
    Actor* owner_;
    std::vector< std::unique_ptr< Skill > > skills_;
    std::vector< bool > locked_;
  };

}

#endif
