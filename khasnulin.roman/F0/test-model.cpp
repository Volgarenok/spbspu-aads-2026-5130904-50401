#include "Math.hpp"
#include "Model.hpp"
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>

using namespace khasnulin;

namespace
{
  const double tolerance = 1e-12;

  bool isClose(double lhs, double rhs, double eps = tolerance)
  {
    return std::fabs(lhs - rhs) < eps;
  }

  bool sameLatent(const Latent &lhs, const Latent &rhs)
  {
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      if (lhs.v[i] != rhs.v[i])
      {
        return false;
      }
    }
    return true;
  }

  bool withinAmplitude(const Latent &latent)
  {
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      if (latent.v[i] < -0.1 || latent.v[i] > 0.1)
      {
        return false;
      }
    }
    return true;
  }

  bool matchesReference(const Latent &latent, const double (&reference)[8])
  {
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      if (!isClose(latent.v[i], reference[i]))
      {
        return false;
      }
    }
    return true;
  }

  const double convergenceTolerance = 1e-9;

  bool closeLatent(const Latent &lhs, const Latent &rhs)
  {
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      if (!isClose(lhs.v[i], rhs.v[i]))
      {
        return false;
      }
    }
    return true;
  }

  Latent fillLatent(double value)
  {
    Latent latent{};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      latent.v[i] = value;
    }
    return latent;
  }

  Latent firstComponentLatent(double value)
  {
    Latent latent{};
    latent.v[0] = value;
    return latent;
  }

  Latent userSample()
  {
    Latent latent{};
    const double values[8] = {0.1, -0.05, 0.08, 0.02, -0.03, 0.06, 0.04, -0.07};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      latent.v[i] = values[i];
    }
    return latent;
  }

  Latent articleSample()
  {
    Latent latent{};
    const double values[8] = {0.05, 0.09, -0.02, 0.07, 0.01, -0.04, 0.08, 0.03};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      latent.v[i] = values[i];
    }
    return latent;
  }

  double predictionError(const Latent &user, const Latent &article, double target)
  {
    return std::fabs(target - scalarProduct(user, article));
  }
}

BOOST_AUTO_TEST_SUITE(fnv1a_tests)

BOOST_AUTO_TEST_CASE(empty_string_gives_offset_basis)
{
  BOOST_CHECK(fnv1aHash("") == offsetBasisFNV1a);
  BOOST_CHECK(offsetBasisFNV1a == 0xcbf29ce484222325ULL);
}

BOOST_AUTO_TEST_CASE(matches_reference_values)
{
  BOOST_CHECK(fnv1aHash("a") == 0xaf63dc4c8601ec8cULL);
  BOOST_CHECK(fnv1aHash("foobar") == 0x85944171f73967e8ULL);
}

BOOST_AUTO_TEST_CASE(step_matches_single_byte_hash)
{
  BOOST_CHECK(fnv1aStep(offsetBasisFNV1a, 'a') == fnv1aHash("a"));
}

BOOST_AUTO_TEST_CASE(continues_from_given_hash)
{
  BOOST_CHECK(fnv1aHash("bar", fnv1aHash("foo")) == fnv1aHash("foobar"));
}

BOOST_AUTO_TEST_CASE(prefix_step_then_name_equals_prefixed_string)
{
  BOOST_CHECK(fnv1aHash("alice", fnv1aStep(offsetBasisFNV1a, 'u')) == fnv1aHash("ualice"));
  BOOST_CHECK(fnv1aHash("alice", fnv1aStep(offsetBasisFNV1a, 'u')) == 0x2341ac0552eb9658ULL);
}

BOOST_AUTO_TEST_CASE(bytes_above_127_are_hashed_as_unsigned)
{
  BOOST_CHECK(fnv1aHash(std::string(1, '\xFF')) == 0xaf64724c8602eb6eULL);
  BOOST_CHECK(fnv1aStep(offsetBasisFNV1a, 0xFF) == 0xaf64724c8602eb6eULL);
  BOOST_CHECK(fnv1aHash("\xD0\x96") == 0x0af5f207b744ebbbULL);
}

