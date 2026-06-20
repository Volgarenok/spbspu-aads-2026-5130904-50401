#include "skill_manager.hpp"
#include <iostream>

hvostov::SkillManager::SkillManager(Actor* owner):
  owner_(owner)
{}

void hvostov::SkillManager::addSkill(std::unique_ptr< Skill > skill)
{
  skills_.push_back(std::move(skill));
  locked_.push_back(false);
}

void hvostov::SkillManager::addLockedSkill(std::unique_ptr< Skill > skill)
{
  skills_.push_back(std::move(skill));
  locked_.push_back(true);
}

void hvostov::SkillManager::unlockSkill(size_t index)
{
  if (index < locked_.size() && locked_[index]) {
    locked_[index] = false;
    std::cout << "🔓 Skill unlocked: " << skills_[index]->getSkillName() << "\n";
  } else if (index >= locked_.size()) {
    std::cout << "Invalid skill index\n";
  } else {
    std::cout << "Skill already unlocked\n";
  }
}

bool hvostov::SkillManager::isSkillLocked(size_t index) const
{
  if (index >= locked_.size())
    return true;
  return locked_[index];
}

std::string hvostov::SkillManager::getSkillName(size_t index) const
{
  if (index < skills_.size() && skills_[index]) {
    return skills_[index]->getSkillName();
  }
  return "Unknown";
}

void hvostov::SkillManager::useSkill(size_t id) const
{
  if (id >= skills_.size()) {
    std::cout << "Skill not found\n";
    return;
  }

  if (locked_[id]) {
    std::cout << "🔒 Skill is locked! Unlock it first.\n";
    return;
  }

  if (!skills_[id]) {
    std::cout << "Skill is null\n";
    return;
  }

  std::cout << id << " <- id\n";
  skills_[id]->processSkill(owner_, owner_);
}

void hvostov::SkillManager::useSkill(size_t id, Actor* target) const
{
  if (id >= skills_.size()) {
    std::cout << "Skill not found\n";
    return;
  }

  if (locked_[id]) {
    std::cout << "🔒 Skill is locked! Unlock it first.\n";
    return;
  }

  if (!skills_[id]) {
    std::cout << "Skill is null\n";
    return;
  }

  std::cout << id << " <- id\n";
  skills_[id]->processSkill(owner_, target);
}

void hvostov::SkillManager::useSkill(size_t id, const std::vector< Actor* >& targets) const
{
  if (id >= skills_.size()) {
    std::cout << "Skill not found\n";
    return;
  }

  if (locked_[id]) {
    std::cout << "🔒 Skill is locked! Unlock it first.\n";
    return;
  }

  if (!skills_[id]) {
    std::cout << "Skill is null\n";
    return;
  }

  skills_[id]->processSkill(owner_, targets);
}

void hvostov::SkillManager::addLevelsToSkill(size_t id, size_t levels)
{
  if (id >= skills_.size()) {
    std::cout << "Skill not found\n";
    return;
  }

  if (!skills_[id]) {
    std::cout << "Skill is null\n";
    return;
  }

  skills_[id]->addSkillLevel(levels);
  std::cout << "Skill " << skills_[id]->getSkillName() << " leveled up to " << skills_[id]->getSkillLevel() << "\n";
}

size_t hvostov::SkillManager::getSkillCount() const
{
  return skills_.size();
}

float hvostov::SkillManager::getSkillCost(size_t index) const
{
  if (index >= skills_.size() || !skills_[index])
    return 0.0f;
  return skills_[index]->getResourceCost();
}

std::string hvostov::SkillManager::getSkillDescription(size_t index) const
{
  if (index >= skills_.size() || !skills_[index])
    return "";
  return skills_[index]->getDescription();
}

hvostov::SkillTarget hvostov::SkillManager::getSkillTargetType(size_t index) const
{
  if (index >= skills_.size() || !skills_[index])
    return SkillTarget::Enemy;
  return skills_[index]->getTargetType();
}

size_t hvostov::SkillManager::getSkillLevel(size_t index) const
{
  if (index >= skills_.size() || !skills_[index])
    return 0;
  return skills_[index]->getSkillLevel();
}
