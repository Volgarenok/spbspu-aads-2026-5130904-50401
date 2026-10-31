#include "Math.hpp"
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstddef>
#include <string>
#include <vector>

using namespace khasnulin;

namespace
{
  const double tolerance = 1e-9;

  bool isClose(double lhs, double rhs)
  {
    return std::fabs(lhs - rhs) < tolerance;
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

  Latent basisLatent(std::size_t index)
  {
    Latent latent{};
    latent.v[index] = 1.0;
    return latent;
  }

  Latent indexLatent()
  {
    Latent latent{};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      latent.v[i] = static_cast< double >(i);
    }
    return latent;
  }

  Latent mixedLatent()
  {
    Latent latent{};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      latent.v[i] = (i % 2 == 0) ? 0.3 + 0.1 * static_cast< double >(i) : -0.2 * static_cast< double >(i);
    }
    return latent;
  }

  Latent scaleLatent(const Latent &latent, double factor)
  {
    Latent result{};
    for (std::size_t i = 0; i < latentDimensionCount; ++i)
    {
      result.v[i] = latent.v[i] * factor;
    }
    return result;
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
}

BOOST_AUTO_TEST_SUITE(vec_module_tests)

BOOST_AUTO_TEST_CASE(zero_vector_has_zero_length)
{
  BOOST_CHECK(vecModule(Latent{}) == 0.0);
}

BOOST_AUTO_TEST_CASE(basis_vector_has_unit_length)
{
  BOOST_CHECK(isClose(vecModule(basisLatent(0)), 1.0));
  BOOST_CHECK(isClose(vecModule(basisLatent(latentDimensionCount - 1)), 1.0));
}

BOOST_AUTO_TEST_CASE(matches_pythagorean_triple)
{
  Latent latent{};
  latent.v[0] = 3.0;
  latent.v[1] = 4.0;
  BOOST_CHECK(isClose(vecModule(latent), 5.0));
}

BOOST_AUTO_TEST_CASE(uses_all_dimensions)
{
  double expected = std::sqrt(static_cast< double >(latentDimensionCount));
  BOOST_CHECK(isClose(vecModule(fillLatent(1.0)), expected));
}

