#include "BoundedPriorityQueue.hpp"
#include "MaxHeap.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

using namespace khasnulin;

namespace
{
  struct GreaterIsBetter
  {
    bool operator()(int lhs, int rhs) const
    {
      return lhs > rhs;
    }
  };

  struct Candidate
  {
    double score;
    std::string name;
  };

  struct CandidateBetter
  {
    bool operator()(const Candidate &lhs, const Candidate &rhs) const
    {
      if (lhs.score != rhs.score)
      {
        return lhs.score > rhs.score;
      }
      return lhs.name < rhs.name;
    }
  };

  struct CloserToCenter
  {
    int center;

    bool operator()(int lhs, int rhs) const
    {
      return std::abs(lhs - center) < std::abs(rhs - center);
    }
  };

  using IntQueue = BoundedPriorityQueue< int, GreaterIsBetter >;
  using CandidateQueue = BoundedPriorityQueue< Candidate, CandidateBetter >;

  template < class Queue > std::vector< typename Queue::value_type > drainAll(Queue &queue)
  {
    std::vector< typename Queue::value_type > result;
    queue.traverseAndClear([&result](const typename Queue::value_type &value) { result.push_back(value); });
    return result;
  }

  std::vector< bool > suggestAll(IntQueue &queue, const std::vector< int > &values)
  {
    std::vector< bool > accepted;
    for (int value : values)
    {
      accepted.push_back(queue.suggest(value));
    }
    return accepted;
  }
}

BOOST_AUTO_TEST_SUITE(bounded_priority_queue_construction_tests)

