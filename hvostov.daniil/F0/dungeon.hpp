#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>
#include <string>
#include <memory>
#include "reward.hpp"

namespace hvostov {

  class Enemy;
  class Hero;

  struct DungeonRoom {
    size_t room_id;
    std::string description;
    std::vector< std::unique_ptr< Enemy > > monsters;

    RewardQuality reward_quality;

    DungeonRoom();
    DungeonRoom(size_t id, const std::string& desc);
    ~DungeonRoom();

    DungeonRoom(DungeonRoom&&) = default;
    DungeonRoom& operator=(DungeonRoom&&) = default;
    DungeonRoom(const DungeonRoom&) = delete;
    DungeonRoom& operator=(const DungeonRoom&) = delete;

    bool hasMonsters() const;
    size_t getMonsterCount() const;
  };

  struct DungeonFloor {
    size_t floor_number;
    std::vector< DungeonRoom > rooms;

    DungeonFloor();
    DungeonFloor(size_t num);

    DungeonFloor(DungeonFloor&&) = default;
    DungeonFloor& operator=(DungeonFloor&&) = default;
    DungeonFloor(const DungeonFloor&) = delete;
    DungeonFloor& operator=(const DungeonFloor&) = delete;

    size_t getRoomCount() const;
    DungeonRoom* room(size_t index);
    const DungeonRoom* room(size_t index) const;
    size_t addRoom(const std::string& desc = "");
    bool removeRoom(size_t index);
  };

  class Dungeon {
  private:
    std::string name_;
    std::vector< DungeonFloor > floors_;
    size_t current_floor_ = 0;
    size_t current_room_ = 0;
    bool completed_ = false;
    bool in_progress_ = false;

  public:
    Dungeon() = default;
    explicit Dungeon(const std::string& name);

    size_t addFloor();
    size_t insertFloor(size_t index);
    bool removeFloor(size_t index);
    DungeonFloor* floor(size_t index);
    const DungeonFloor* floor(size_t index) const;
    size_t floorCount() const;

    size_t addRoom(size_t floor_index, const std::string& desc = "");
    bool removeRoom(size_t floor_index, size_t room_index);

    void spawnMonster(size_t floor_index, size_t room_index, const std::string& type, size_t level);
    void removeMonster(size_t floor_index, size_t room_index, size_t monster_index);

    void setRewardQuality(size_t floor_index, size_t room_index, RewardQuality q);
    RewardQuality rewardQuality(size_t floor_index, size_t room_index) const;
    std::vector< std::unique_ptr< Reward > > generateRewards(Hero& hero) const;

    void start();
    bool chooseRoom(size_t room_index);
    void clearRoom();
    bool nextFloor();

    const std::string& name() const;
    void setName(const std::string& n);
    size_t currentFloorIndex() const;
    size_t currentRoomIndex() const;
    bool isCompleted() const;
    bool isInProgress() const;
    bool isLastFloor() const;

    const DungeonRoom* currentRoom() const;
    const DungeonFloor* currentFloor() const;

    std::string showRoomChoice() const;
    std::string showRoom() const;
    std::string showProgress() const;
    std::string showFull() const;

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);
  };

}

#endif
