#ifndef BATTLE_SYSTEM_HPP
#define BATTLE_SYSTEM_HPP

#include <vector>
#include "actor.hpp"
#include "enemy.hpp"

namespace hvostov {

  class BattleSystem {
  private:
    std::vector< Actor* > heroes_;
    std::vector< Actor* > enemies_;
    std::vector< Actor* > round_queue_;
    size_t current_turn_ = 0;
    bool battle_over_ = false;

    void buildTurnQueue(size_t preview_count = 5);
    void advanceTurn();
    void buildRound();
    void printHero(std::stringstream& ss, size_t row) const;
    void printEnemy(std::stringstream& ss, size_t row) const;
    void regenMana();

  public:
    BattleSystem() = default;

    void addHero(Actor* hero);
    void addEnemy(Enemy* enemy);
    void startBattle();
    void endBattle();
    Actor* currentActor();

    size_t currentTurn() const;
    bool isBattleOver() const;
    bool heroesWon() const;
    bool heroesLost() const;

    std::string showTurnQueue(size_t count = 5) const;
    std::string showBattleStatus() const;

    void executeHeroAction(size_t skill_index, size_t target_index, SkillTarget target_type);
    void executeEnemyAction();

    std::vector< Actor* > getAliveHeroes() const;
    std::vector< Actor* > getAliveEnemies() const;

    Actor* findBestTargetForEnemy(Actor* enemy) const;
    size_t chooseBestSkillForEnemy(Actor* enemy) const;
  };

}

#endif
