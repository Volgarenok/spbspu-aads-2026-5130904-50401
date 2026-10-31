#include "Model.hpp"
#include "Entities.hpp"
#include "Math.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace
{
  constexpr std::uint64_t primeFNV1a = 1099511628211;

  constexpr std::uint64_t splitmixIncrement = 0x9E3779B97F4A7C15;

  constexpr std::uint64_t splitmixMultiplier1 = 0xBF58476D1CE4E5B9;
  constexpr std::uint64_t splitmixMultiplier2 = 0x94D049BB133111EB;

  constexpr unsigned char userPrefix = 'u';
  constexpr unsigned char articlePrefix = 'a';

  constexpr double latentInitAmplitude = 0.1;

  khasnulin::Latent latentFromSeed(std::uint64_t seed)
  {
    using namespace khasnulin;
    Latent latent;
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      std::uint64_t bits = splitmix64(seed);
      double unit = unitFromBits(bits);
      latent.v[i] = toInterval(unit, 0.0, 1.0, -latentInitAmplitude, latentInitAmplitude);
    }
    return latent;
  }
}

std::uint64_t khasnulin::fnv1aStep(std::uint64_t hash, unsigned char byte) noexcept
{
  return (hash ^ byte) * primeFNV1a;
}

std::uint64_t khasnulin::fnv1aHash(const std::string &str, std::uint64_t hash) noexcept
{
  std::uint64_t result = hash;
  for (unsigned char ch : str)
  {
    result = fnv1aStep(result, ch);
  }
  return result;
}

std::uint64_t khasnulin::splitmix64(std::uint64_t &state) noexcept
{
  state = state + splitmixIncrement;
  std::uint64_t result = state;
  result = (result ^ (result >> 30)) * splitmixMultiplier1;
  result = (result ^ (result >> 27)) * splitmixMultiplier2;
  result = result ^ (result >> 31);
  return result;
}

double khasnulin::unitFromBits(std::uint64_t bits) noexcept
{
  bits = bits >> 11;
  return static_cast< double >(bits) / (2ULL << 52);
}

khasnulin::Latent khasnulin::initialLatent(const UserName &name)
{
  std::uint64_t hash = fnv1aStep(offsetBasisFNV1a, userPrefix);
  hash = fnv1aHash(name.value(), hash);

  return latentFromSeed(hash);
}

khasnulin::Latent khasnulin::initialLatent(const ArticleName &name)
{
  std::uint64_t hash = fnv1aStep(offsetBasisFNV1a, articlePrefix);
  hash = fnv1aHash(name.value(), hash);

  return latentFromSeed(hash);
}

khasnulin::Learner::Learner(double learningRate, double regularization, std::size_t stepCount) noexcept:
    learningRate_(learningRate),
    regularization_(regularization),
    steps_(stepCount)
{
  assert(learningRate > 0);
  assert(regularization >= 0);
  assert(stepCount > 0);
}

void khasnulin::Learner::sgdStep(Latent &user, Latent &article, double target) const noexcept
{
  double error = target - scalarProduct(user, article);
  Latent tempUser = user;

  for (std::size_t i = 0; i < latentDimensionCount; ++i)
  {
    user.v[i] = user.v[i] + learningRate_ * (error * article.v[i] - regularization_ * tempUser.v[i]);
  }

  for (std::size_t i = 0; i < latentDimensionCount; ++i)
  {
    article.v[i] = article.v[i] + learningRate_ * (error * tempUser.v[i] - regularization_ * article.v[i]);
  }
}

void khasnulin::Learner::operator()(Latent &user, Latent &article, const Rating &rating) const noexcept
{
  double target = toInterval(rating.value(), static_cast< double >(Rating::minRating),
                             static_cast< double >(Rating::maxRating), targetScaleMin, targetScaleMax);
  for (std::size_t i = 0; i < steps_; ++i)
  {
    sgdStep(user, article, target);
  }
}

khasnulin::Scorer::Scorer(double shrinkage) noexcept:
    shrinkage_(shrinkage)
{
  assert(shrinkage > 0);
}

// доверие к сигналу векторов при ratingCount оценках, [0, 1)
double khasnulin::Scorer::confidence(std::size_t ratingCount) const noexcept
{
  return static_cast< double >(ratingCount) / (static_cast< double >(ratingCount) + shrinkage_);
}

// коэффициент рекомендуемости  статьи для этого пользователя [0, 1] только на основе латентных векторов
double khasnulin::Scorer::mfSignal(const Latent &user, const Latent &article) noexcept
{
  double koef = scalarProduct(user, article);
  koef = clip(koef, targetScaleMin, targetScaleMax);
  return toInterval(koef, targetScaleMin, targetScaleMax, scoreKoefScaleMin, scoreKoefScaleMax);
}

// возвращает коэффициент [0; 1] -- то, насколько статья кандидат подходит пользователю
double khasnulin::Scorer::userArticleScore(const Latent &user, const TagSet &preferences,
                                           std::size_t userRatingCount, const Latent &article,
                                           const TagSet &tags) const noexcept
{
  double conf = confidence(userRatingCount);
  return (mfSignal(user, article) * conf) + ((1 - conf) * jaccard(preferences, tags));
}

// возвращает коэффициент [0; 1] -- то, насколько две статьи схожи
double khasnulin::Scorer::articleArticleScore(const Latent &art1, const TagSet &tags1,
                                              std::size_t art1RatingsCount, const Latent &art2,
                                              const TagSet &tags2,
                                              std::size_t art2RatingsCount) const noexcept
{
  std::size_t minCount = art1RatingsCount < art2RatingsCount ? art1RatingsCount : art2RatingsCount;
  double conf = confidence(minCount);
  double normilizeCosine = toInterval(cosine(art1, art2), -1.0, 1.0, scoreKoefScaleMin, scoreKoefScaleMax);
  return (normilizeCosine * conf) + ((1 - conf) * jaccard(tags1, tags2));
}