BOOST_AUTO_TEST_CASE(ignores_sign)
{
  BOOST_CHECK(isClose(vecModule(fillLatent(-1.0)), vecModule(fillLatent(1.0))));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(scalar_product_tests)

BOOST_AUTO_TEST_CASE(orthogonal_basis_vectors_give_zero)
{
  BOOST_CHECK(scalarProduct(basisLatent(0), basisLatent(1)) == 0.0);
}

BOOST_AUTO_TEST_CASE(zero_vector_gives_zero)
{
  BOOST_CHECK(scalarProduct(Latent{}, mixedLatent()) == 0.0);
  BOOST_CHECK(scalarProduct(mixedLatent(), Latent{}) == 0.0);
}

BOOST_AUTO_TEST_CASE(sums_over_all_dimensions)
{
  double expected = static_cast< double >(latentDimensionCount * (latentDimensionCount - 1) / 2);
  BOOST_CHECK(isClose(scalarProduct(indexLatent(), fillLatent(1.0)), expected));
}

BOOST_AUTO_TEST_CASE(is_negative_for_opposite_vectors)
{
  double expected = -static_cast< double >(latentDimensionCount);
  BOOST_CHECK(isClose(scalarProduct(fillLatent(1.0), fillLatent(-1.0)), expected));
}

BOOST_AUTO_TEST_CASE(is_symmetric)
{
  BOOST_CHECK(
      isClose(scalarProduct(indexLatent(), mixedLatent()), scalarProduct(mixedLatent(), indexLatent())));
}

BOOST_AUTO_TEST_CASE(square_of_vector_equals_squared_module)
{
  double module = vecModule(mixedLatent());
  BOOST_CHECK(isClose(scalarProduct(mixedLatent(), mixedLatent()), module * module));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(cosine_tests)

BOOST_AUTO_TEST_CASE(same_direction_gives_one)
{
  BOOST_CHECK(isClose(cosine(mixedLatent(), mixedLatent()), 1.0));
  BOOST_CHECK(isClose(cosine(mixedLatent(), scaleLatent(mixedLatent(), 10.0)), 1.0));
}

BOOST_AUTO_TEST_CASE(opposite_direction_gives_minus_one)
{
  BOOST_CHECK(isClose(cosine(mixedLatent(), scaleLatent(mixedLatent(), -2.0)), -1.0));
}

BOOST_AUTO_TEST_CASE(orthogonal_vectors_give_zero)
{
  BOOST_CHECK(isClose(cosine(basisLatent(0), basisLatent(1)), 0.0));
}

BOOST_AUTO_TEST_CASE(matches_known_angle)
{
  Latent diagonal{};
  diagonal.v[0] = 1.0;
  diagonal.v[1] = 1.0;
  BOOST_CHECK(isClose(cosine(basisLatent(0), diagonal), 1.0 / std::sqrt(2.0)));
}

BOOST_AUTO_TEST_CASE(does_not_depend_on_length)
{
  double original = cosine(indexLatent(), mixedLatent());
  double scaled = cosine(scaleLatent(indexLatent(), 3.0), scaleLatent(mixedLatent(), 0.5));
  BOOST_CHECK(isClose(original, scaled));
}

BOOST_AUTO_TEST_CASE(zero_vector_gives_zero)
{
  BOOST_CHECK(cosine(Latent{}, mixedLatent()) == 0.0);
  BOOST_CHECK(cosine(mixedLatent(), Latent{}) == 0.0);
  BOOST_CHECK(cosine(Latent{}, Latent{}) == 0.0);
}

BOOST_AUTO_TEST_CASE(is_symmetric)
{
  BOOST_CHECK(isClose(cosine(indexLatent(), mixedLatent()), cosine(mixedLatent(), indexLatent())));
}

BOOST_AUTO_TEST_CASE(stays_strictly_within_bounds)
{
  Latent samples[] = {mixedLatent(),
                      indexLatent(),
                      fillLatent(0.1),
                      fillLatent(0.7),
                      scaleLatent(mixedLatent(), 1e-3),
                      scaleLatent(indexLatent(), 1e3)};
  for (const Latent &sample : samples)
  {
    double same = cosine(sample, sample);
    double opposite = cosine(sample, scaleLatent(sample, -1.0));
    BOOST_CHECK(same <= 1.0);
    BOOST_CHECK(same >= -1.0);
    BOOST_CHECK(opposite >= -1.0);
    BOOST_CHECK(opposite <= 1.0);
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(jaccard_tests)

BOOST_AUTO_TEST_CASE(matches_design_example)
{
  TagSet preferences = makeTags({"cpp", "backend", "async"});
  TagSet articleTags = makeTags({"cpp", "templates", "async", "memory"});
  BOOST_CHECK(isClose(jaccard(preferences, articleTags), 0.4));
}

BOOST_AUTO_TEST_CASE(both_empty_give_zero)
{
  BOOST_CHECK(jaccard(TagSet(), TagSet()) == 0.0);
}

BOOST_AUTO_TEST_CASE(one_empty_gives_zero)
{
  TagSet tags = makeTags({"cpp", "go"});
  BOOST_CHECK(jaccard(tags, TagSet()) == 0.0);
  BOOST_CHECK(jaccard(TagSet(), tags) == 0.0);
}

BOOST_AUTO_TEST_CASE(equal_sets_give_one)
{
  BOOST_CHECK(isClose(jaccard(makeTags({"cpp", "go", "rust"}), makeTags({"rust", "cpp", "go"})), 1.0));
}

BOOST_AUTO_TEST_CASE(disjoint_sets_give_zero)
{
  BOOST_CHECK(jaccard(makeTags({"a", "b"}), makeTags({"c", "d"})) == 0.0);
  BOOST_CHECK(jaccard(makeTags({"a", "c"}), makeTags({"b", "d"})) == 0.0);
}

BOOST_AUTO_TEST_CASE(subset_gives_size_ratio)
{
  BOOST_CHECK(isClose(jaccard(makeTags({"cpp", "go"}), makeTags({"cpp", "go", "java", "rust"})), 0.5));
}

BOOST_AUTO_TEST_CASE(handles_interleaved_tags)
{
  TagSet first = makeTags({"a", "c", "e", "g"});
  TagSet second = makeTags({"b", "c", "f", "g"});
  BOOST_CHECK(isClose(jaccard(first, second), 1.0 / 3.0));
}

BOOST_AUTO_TEST_CASE(counts_match_at_the_end)
{
  BOOST_CHECK(isClose(jaccard(makeTags({"a", "z"}), makeTags({"b", "z"})), 1.0 / 3.0));
}

BOOST_AUTO_TEST_CASE(is_symmetric)
{
  TagSet first = makeTags({"cpp", "go", "async"});
  TagSet second = makeTags({"go", "rust"});
  BOOST_CHECK(isClose(jaccard(first, second), jaccard(second, first)));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(clip_tests)

BOOST_AUTO_TEST_CASE(keeps_value_inside)
{
  BOOST_CHECK(clip(0.3, -1.0, 1.0) == 0.3);
}

BOOST_AUTO_TEST_CASE(clamps_below_to_low)
{
  BOOST_CHECK(clip(-5.0, -1.0, 1.0) == -1.0);
}

BOOST_AUTO_TEST_CASE(clamps_above_to_high)
{
  BOOST_CHECK(clip(1.7, -1.0, 1.0) == 1.0);
}

BOOST_AUTO_TEST_CASE(keeps_bounds)
{
  BOOST_CHECK(clip(-1.0, -1.0, 1.0) == -1.0);
  BOOST_CHECK(clip(1.0, -1.0, 1.0) == 1.0);
}

BOOST_AUTO_TEST_CASE(works_on_rating_scale)
{
  BOOST_CHECK(clip(12.3, 1.0, 10.0) == 10.0);
  BOOST_CHECK(clip(0.2, 1.0, 10.0) == 1.0);
  BOOST_CHECK(clip(7.5, 1.0, 10.0) == 7.5);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(to_interval_tests)

BOOST_AUTO_TEST_CASE(maps_bounds_to_bounds)
{
  BOOST_CHECK(isClose(toInterval(1.0, 1.0, 10.0, -1.0, 1.0), -1.0));
  BOOST_CHECK(isClose(toInterval(10.0, 1.0, 10.0, -1.0, 1.0), 1.0));
}

BOOST_AUTO_TEST_CASE(maps_midpoint_to_midpoint)
{
  BOOST_CHECK(isClose(toInterval(5.5, 1.0, 10.0, -1.0, 1.0), 0.0));
  BOOST_CHECK(isClose(toInterval(0.0, -1.0, 1.0, 0.0, 1.0), 0.5));
}

BOOST_AUTO_TEST_CASE(rating_to_signed_scale)
{
  BOOST_CHECK(isClose(toInterval(7.0, 1.0, 10.0, -1.0, 1.0), 1.0 / 3.0));
  BOOST_CHECK(isClose(toInterval(4.0, 1.0, 10.0, -1.0, 1.0), -1.0 / 3.0));
}

BOOST_AUTO_TEST_CASE(prediction_to_unit_scale)
{
  BOOST_CHECK(isClose(toInterval(1.0, 1.0, 10.0, 0.0, 1.0), 0.0));
  BOOST_CHECK(isClose(toInterval(10.0, 1.0, 10.0, 0.0, 1.0), 1.0));
  BOOST_CHECK(isClose(toInterval(4.0, 1.0, 10.0, 0.0, 1.0), 1.0 / 3.0));
}

BOOST_AUTO_TEST_CASE(signed_to_unit_scale)
{
  BOOST_CHECK(isClose(toInterval(-1.0, -1.0, 1.0, 0.0, 1.0), 0.0));
  BOOST_CHECK(isClose(toInterval(1.0, -1.0, 1.0, 0.0, 1.0), 1.0));
  BOOST_CHECK(isClose(toInterval(0.6, -1.0, 1.0, 0.0, 1.0), 0.8));
}

BOOST_AUTO_TEST_CASE(preserves_order)
{
  BOOST_CHECK(toInterval(3.0, 1.0, 10.0, -1.0, 1.0) < toInterval(8.0, 1.0, 10.0, -1.0, 1.0));
}

BOOST_AUTO_TEST_CASE(round_trip_returns_original)
{
  double signedValue = toInterval(7.3, 1.0, 10.0, -1.0, 1.0);
  BOOST_CHECK(isClose(toInterval(signedValue, -1.0, 1.0, 1.0, 10.0), 7.3));
}

BOOST_AUTO_TEST_CASE(composition_equals_direct_mapping)
{
  double ratings[] = {1.0, 2.5, 5.5, 8.0, 10.0};
  for (double rating : ratings)
  {
    double viaSigned = toInterval(toInterval(rating, 1.0, 10.0, -1.0, 1.0), -1.0, 1.0, 0.0, 1.0);
    double direct = toInterval(rating, 1.0, 10.0, 0.0, 1.0);
    BOOST_CHECK(isClose(viaSigned, direct));
  }
}

BOOST_AUTO_TEST_SUITE_END()
