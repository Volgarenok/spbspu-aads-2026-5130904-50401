#ifndef DUNGEON_ADMIN_HPP
#define DUNGEON_ADMIN_HPP

#include <string>
#include <memory>
#include "dungeon.hpp"

namespace hvostov {

  class DungeonAdmin {
  private:
    std::unique_ptr< Dungeon > active_dungeon_;

  public:
    DungeonAdmin() = default;

    bool createDungeon(const std::string& name, size_t floors);
    bool generateDungeon(const std::string& name, size_t floors);
    bool deleteDungeon(const std::string& name);
    bool loadDungeon(const std::string& name);
    bool saveDungeon(const std::string& name);

    Dungeon* activeDungeon();
    const Dungeon* activeDungeon() const;
    bool hasActiveDungeon() const;
    std::string activeDungeonName() const;
  };

}

#endif