BOOST_AUTO_TEST_CASE(order_of_bytes_matters)
{
  BOOST_CHECK(fnv1aHash("ab") != fnv1aHash("ba"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(splitmix64_tests)

BOOST_AUTO_TEST_CASE(matches_reference_sequence_from_zero)
{
  std::uint64_t state = 0;
  BOOST_CHECK(splitmix64(state) == 0xe220a8397b1dcdafULL);
  BOOST_CHECK(splitmix64(state) == 0x6e789e6aa1b965f4ULL);
  BOOST_CHECK(splitmix64(state) == 0x06c45d188009454fULL);
}

BOOST_AUTO_TEST_CASE(advances_state_by_gamma)
{
  std::uint64_t state = 0;
  splitmix64(state);
  BOOST_CHECK(state == 0x9e3779b97f4a7c15ULL);
  splitmix64(state);
  BOOST_CHECK(state == 0x3c6ef372fe94f82aULL);
}

BOOST_AUTO_TEST_CASE(same_state_gives_same_value)
{
  std::uint64_t first = 12345;
  std::uint64_t second = 12345;
  BOOST_CHECK(splitmix64(first) == splitmix64(second));
  BOOST_CHECK(first == second);
}

BOOST_AUTO_TEST_CASE(consecutive_values_differ)
{
  std::uint64_t state = 42;
  std::uint64_t first = splitmix64(state);
  std::uint64_t second = splitmix64(state);
  BOOST_CHECK(first != second);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(unit_from_bits_tests)

BOOST_AUTO_TEST_CASE(zero_gives_zero)
{
  BOOST_CHECK(unitFromBits(0) == 0.0);
}

BOOST_AUTO_TEST_CASE(maximum_stays_strictly_below_one)
{
  double value = unitFromBits(std::numeric_limits< std::uint64_t >::max());
  BOOST_CHECK(value < 1.0);
  BOOST_CHECK(value == 1.0 - std::ldexp(1.0, -53));
}

BOOST_AUTO_TEST_CASE(highest_bit_gives_half)
{
  BOOST_CHECK(unitFromBits(1ULL << 63) == 0.5);
}

BOOST_AUTO_TEST_CASE(low_eleven_bits_are_ignored)
{
  BOOST_CHECK(unitFromBits(0x7FFULL) == 0.0);
  BOOST_CHECK(unitFromBits((1ULL << 63) | 0x7FFULL) == 0.5);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(initial_latent_tests)

BOOST_AUTO_TEST_CASE(user_matches_reference_vector)
{
  const double reference[8] = {0.058368528226680566, -0.07039031273869019, 0.0709408206449437,
                               -0.08524817123965558, 0.02792848819596777,  -0.062912342008928,
                               -0.08431351853723418, 0.06711034768609475};
  BOOST_CHECK(matchesReference(initialLatent(UserName("alice")), reference));
}

BOOST_AUTO_TEST_CASE(article_matches_reference_vector)
{
  const double reference[8] = {0.09520513202736208,  0.036143239716413034, -0.09761525777759723,
                               -0.05730237065542494, -0.06833917649221177, -0.08180708017777638,
                               0.023130054692718532, -0.07400048912366558};
  BOOST_CHECK(matchesReference(initialLatent(ArticleName("alice")), reference));
}

BOOST_AUTO_TEST_CASE(is_deterministic)
{
  BOOST_CHECK(sameLatent(initialLatent(UserName("bob")), initialLatent(UserName("bob"))));
  BOOST_CHECK(sameLatent(initialLatent(ArticleName("bob")), initialLatent(ArticleName("bob"))));
}

BOOST_AUTO_TEST_CASE(user_and_article_with_same_name_differ)
{
  BOOST_CHECK(!sameLatent(initialLatent(UserName("rust")), initialLatent(ArticleName("rust"))));
}

BOOST_AUTO_TEST_CASE(similar_names_give_different_vectors)
{
  BOOST_CHECK(!sameLatent(initialLatent(UserName("alice")), initialLatent(UserName("alicf"))));
  BOOST_CHECK(!sameLatent(initialLatent(UserName("alice")), initialLatent(UserName("Alice"))));
}

BOOST_AUTO_TEST_CASE(components_stay_within_amplitude)
{
  const std::string letters = "abcdefghijklmnopqrstuvwxyz";
  for (char letter : letters)
  {
    std::string name = std::string("item") + letter;
    BOOST_CHECK(withinAmplitude(initialLatent(UserName(name))));
    BOOST_CHECK(withinAmplitude(initialLatent(ArticleName(name))));
  }
}

BOOST_AUTO_TEST_CASE(vector_is_not_zero)
{
  BOOST_CHECK(vecModule(initialLatent(UserName("alice"))) > 0.0);
  BOOST_CHECK(vecModule(initialLatent(ArticleName("alice"))) > 0.0);
}

BOOST_AUTO_TEST_CASE(components_use_whole_range_with_both_signs)
{
  const std::string letters = "abcdefghijklmnopqrstuvwxyz";
  double minimum = 1.0;
  double maximum = -1.0;
  std::size_t negativeCount = 0;
  std::size_t totalCount = 0;
  for (char letter : letters)
  {
    Latent latent = initialLatent(UserName(std::string("item") + letter));
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      minimum = std::fmin(minimum, latent.v[i]);
      maximum = std::fmax(maximum, latent.v[i]);
      if (latent.v[i] < 0.0)
      {
        ++negativeCount;
      }
      ++totalCount;
    }
  }
  BOOST_CHECK(minimum < -0.09);
  BOOST_CHECK(maximum > 0.09);
  BOOST_CHECK(negativeCount > totalCount / 3);
  BOOST_CHECK(negativeCount < 2 * totalCount / 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(learner_defaults_tests)

BOOST_AUTO_TEST_CASE(default_constants_match_design)
{
  BOOST_CHECK(Learner::defaultLearningRate == 0.1);
  BOOST_CHECK(Learner::defaultRegularization == 0.02);
  BOOST_CHECK(Learner::defaultStepCount == 10);
}

BOOST_AUTO_TEST_CASE(default_learner_equals_explicit_defaults)
{
  Latent firstUser = userSample();
  Latent firstArticle = articleSample();
  Latent secondUser = userSample();
  Latent secondArticle = articleSample();
  Learner()(firstUser, firstArticle, Rating(8));
  Learner(0.1, 0.02, 10)(secondUser, secondArticle, Rating(8));
  BOOST_CHECK(sameLatent(firstUser, secondUser));
  BOOST_CHECK(sameLatent(firstArticle, secondArticle));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(sgd_step_tests)

BOOST_AUTO_TEST_CASE(uses_old_values_for_both_vectors)
{
  Latent user = firstComponentLatent(0.5);
  Latent article = firstComponentLatent(0.4);
  Learner(0.1, 0.0, 1).sgdStep(user, article, 1.0);
  BOOST_CHECK(isClose(user.v[0], 0.532));
  BOOST_CHECK(isClose(article.v[0], 0.44));
}

BOOST_AUTO_TEST_CASE(updates_all_dimensions)
{
  Latent user = fillLatent(0.1);
  Latent article = fillLatent(0.2);
  Learner(0.1, 0.02, 1).sgdStep(user, article, 1.0);
  BOOST_CHECK(closeLatent(user, fillLatent(0.1166)));
  BOOST_CHECK(closeLatent(article, fillLatent(0.208)));
}

BOOST_AUTO_TEST_CASE(regularization_alone_shrinks_vectors)
{
  Latent user = firstComponentLatent(0.5);
  Latent article = firstComponentLatent(0.4);
  Learner(0.1, 0.02, 1).sgdStep(user, article, 0.2);
  BOOST_CHECK(isClose(user.v[0], 0.5 * (1.0 - 0.1 * 0.02)));
  BOOST_CHECK(isClose(article.v[0], 0.4 * (1.0 - 0.1 * 0.02)));
}

BOOST_AUTO_TEST_CASE(zero_error_without_regularization_changes_nothing)
{
  Latent user = firstComponentLatent(0.5);
  Latent article = firstComponentLatent(0.4);
  Learner(0.1, 0.0, 1).sgdStep(user, article, 0.2);
  BOOST_CHECK(isClose(user.v[0], 0.5));
  BOOST_CHECK(isClose(article.v[0], 0.4));
}

BOOST_AUTO_TEST_CASE(is_symmetric_in_user_and_article)
{
  Latent firstUser = userSample();
  Latent firstArticle = articleSample();
  Latent secondUser = articleSample();
  Latent secondArticle = userSample();
  Learner learner(0.1, 0.02, 1);
  learner.sgdStep(firstUser, firstArticle, 0.6);
  learner.sgdStep(secondUser, secondArticle, 0.6);
  BOOST_CHECK(sameLatent(firstUser, secondArticle));
  BOOST_CHECK(sameLatent(firstArticle, secondUser));
}

BOOST_AUTO_TEST_CASE(zero_vectors_never_learn)
{
  Latent user{};
  Latent article{};
  Learner(0.1, 0.02, 1).sgdStep(user, article, 1.0);
  BOOST_CHECK(sameLatent(user, Latent{}));
  BOOST_CHECK(sameLatent(article, Latent{}));
}

BOOST_AUTO_TEST_CASE(positive_target_pulls_prediction_up)
{
  Latent user = userSample();
  Latent article = articleSample();
  double before = scalarProduct(user, article);
  Learner(0.1, 0.02, 1).sgdStep(user, article, 1.0);
  BOOST_CHECK(scalarProduct(user, article) > before);
}

BOOST_AUTO_TEST_CASE(negative_target_pulls_prediction_down)
{
  Latent user = userSample();
  Latent article = articleSample();
  double before = scalarProduct(user, article);
  Learner(0.1, 0.02, 1).sgdStep(user, article, -1.0);
  BOOST_CHECK(scalarProduct(user, article) < before);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(learner_call_tests)

BOOST_AUTO_TEST_CASE(single_step_equals_sgd_step_with_converted_target)
{
  Learner learner(0.1, 0.02, 1);
  Latent viaCallUser = userSample();
  Latent viaCallArticle = articleSample();
  Latent viaStepUser = userSample();
  Latent viaStepArticle = articleSample();
  learner(viaCallUser, viaCallArticle, Rating(10));
  learner.sgdStep(viaStepUser, viaStepArticle, 1.0);
  BOOST_CHECK(closeLatent(viaCallUser, viaStepUser));
  BOOST_CHECK(closeLatent(viaCallArticle, viaStepArticle));
}

BOOST_AUTO_TEST_CASE(rating_scale_maps_to_target_scale)
{
  Learner learner(0.1, 0.02, 1);
  const int ratings[3] = {1, 7, 4};
  const double targets[3] = {-1.0, 1.0 / 3.0, -1.0 / 3.0};
  for (std::size_t i = 0; i < 3; ++i)
  {
    Latent viaCallUser = userSample();
    Latent viaCallArticle = articleSample();
    Latent viaStepUser = userSample();
    Latent viaStepArticle = articleSample();
    learner(viaCallUser, viaCallArticle, Rating(ratings[i]));
    learner.sgdStep(viaStepUser, viaStepArticle, targets[i]);
    BOOST_CHECK(closeLatent(viaCallUser, viaStepUser));
    BOOST_CHECK(closeLatent(viaCallArticle, viaStepArticle));
  }
}

BOOST_AUTO_TEST_CASE(makes_exactly_step_count_steps)
{
  Learner threeSteps(0.1, 0.02, 3);
  Learner oneStep(0.1, 0.02, 1);
  Latent viaCallUser = userSample();
  Latent viaCallArticle = articleSample();
  Latent viaStepsUser = userSample();
  Latent viaStepsArticle = articleSample();
  threeSteps(viaCallUser, viaCallArticle, Rating(9));
  for (std::size_t i = 0; i < 3; ++i)
  {
    oneStep(viaStepsUser, viaStepsArticle, Rating(9));
  }
  BOOST_CHECK(closeLatent(viaCallUser, viaStepsUser));
  BOOST_CHECK(closeLatent(viaCallArticle, viaStepsArticle));
}

BOOST_AUTO_TEST_CASE(high_rating_raises_and_low_rating_lowers_prediction)
{
  Latent likedUser = userSample();
  Latent likedArticle = articleSample();
  Latent dislikedUser = userSample();
  Latent dislikedArticle = articleSample();
  double before = scalarProduct(userSample(), articleSample());
  Learner()(likedUser, likedArticle, Rating(10));
  Learner()(dislikedUser, dislikedArticle, Rating(1));
  BOOST_CHECK(scalarProduct(likedUser, likedArticle) > before);
  BOOST_CHECK(scalarProduct(dislikedUser, dislikedArticle) < before);
}

BOOST_AUTO_TEST_CASE(error_decreases_after_learning)
{
  const int ratings[4] = {1, 4, 7, 10};
  for (int rating : ratings)
  {
    double target = toInterval(rating, 1.0, 10.0, -1.0, 1.0);
    Latent user = userSample();
    Latent article = articleSample();
    double before = predictionError(user, article, target);
    Learner()(user, article, Rating(rating));
    BOOST_CHECK(predictionError(user, article, target) < before);
  }
}

BOOST_AUTO_TEST_CASE(is_deterministic)
{
  Latent firstUser = userSample();
  Latent firstArticle = articleSample();
  Latent secondUser = userSample();
  Latent secondArticle = articleSample();
  Learner()(firstUser, firstArticle, Rating(6));
  Learner()(secondUser, secondArticle, Rating(6));
  BOOST_CHECK(sameLatent(firstUser, secondUser));
  BOOST_CHECK(sameLatent(firstArticle, secondArticle));
}

BOOST_AUTO_TEST_CASE(zero_vectors_stay_zero)
{
  Latent user{};
  Latent article{};
  Learner()(user, article, Rating(10));
  BOOST_CHECK(sameLatent(user, Latent{}));
  BOOST_CHECK(sameLatent(article, Latent{}));
}

BOOST_AUTO_TEST_CASE(repeated_rating_converges_to_target_minus_regularization)
{
  const int ratings[3] = {10, 7, 1};
  const double limits[3] = {0.98, 1.0 / 3.0 - 0.02, -0.98};
  for (std::size_t i = 0; i < 3; ++i)
  {
    Latent user = userSample();
    Latent article = articleSample();
    for (std::size_t call = 0; call < 300; ++call)
    {
      Learner()(user, article, Rating(ratings[i]));
    }
    BOOST_CHECK(isClose(scalarProduct(user, article), limits[i], convergenceTolerance));
  }
}

BOOST_AUTO_TEST_SUITE_END()

using namespace khasnulin;

namespace
{

  Latent twoComponentLatent(double first, double second)
  {
    Latent latent{};
    latent.v[0] = first;
    latent.v[1] = second;
    return latent;
  }

  TagSet makeTags(const std::vector< std::string > &values)
  {
    TagSet tags;
    for (const std::string &value : values)
    {
      tags.insert(TagSet::value_type(Tag(value), true));
    }
    return tags;
  }

  TagSet preferenceSample()
  {
    return makeTags({"cpp", "backend", "async"});
  }

  TagSet articleTagSample()
  {
    return makeTags({"cpp", "templates", "async", "memory"});
  }
}

BOOST_AUTO_TEST_SUITE(confidence_tests)

BOOST_AUTO_TEST_CASE(default_shrinkage_matches_design)
{
  BOOST_CHECK(Scorer::defaultShrinkage == 5.0);
}

BOOST_AUTO_TEST_CASE(matches_design_values)
{
  Scorer recommender;
  BOOST_CHECK(recommender.confidence(0) == 0.0);
  BOOST_CHECK(isClose(recommender.confidence(5), 0.5));
  BOOST_CHECK(isClose(recommender.confidence(15), 0.75));
  BOOST_CHECK(isClose(recommender.confidence(1), 1.0 / 6.0));
}

BOOST_AUTO_TEST_CASE(grows_with_rating_count_and_stays_below_one)
{
  Scorer recommender;
  double previous = recommender.confidence(0);
  for (std::size_t count = 1; count <= 100; ++count)
  {
    double current = recommender.confidence(count);
    BOOST_CHECK(current > previous);
    BOOST_CHECK(current < 1.0);
    previous = current;
  }
  BOOST_CHECK(recommender.confidence(1000000) < 1.0);
}

BOOST_AUTO_TEST_CASE(half_trust_is_reached_at_shrinkage)
{
  BOOST_CHECK(isClose(Scorer(1.0).confidence(1), 0.5));
  BOOST_CHECK(isClose(Scorer(10.0).confidence(10), 0.5));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(mf_signal_tests)

BOOST_AUTO_TEST_CASE(zero_vectors_give_neutral_half)
{
  BOOST_CHECK(isClose(Scorer::mfSignal(Latent{}, Latent{}), 0.5));
}

BOOST_AUTO_TEST_CASE(scale_bounds_map_to_unit_bounds)
{
  BOOST_CHECK(isClose(Scorer::mfSignal(firstComponentLatent(1.0), firstComponentLatent(1.0)), 1.0));
  BOOST_CHECK(isClose(Scorer::mfSignal(firstComponentLatent(1.0), firstComponentLatent(-1.0)), 0.0));
}

BOOST_AUTO_TEST_CASE(maps_inner_value_linearly)
{
  BOOST_CHECK(isClose(Scorer::mfSignal(firstComponentLatent(1.0), firstComponentLatent(0.6)), 0.8));
  BOOST_CHECK(isClose(Scorer::mfSignal(firstComponentLatent(1.0), firstComponentLatent(-0.6)), 0.2));
}

BOOST_AUTO_TEST_CASE(clips_prediction_outside_scale)
{
  BOOST_CHECK(isClose(Scorer::mfSignal(firstComponentLatent(1.5), firstComponentLatent(2.0)), 1.0));
  BOOST_CHECK(isClose(Scorer::mfSignal(firstComponentLatent(1.5), firstComponentLatent(-2.0)), 0.0));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(user_article_score_tests)

BOOST_AUTO_TEST_CASE(cold_start_uses_only_tags)
{
  Scorer recommender;
  double score = recommender.userArticleScore(firstComponentLatent(1.0), preferenceSample(), 0,
                                              firstComponentLatent(0.6), articleTagSample());
  BOOST_CHECK(isClose(score, 0.4));
}

BOOST_AUTO_TEST_CASE(cold_start_ignores_vectors)
{
  Scorer recommender;
  double liked = recommender.userArticleScore(firstComponentLatent(1.5), preferenceSample(), 0,
                                              firstComponentLatent(2.0), articleTagSample());
  double disliked = recommender.userArticleScore(firstComponentLatent(1.5), preferenceSample(), 0,
                                                 firstComponentLatent(-2.0), articleTagSample());
  BOOST_CHECK(isClose(liked, disliked));
}

BOOST_AUTO_TEST_CASE(shrinkage_count_gives_equal_weights)
{
  Scorer recommender;
  double score = recommender.userArticleScore(firstComponentLatent(1.0), preferenceSample(), 5,
                                              firstComponentLatent(0.6), articleTagSample());
  BOOST_CHECK(isClose(score, 0.5 * 0.8 + 0.5 * 0.4));
}

BOOST_AUTO_TEST_CASE(matches_design_example)
{
  Scorer recommender;
  double score = recommender.userArticleScore(firstComponentLatent(1.0), preferenceSample(), 15,
                                              firstComponentLatent(0.6), articleTagSample());
  BOOST_CHECK(isClose(score, 0.75 * 0.8 + 0.25 * 0.4));
}

BOOST_AUTO_TEST_CASE(many_ratings_approach_mf_signal)
{
  Scorer recommender;
  double score = recommender.userArticleScore(firstComponentLatent(1.0), preferenceSample(), 1000000,
                                              firstComponentLatent(0.6), articleTagSample());
  BOOST_CHECK(isClose(score, 0.8, 1e-5));
}

BOOST_AUTO_TEST_CASE(custom_shrinkage_changes_weights)
{
  Scorer recommender(1.0);
  double score = recommender.userArticleScore(firstComponentLatent(1.0), preferenceSample(), 1,
                                              firstComponentLatent(0.6), articleTagSample());
  BOOST_CHECK(isClose(score, 0.5 * 0.8 + 0.5 * 0.4));
}

BOOST_AUTO_TEST_CASE(stays_within_unit_interval)
{
  Scorer recommender;
  TagSet tags = articleTagSample();
  double best =
      recommender.userArticleScore(firstComponentLatent(1.5), tags, 5, firstComponentLatent(2.0), tags);
  double worst = recommender.userArticleScore(firstComponentLatent(1.5), makeTags({"a", "b"}), 5,
                                              firstComponentLatent(-2.0), makeTags({"c", "d"}));
  BOOST_CHECK(isClose(best, 1.0));
  BOOST_CHECK(isClose(worst, 0.0));
}

BOOST_AUTO_TEST_CASE(no_preferences_rank_only_by_vectors)
{
  Scorer recommender;
  double liked = recommender.userArticleScore(firstComponentLatent(1.0), TagSet(), 5,
                                              firstComponentLatent(0.6), articleTagSample());
  double neutral =
      recommender.userArticleScore(firstComponentLatent(1.0), TagSet(), 5, Latent{}, articleTagSample());
  BOOST_CHECK(isClose(liked, 0.5 * 0.8));
  BOOST_CHECK(isClose(neutral, 0.5 * 0.5));
  BOOST_CHECK(liked > neutral);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(article_article_score_tests)

BOOST_AUTO_TEST_CASE(matches_design_example)
{
  Scorer recommender;
  double score = recommender.articleArticleScore(firstComponentLatent(1.0), preferenceSample(), 5,
                                                 twoComponentLatent(1.0, 1.0), articleTagSample(), 100);
  double cosineSignal = (1.0 / std::sqrt(2.0) + 1.0) / 2.0;
  BOOST_CHECK(isClose(score, 0.5 * cosineSignal + 0.5 * 0.4));
}

BOOST_AUTO_TEST_CASE(weight_uses_smaller_rating_count)
{
  Scorer recommender;
  double unrated = recommender.articleArticleScore(firstComponentLatent(1.0), preferenceSample(), 0,
                                                   firstComponentLatent(-1.0), articleTagSample(), 100);
  BOOST_CHECK(isClose(unrated, 0.4));
  double fewRatings = recommender.articleArticleScore(firstComponentLatent(1.0), preferenceSample(), 100,
                                                      firstComponentLatent(1.0), articleTagSample(), 5);
  BOOST_CHECK(isClose(fewRatings, 0.5 * 1.0 + 0.5 * 0.4));
}

BOOST_AUTO_TEST_CASE(is_symmetric_in_article_order)
{
  Scorer recommender;
  double forward = recommender.articleArticleScore(firstComponentLatent(1.0), preferenceSample(), 3,
                                                   twoComponentLatent(0.4, -0.9), articleTagSample(), 12);
  double backward = recommender.articleArticleScore(twoComponentLatent(0.4, -0.9), articleTagSample(), 12,
                                                    firstComponentLatent(1.0), preferenceSample(), 3);
  BOOST_CHECK(isClose(forward, backward));
}

BOOST_AUTO_TEST_CASE(identical_articles_give_one)
{
  Scorer recommender;
  Latent latent = twoComponentLatent(0.3, -0.7);
  TagSet tags = articleTagSample();
  BOOST_CHECK(isClose(recommender.articleArticleScore(latent, tags, 7, latent, tags, 7), 1.0));
}

BOOST_AUTO_TEST_CASE(does_not_depend_on_vector_length)
{
  Scorer recommender;
  double shortVectors = recommender.articleArticleScore(twoComponentLatent(0.1, 0.2), preferenceSample(), 5,
                                                        twoComponentLatent(0.3, -0.1), articleTagSample(), 5);
  double longVectors = recommender.articleArticleScore(twoComponentLatent(10.0, 20.0), preferenceSample(), 5,
                                                       twoComponentLatent(3.0, -1.0), articleTagSample(), 5);
  BOOST_CHECK(isClose(shortVectors, longVectors));
}

BOOST_AUTO_TEST_CASE(orthogonal_vectors_give_neutral_vector_signal)
{
  Scorer recommender;
  double score = recommender.articleArticleScore(firstComponentLatent(1.0), preferenceSample(), 5,
                                                 twoComponentLatent(0.0, 1.0), articleTagSample(), 5);
  BOOST_CHECK(isClose(score, 0.5 * 0.5 + 0.5 * 0.4));
}

BOOST_AUTO_TEST_CASE(empty_tags_rely_only_on_vectors)
{
  Scorer recommender;
  Latent latent = twoComponentLatent(0.3, -0.7);
  BOOST_CHECK(isClose(recommender.articleArticleScore(latent, TagSet(), 5, latent, TagSet(), 5), 0.5));
}

BOOST_AUTO_TEST_SUITE_END()
