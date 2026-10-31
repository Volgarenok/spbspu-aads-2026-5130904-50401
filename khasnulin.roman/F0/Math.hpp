#ifndef MATH_HPP
#define MATH_HPP

#include "Entities.hpp"
namespace khasnulin
{
  // возвращает длину вектора
  double vecModule(const Latent &vec) noexcept;

  // скалярное произведение двух векторов
  double scalarProduct(const Latent &v1, const Latent &v2) noexcept;

  // косинус угла между векторами от [-1; 1]. Если один из векторов нулевой -- возвращает 0.
  double cosine(const Latent &v1, const Latent &v2) noexcept;

  // Сходство Жаккара для двух наборов тегов. Возвращает [0; 1], если одно из переданных множеств пустое --
  // возвращает 0.
  double jaccard(const TagSet &t1, const TagSet &t2) noexcept;

  // если val выходит за low или high -- возвращает val приведенный к границе. low <= high контракт
  double clip(double val, double low, double high) noexcept;

  // переводит val к интервалу между newLow и newHigh вместо oldLow и oldHigh. oldLow < oldHigh и newLow <
  // newHigh контракт и oldLow <= val <= oldHigh.
  double toInterval(double val, double oldLow, double oldHigh, double newLow, double newHigh) noexcept;
}

#endif