BOOST_AUTO_TEST_CASE(new_queue_is_empty_with_given_limit)
{
  IntQueue queue(5, GreaterIsBetter());
  BOOST_CHECK(queue.limit() == 5);
  BOOST_CHECK(queue.size() == 0);
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(traverse_of_empty_queue_calls_nothing)
{
  IntQueue queue(3, GreaterIsBetter());
  BOOST_CHECK(drainAll(queue).empty());
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bounded_priority_queue_suggest_tests)

BOOST_AUTO_TEST_CASE(fewer_than_limit_are_all_accepted)
{
  IntQueue queue(5, GreaterIsBetter());
  std::vector< bool > accepted = suggestAll(queue, {3, 1, 2});
  BOOST_CHECK(accepted == std::vector< bool >({true, true, true}));
  BOOST_CHECK(queue.size() == 3);
  BOOST_CHECK(!queue.empty());
}

BOOST_AUTO_TEST_CASE(ascending_stream_displaces_on_every_step)
{
  IntQueue queue(3, GreaterIsBetter());
  std::vector< bool > accepted = suggestAll(queue, {1, 2, 3, 4, 5, 6});
  BOOST_CHECK(accepted == std::vector< bool >({true, true, true, true, true, true}));
  BOOST_CHECK(queue.size() == 3);
  BOOST_CHECK(drainAll(queue) == std::vector< int >({6, 5, 4}));
}

BOOST_AUTO_TEST_CASE(descending_stream_rejects_after_filling)
{
  IntQueue queue(3, GreaterIsBetter());
  std::vector< bool > accepted = suggestAll(queue, {6, 5, 4, 3, 2, 1});
  BOOST_CHECK(accepted == std::vector< bool >({true, true, true, false, false, false}));
  BOOST_CHECK(queue.size() == 3);
  BOOST_CHECK(drainAll(queue) == std::vector< int >({6, 5, 4}));
}

BOOST_AUTO_TEST_CASE(mixed_stream_keeps_best)
{
  IntQueue queue(3, GreaterIsBetter());
  std::vector< bool > accepted = suggestAll(queue, {5, 1, 8, 3, 9, 7, 2, 6});
  BOOST_CHECK(accepted == std::vector< bool >({true, true, true, true, true, true, false, false}));
  BOOST_CHECK(drainAll(queue) == std::vector< int >({9, 8, 7}));
}

BOOST_AUTO_TEST_CASE(equal_to_worst_is_rejected)
{
  IntQueue queue(2, GreaterIsBetter());
  queue.suggest(5);
  queue.suggest(3);
  BOOST_CHECK(!queue.suggest(3));
  BOOST_CHECK(queue.size() == 2);
  BOOST_CHECK(drainAll(queue) == std::vector< int >({5, 3}));
}

BOOST_AUTO_TEST_CASE(size_never_exceeds_limit)
{
  IntQueue queue(4, GreaterIsBetter());
  for (int value = 0; value < 100; ++value)
  {
    queue.suggest(value % 2 == 0 ? value : -value);
    BOOST_CHECK(queue.size() <= queue.limit());
  }
  BOOST_CHECK(queue.size() == 4);
}

BOOST_AUTO_TEST_CASE(limit_one_keeps_only_best)
{
  IntQueue queue(1, GreaterIsBetter());
  suggestAll(queue, {4, 9, 2, 7});
  BOOST_CHECK(drainAll(queue) == std::vector< int >({9}));
}

BOOST_AUTO_TEST_CASE(rvalue_suggest_works)
{
  CandidateQueue queue(2, CandidateBetter());
  Candidate first{0.3, "first"};
  Candidate second{0.8, "second"};
  Candidate third{0.5, "third"};
  BOOST_CHECK(queue.suggest(std::move(first)));
  BOOST_CHECK(queue.suggest(std::move(second)));
  BOOST_CHECK(queue.suggest(std::move(third)));
  std::vector< Candidate > result = drainAll(queue);
  BOOST_CHECK(result.size() == 2);
  BOOST_CHECK(result[0].name == "second");
  BOOST_CHECK(result[1].name == "third");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bounded_priority_queue_composite_compare_tests)

BOOST_AUTO_TEST_CASE(equal_scores_ordered_by_name)
{
  CandidateQueue queue(3, CandidateBetter());
  queue.suggest(Candidate{0.5, "c"});
  queue.suggest(Candidate{0.5, "a"});
  queue.suggest(Candidate{0.5, "b"});
  std::vector< Candidate > result = drainAll(queue);
  BOOST_CHECK(result.size() == 3);
  BOOST_CHECK(result[0].name == "a");
  BOOST_CHECK(result[1].name == "b");
  BOOST_CHECK(result[2].name == "c");
}

BOOST_AUTO_TEST_CASE(tie_break_decides_who_is_displaced)
{
  CandidateQueue queue(2, CandidateBetter());
  queue.suggest(Candidate{0.5, "b"});
  queue.suggest(Candidate{0.5, "c"});
  BOOST_CHECK(queue.suggest(Candidate{0.5, "a"}));
  BOOST_CHECK(!queue.suggest(Candidate{0.5, "d"}));
  BOOST_CHECK(queue.suggest(Candidate{0.9, "z"}));
  std::vector< Candidate > result = drainAll(queue);
  BOOST_CHECK(result.size() == 2);
  BOOST_CHECK(result[0].name == "z");
  BOOST_CHECK(result[1].name == "a");
}

BOOST_AUTO_TEST_CASE(result_does_not_depend_on_input_order)
{
  std::vector< Candidate > candidates = {{0.7, "x"}, {0.7, "y"}, {0.2, "q"}, {0.9, "m"}, {0.7, "w"}};
  CandidateQueue forward(3, CandidateBetter());
  CandidateQueue backward(3, CandidateBetter());
  for (std::size_t i = 0; i < candidates.size(); ++i)
  {
    forward.suggest(candidates[i]);
    backward.suggest(candidates[candidates.size() - 1 - i]);
  }
  std::vector< Candidate > forwardResult = drainAll(forward);
  std::vector< Candidate > backwardResult = drainAll(backward);
  BOOST_CHECK(forwardResult.size() == 3);
  BOOST_CHECK(backwardResult.size() == 3);
  for (std::size_t i = 0; i < forwardResult.size(); ++i)
  {
    BOOST_CHECK(forwardResult[i].name == backwardResult[i].name);
  }
  BOOST_CHECK(forwardResult[0].name == "m");
  BOOST_CHECK(forwardResult[1].name == "w");
  BOOST_CHECK(forwardResult[2].name == "x");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bounded_priority_queue_clear_tests)

BOOST_AUTO_TEST_CASE(traverse_visits_exactly_size_elements_and_empties)
{
  IntQueue queue(4, GreaterIsBetter());
  suggestAll(queue, {10, 20, 30});
  std::size_t sizeBefore = queue.size();
  std::size_t visited = 0;
  queue.traverseAndClear([&visited](const int &) { ++visited; });
  BOOST_CHECK(visited == sizeBefore);
  BOOST_CHECK(queue.empty());
  BOOST_CHECK(queue.size() == 0);
  BOOST_CHECK(queue.limit() == 4);
}

BOOST_AUTO_TEST_CASE(can_refill_after_traverse)
{
  IntQueue queue(2, GreaterIsBetter());
  suggestAll(queue, {9, 8, 7});
  drainAll(queue);
  std::vector< bool > accepted = suggestAll(queue, {1, 2, 3});
  BOOST_CHECK(accepted == std::vector< bool >({true, true, true}));
  BOOST_CHECK(drainAll(queue) == std::vector< int >({3, 2}));
}

BOOST_AUTO_TEST_CASE(clear_empties_and_keeps_limit)
{
  IntQueue queue(3, GreaterIsBetter());
  suggestAll(queue, {5, 6, 7, 8});
  queue.clear();
  BOOST_CHECK(queue.empty());
  BOOST_CHECK(queue.size() == 0);
  BOOST_CHECK(queue.limit() == 3);
  BOOST_CHECK(drainAll(queue).empty());
}

BOOST_AUTO_TEST_CASE(can_refill_after_clear)
{
  IntQueue queue(2, GreaterIsBetter());
  suggestAll(queue, {100, 200});
  queue.clear();
  std::vector< bool > accepted = suggestAll(queue, {1, 3, 2});
  BOOST_CHECK(accepted == std::vector< bool >({true, true, true}));
  BOOST_CHECK(drainAll(queue) == std::vector< int >({3, 2}));
}

BOOST_AUTO_TEST_CASE(clear_of_empty_queue_is_safe)
{
  IntQueue queue(2, GreaterIsBetter());
  queue.clear();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(max_heap_value_comp_tests)

BOOST_AUTO_TEST_CASE(returns_comparator_passed_to_constructor)
{
  const MaxHeap< int, CloserToCenter > heap(CloserToCenter{10});
  CloserToCenter compare = heap.valueComp();
  BOOST_CHECK(compare.center == 10);
  BOOST_CHECK(compare(9, 5));
  BOOST_CHECK(!compare(5, 9));
}

BOOST_AUTO_TEST_SUITE_END()
