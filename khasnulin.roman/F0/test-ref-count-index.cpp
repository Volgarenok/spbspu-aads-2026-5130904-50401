#include "Entities.hpp"
#include "RefCountIndex.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

using namespace khasnulin;

namespace
{
  using TagIndex = RefCountIndex< Tag >;
  using CategoryIndex = RefCountIndex< Category >;
  using Entry = std::pair< std::string, std::size_t >;

  template < class Key > std::vector< Entry > collect(const RefCountIndex< Key > &index)
  {
    std::vector< Entry > result;
    index.traverse([&result](const Key &key, std::size_t count) { result.push_back({key.value(), count}); });
    return result;
  }
}

BOOST_AUTO_TEST_SUITE(ref_count_index_tests)

BOOST_AUTO_TEST_CASE(new_index_is_empty)
{
  TagIndex index;
  BOOST_CHECK(index.empty());
  BOOST_CHECK(index.size() == 0);
  BOOST_CHECK(index.count(Tag("cpp")) == 0);
  BOOST_CHECK(collect(index).empty());
}

BOOST_AUTO_TEST_CASE(first_acquire_reports_new_key)
{
  TagIndex index;
  BOOST_CHECK(index.acquire(Tag("cpp")));
  BOOST_CHECK(index.count(Tag("cpp")) == 1);
  BOOST_CHECK(index.size() == 1);
  BOOST_CHECK(index.empty() == false);
}

BOOST_AUTO_TEST_CASE(repeated_acquire_increments_count)
{
  TagIndex index;
  index.acquire(Tag("cpp"));
  BOOST_CHECK(index.acquire(Tag("cpp")) == false);
  BOOST_CHECK(index.acquire(Tag("cpp")) == false);
  BOOST_CHECK(index.count(Tag("cpp")) == 3);
  BOOST_CHECK(index.size() == 1);
}

BOOST_AUTO_TEST_CASE(keys_are_normalized_tags)
{
  TagIndex index;
  index.acquire(Tag("CPP"));
  BOOST_CHECK(index.acquire(Tag("  cpp ")) == false);
  BOOST_CHECK(index.count(Tag("cpp")) == 2);
  BOOST_CHECK(index.size() == 1);
}

BOOST_AUTO_TEST_CASE(release_decrements_and_keeps_key)
{
  TagIndex index;
  index.acquire(Tag("cpp"));
  index.acquire(Tag("cpp"));
  index.release(Tag("cpp"));
  BOOST_CHECK(index.count(Tag("cpp")) == 1);
  BOOST_CHECK(index.size() == 1);
}

BOOST_AUTO_TEST_CASE(last_release_removes_key)
{
  TagIndex index;
  index.acquire(Tag("cpp"));
  index.acquire(Tag("go"));
  index.release(Tag("cpp"));
  BOOST_CHECK(index.count(Tag("cpp")) == 0);
  BOOST_CHECK(index.size() == 1);
  std::vector< Entry > expected = {{"go", 1}};
  BOOST_CHECK(collect(index) == expected);
}

BOOST_AUTO_TEST_CASE(released_key_can_be_acquired_again)
{
  TagIndex index;
  index.acquire(Tag("cpp"));
  index.release(Tag("cpp"));
  BOOST_CHECK(index.empty());
  BOOST_CHECK(index.acquire(Tag("cpp")));
  BOOST_CHECK(index.count(Tag("cpp")) == 1);
}

BOOST_AUTO_TEST_CASE(size_counts_distinct_keys_not_references)
{
  TagIndex index;
  index.acquire(Tag("cpp"));
  index.acquire(Tag("cpp"));
  index.acquire(Tag("go"));
  index.acquire(Tag("rust"));
  index.acquire(Tag("rust"));
  index.acquire(Tag("rust"));
  BOOST_CHECK(index.size() == 3);
}

BOOST_AUTO_TEST_CASE(traverse_visits_keys_in_ascending_order_with_counts)
{
  TagIndex index;
  index.acquire(Tag("rust"));
  index.acquire(Tag("cpp"));
  index.acquire(Tag("go"));
  index.acquire(Tag("cpp"));
  index.acquire(Tag("algorithms"));
  std::vector< Entry > expected = {{"algorithms", 1}, {"cpp", 2}, {"go", 1}, {"rust", 1}};
  BOOST_CHECK(collect(index) == expected);
}

BOOST_AUTO_TEST_CASE(clear_removes_all_keys)
{
  TagIndex index;
  index.acquire(Tag("cpp"));
  index.acquire(Tag("go"));
  index.clear();
  BOOST_CHECK(index.empty());
  BOOST_CHECK(index.count(Tag("cpp")) == 0);
  BOOST_CHECK(index.acquire(Tag("cpp")));
}

BOOST_AUTO_TEST_CASE(swap_exchanges_contents)
{
  TagIndex lhs;
  lhs.acquire(Tag("cpp"));
  lhs.acquire(Tag("cpp"));
  TagIndex rhs;
  rhs.acquire(Tag("go"));
  rhs.acquire(Tag("rust"));
  lhs.swap(rhs);
  std::vector< Entry > expectedLhs = {{"go", 1}, {"rust", 1}};
  std::vector< Entry > expectedRhs = {{"cpp", 2}};
  BOOST_CHECK(collect(lhs) == expectedLhs);
  BOOST_CHECK(collect(rhs) == expectedRhs);
}

BOOST_AUTO_TEST_CASE(swap_with_empty_index)
{
  TagIndex filled;
  filled.acquire(Tag("cpp"));
  TagIndex empty;
  filled.swap(empty);
  BOOST_CHECK(filled.empty());
  BOOST_CHECK(empty.count(Tag("cpp")) == 1);
}

BOOST_AUTO_TEST_CASE(copy_is_independent)
{
  TagIndex original;
  original.acquire(Tag("cpp"));
  TagIndex copy = original;
  copy.acquire(Tag("cpp"));
  copy.acquire(Tag("go"));
  copy.release(Tag("cpp"));
  copy.release(Tag("cpp"));
  BOOST_CHECK(original.count(Tag("cpp")) == 1);
  BOOST_CHECK(original.size() == 1);
  BOOST_CHECK(copy.count(Tag("cpp")) == 0);
  BOOST_CHECK(copy.count(Tag("go")) == 1);
}

BOOST_AUTO_TEST_CASE(works_with_categories)
{
  CategoryIndex index;
  BOOST_CHECK(index.acquire(Category("AI")));
  BOOST_CHECK(index.acquire(Category(" ai ")) == false);
  index.acquire(Category("Algorithms"));
  std::vector< Entry > expected = {{"ai", 2}, {"algorithms", 1}};
  BOOST_CHECK(collect(index) == expected);
  index.release(Category("ai"));
  index.release(Category("ai"));
  BOOST_CHECK(index.count(Category("ai")) == 0);
  BOOST_CHECK(index.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
