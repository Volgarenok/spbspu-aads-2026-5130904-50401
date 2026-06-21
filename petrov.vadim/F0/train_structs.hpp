#ifndef TRAIN_STRUCTS_HPP
#define TRAIN_STRUCTS_HPP

#include <cstddef>
#include <string>
#include "kuku_hash_table.hpp"
#include "hash_func.hpp"

namespace petrov
{
  struct Variant
  {
    int cost_;
    size_t sets_;
    size_t reps_;
    int value_;
  };

  struct ExerciseGroup
  {
    std::string name_;
    std::string mg_;
    topit::Vector< Variant > variants_;
  };

  struct Exercise
  {
    std::string muscleGroup_;
    int stamina_;
  };

  struct UExercise: Exercise
  {
    size_t sets_;
    size_t reps_;

    UExercise() = default;
    UExercise(const std::string& mg, int st, size_t s, size_t r):
      Exercise{mg, st},
      sets_(s),
      reps_(r)
    {}
  };

  struct User
  {
    size_t height_;
    size_t weight_;
    size_t old_;
    int stamina_;

    using train_ex_t = KukuHashTable< UExercise, std::string, KKHash< std::string >, Equal< std::string > >;
    KukuHashTable< train_ex_t, std::string, KKHash< std::string >, Equal< std::string > > plans_;
  };
}

#endif
