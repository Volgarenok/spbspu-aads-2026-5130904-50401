#include "random.hpp"
#include <chrono>
#include <random>

static std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());

int hvostov::Random::getInt(int min, int max)
{
  std::uniform_int_distribution< int > dist(min, max);
  return dist(generator);
}

float hvostov::Random::getFloat(float min, float max)
{
  std::uniform_real_distribution< float > dist(min, max);
  return dist(generator);
}
