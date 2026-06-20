#include "dungeon_admin.hpp"
#include <iostream>
#include <cstdio>
#include "random.hpp"

hvostov::Dungeon* hvostov::DungeonAdmin::activeDungeon()
{
  return active_dungeon_.get();
}

const hvostov::Dungeon* hvostov::DungeonAdmin::activeDungeon() const
{
  return active_dungeon_.get();
}

bool hvostov::DungeonAdmin::hasActiveDungeon() const
{
  return active_dungeon_ != nullptr;
}

std::string hvostov::DungeonAdmin::activeDungeonName() const
{
  return active_dungeon_ ? active_dungeon_->name() : "";
}

bool hvostov::DungeonAdmin::createDungeon(const std::string& name, size_t floors)
{
  (void)floors;
  auto dungeon = std::make_unique< hvostov::Dungeon >(name);
  active_dungeon_ = std::move(dungeon);
  return true;
}

bool hvostov::DungeonAdmin::generateDungeon(const std::string& name, size_t floors)
{
  auto dungeon = std::make_unique< hvostov::Dungeon >(name);

  std::vector< std::string > common = {"Goblin", "Wolf", "Skeleton", "Slime", "Bat", "Spider", "Zombie"};
  std::vector< std::string > medium = {"Cultist", "Bandit", "Imp", "Harpy", "Mummy"};
  std::vector< std::string > elite = {"Orc Warrior",   "Dark Elf",    "Stone Golem",     "Fire Elemental",
                                      "Ice Elemental", "Thunderbird", "Shadow Assassin", "Ancient Treant"};
  std::vector< std::string > boss = {"Troll King", "Dragon", "Lich"};

  for (size_t i = 0; i < floors; i++) {
    dungeon->addFloor();
    int paths = hvostov::Random::getInt(2, 3);
    for (int j = 0; j < paths; j++) {
      dungeon->addRoom(i, "Path " + std::to_string(j + 1));

      int monster_count = hvostov::Random::getInt(1, 3);
      bool has_elite = false;

      for (int k = 0; k < monster_count; k++) {
        std::string type;
        size_t level = i + 1;

        if (i == floors - 1 && k == 0) {
          type = boss[hvostov::Random::getInt(0, boss.size() - 1)];
          level = i + 2;
        } else if (i >= floors - 3) {
          if (hvostov::Random::getFloat(0, 1) < 0.4f) {
            type = elite[hvostov::Random::getInt(0, elite.size() - 1)];
            has_elite = true;
          } else {
            type = medium[hvostov::Random::getInt(0, medium.size() - 1)];
          }
        } else if (i >= floors / 2) {
          if (hvostov::Random::getFloat(0, 1) < 0.3f) {
            type = elite[hvostov::Random::getInt(0, elite.size() - 1)];
            has_elite = true;
          } else if (hvostov::Random::getFloat(0, 1) < 0.5f) {
            type = medium[hvostov::Random::getInt(0, medium.size() - 1)];
          } else {
            type = common[hvostov::Random::getInt(0, common.size() - 1)];
          }
        } else {
          if (hvostov::Random::getFloat(0, 1) < 0.2f) {
            type = medium[hvostov::Random::getInt(0, medium.size() - 1)];
          } else {
            type = common[hvostov::Random::getInt(0, common.size() - 1)];
          }
        }

        dungeon->spawnMonster(i, j, type, level);
      }

      hvostov::RewardQuality rq;
      if (i == floors - 1) {
        rq = hvostov::RewardQuality::Legendary;
      } else if (i >= floors - 3) {
        rq = hvostov::RewardQuality::Epic;
      } else if (has_elite) {
        rq = hvostov::RewardQuality::Rare;
      } else if (monster_count >= 3) {
        rq = hvostov::RewardQuality::Rare;
      } else if (monster_count >= 2) {
        rq = hvostov::RewardQuality::Rare;
      } else {
        rq = hvostov::RewardQuality::Common;
      }

      dungeon->setRewardQuality(i, j, rq);
    }
  }

  dungeon->save(name);
  active_dungeon_ = std::move(dungeon);
  return true;
}

bool hvostov::DungeonAdmin::deleteDungeon(const std::string& name)
{
  if (active_dungeon_ && active_dungeon_->name() == name) {
    active_dungeon_.reset();
  }

  std::string filepath = "dungeons/" + name + ".dng";
  if (std::remove(filepath.c_str()) == 0) {
    return true;
  }

  std::cout << "<INVALID COMMAND> — file not found: " << filepath << "\n";
  return false;
}

bool hvostov::DungeonAdmin::loadDungeon(const std::string& name)
{
  auto dungeon = std::make_unique< hvostov::Dungeon >();
  if (!dungeon->load(name)) {
    return false;
  }
  active_dungeon_ = std::move(dungeon);
  return true;
}

bool hvostov::DungeonAdmin::saveDungeon(const std::string& name)
{
  if (!active_dungeon_)
    return false;
  return active_dungeon_->save(name);
}
