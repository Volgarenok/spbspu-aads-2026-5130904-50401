#include "help_functions.hpp"
#include <cmath>

int petrov::detail::userStamina(size_t h, size_t w, size_t o)
{
  int hmw = static_cast<int>(h) - static_cast<int>(w);

  int k = 0;
  if (o < 12)
  {
    k = 6;
  }
  else if (o >= 12 && o < 18)
  {
    k = 5;
  }
  else if (o >= 18 && o < 30)
  {
    k = 4;
  }
  else if (o >= 30 && o < 45)
  {
    k = 5;
  }
  else if (o >= 45 && o < 60)
  {
    k = 6;
  }
  else
  {
    k = 7;
  }

  int deviation = std::abs(hmw - 100);
  if (deviation <= 10)
  {
    deviation = 0;
  }
  else
  {
    deviation -= 10;
  }
  int age_penalty = (k - 4) * 50;
  int weight_penalty = deviation * k;
  int stamina = 1000 - age_penalty - weight_penalty;

  return (stamina > 100) ? stamina : 100;
}

int petrov::detail::exStamina(int st, size_t s, size_t r)
{
  double k = static_cast< double >(s * r) / 10;
  return st * k;
}
