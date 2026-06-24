#include <boost/test/unit_test.hpp>
#include <vector>
#include "AVLStructures.hpp"

BOOST_AUTO_TEST_SUITE(AVLMapTests)

BOOST_AUTO_TEST_CASE(basic_insert_and_find)
{
  vasyakin::AVLMap< int, std::string > map;

  auto result1 = map.insert({1, "one"});
  auto it1 = result1.first;
  auto ok1 = result1.second;

  BOOST_CHECK(ok1);
  BOOST_CHECK_EQUAL(it1->first, 1);
  BOOST_CHECK_EQUAL(it1->second, "one");

  auto result2 = map.insert({1, "uno"});
  auto it2 = result2.first;
  auto ok2 = result2.second;

  BOOST_CHECK(!ok2);
  BOOST_CHECK_EQUAL(it2->second, "one");

  BOOST_CHECK_EQUAL(map.size(), 1u);
  BOOST_CHECK_EQUAL(map.count(1), 1u);
  BOOST_CHECK_EQUAL(map.count(99), 0u);
}

BOOST_AUTO_TEST_CASE(at_and_operator_bracket)
{
  vasyakin::AVLMap< int, int > map;

  map[10] = 100;
  map[20] = 200;

  BOOST_CHECK_EQUAL(map.at(10), 100);
  BOOST_CHECK_EQUAL(map[20], 200);

  BOOST_CHECK_EQUAL(map[30], 0);
  BOOST_CHECK_EQUAL(map.size(), 3u);

  BOOST_CHECK_THROW(map.at(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(erase_and_clear)
{
  vasyakin::AVLMap< int, int > map;

  map.insert({1, 1});
  map.insert({2, 2});
  map.insert({3, 3});

  BOOST_CHECK_EQUAL(map.erase(2), 1u);
  BOOST_CHECK_EQUAL(map.erase(999), 0u);
  BOOST_CHECK_EQUAL(map.size(), 2u);
  BOOST_CHECK(!map.count(2));

  map.clear();
  BOOST_CHECK(map.empty());
  BOOST_CHECK_EQUAL(map.size(), 0u);
}

BOOST_AUTO_TEST_CASE(bounds_and_equal_range)
{
  vasyakin::AVLMap< int, std::string > map;

  map.insert({10, "ten"});
  map.insert({20, "twenty"});
  map.insert({30, "thirty"});
  map.insert({40, "forty"});

  auto lb = map.lower_bound(20);
  BOOST_CHECK_EQUAL(lb->first, 20);

  auto ub = map.upper_bound(20);
  BOOST_CHECK_EQUAL(ub->first, 30);

  auto range = map.equal_range(25);
  BOOST_CHECK_EQUAL(range.first->first, 30);
  BOOST_CHECK_EQUAL(range.second->first, 30);
}

BOOST_AUTO_TEST_CASE(iteration_order)
{
  vasyakin::AVLMap< int, int > map;

  map.insert({5, 5});
  map.insert({1, 1});
  map.insert({3, 3});
  map.insert({2, 2});
  map.insert({4, 4});

  std::vector< int > keys;
  for (auto it = map.begin(); it != map.end(); ++it)
  {
    keys.push_back(it->first);
  }

  std::vector< int > expected = {1, 2, 3, 4, 5};

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  vasyakin::AVLMap< int, std::string > map =
    {{1, "one"}, {2, "two"}, {3, "three"}};

  BOOST_CHECK_EQUAL(map.size(), 3u);
  BOOST_CHECK_EQUAL(map.at(1), "one");
  BOOST_CHECK_EQUAL(map.at(2), "two");
  BOOST_CHECK_EQUAL(map.at(3), "three");
}

BOOST_AUTO_TEST_CASE(copy_semantics)
{
  vasyakin::AVLMap< int, int > map1;

  map1.insert({1, 10});
  map1.insert({2, 20});

  vasyakin::AVLMap< int, int > map2 = map1;

  BOOST_CHECK_EQUAL(map2.size(), 2u);
  BOOST_CHECK_EQUAL(map2.at(1), 10);

  map2.erase(1);
  BOOST_CHECK_EQUAL(map1.size(), 2u);
}

BOOST_AUTO_TEST_CASE(map_move_semantics)
{
  vasyakin::AVLMap< int, std::string > map1;

  map1.insert({1, "one"});
  map1.insert({2, "two"});

  vasyakin::AVLMap< int, std::string > map2 = std::move(map1);

  BOOST_CHECK_EQUAL(map2.size(), 2u);
  BOOST_CHECK_EQUAL(map2.at(1), "one");
  BOOST_CHECK(map1.empty());

  vasyakin::AVLMap< int, std::string > map3;

  map3 = std::move(map2);
  BOOST_CHECK_EQUAL(map3.size(), 2u);
  BOOST_CHECK(map2.empty());
}

BOOST_AUTO_TEST_CASE(const_correctness)
{
  vasyakin::AVLMap< int, int > map;

  map.insert({1, 100});
  const auto& cmap = map;

  BOOST_CHECK_EQUAL(cmap.at(1), 100);
  BOOST_CHECK_EQUAL(cmap.count(1), 1u);
  BOOST_CHECK_EQUAL(cmap.size(), 1u);

  auto it = cmap.find(1);
  BOOST_CHECK(it != cmap.end());
  BOOST_CHECK_EQUAL(it->second, 100);

  auto range = cmap.equal_range(1);
  BOOST_CHECK_EQUAL(range.first->first, 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AVLSetTests)

BOOST_AUTO_TEST_CASE(basic_insert_and_find)
{
  vasyakin::AVLSet< int > set;

  auto result1 = set.insert(10);
  auto it1 = result1.first;
  auto ok1 = result1.second;

  BOOST_CHECK(ok1);
  BOOST_CHECK_EQUAL(it1->first, 10);

  auto result2 = set.insert(10);
  auto it2 = result2.first;
  auto ok2 = result2.second;

  BOOST_CHECK(!ok2);
  BOOST_CHECK_EQUAL(it2->first, 10);
  BOOST_CHECK(it1 == it2);

  BOOST_CHECK_EQUAL(set.size(), 1u);
  BOOST_CHECK_EQUAL(set.count(10), 1u);
  BOOST_CHECK_EQUAL(set.count(99), 0u);
}

BOOST_AUTO_TEST_CASE(erase_and_clear)
{
  vasyakin::AVLSet< int > set;

  set.insert(1);
  set.insert(2);
  set.insert(3);

  BOOST_CHECK_EQUAL(set.erase(2), 1u);
  BOOST_CHECK_EQUAL(set.erase(999), 0u);
  BOOST_CHECK_EQUAL(set.size(), 2u);

  set.clear();
  BOOST_CHECK(set.empty());
}

BOOST_AUTO_TEST_CASE(iteration_sorted)
{
  vasyakin::AVLSet< int > set;

  set.insert(5);
  set.insert(1);
  set.insert(4);
  set.insert(2);
  set.insert(3);

  std::vector< int > keys;
  for (auto it = set.begin(); it != set.end(); ++it)
  {
    keys.push_back(it->first);
  }

  std::vector< int > expected = {1, 2, 3, 4, 5};

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(const_correctness)
{
  vasyakin::AVLSet< int > set;

  set.insert(10);
  set.insert(20);
  set.insert(30);

  const vasyakin::AVLSet< int >& cset = set;

  BOOST_CHECK_EQUAL(cset.size(), 3u);
  BOOST_CHECK(!cset.empty());
  BOOST_CHECK_EQUAL(cset.count(10), 1u);
  BOOST_CHECK(cset.count(20));
  BOOST_CHECK(!cset.count(99));

  auto it = cset.find(20);
  BOOST_REQUIRE(it != cset.end());
  BOOST_CHECK_EQUAL(it->first, 20);

  auto lb = cset.lower_bound(15);
  BOOST_REQUIRE(lb != cset.end());
  BOOST_CHECK_EQUAL(lb->first, 20);

  auto ub = cset.upper_bound(20);
  BOOST_REQUIRE(ub != cset.end());
  BOOST_CHECK_EQUAL(ub->first, 30);

  auto range = cset.equal_range(20);
  BOOST_CHECK_EQUAL(range.first->first, 20);
  BOOST_CHECK_EQUAL(range.second->first, 30);

  auto begin_it = cset.begin();
  BOOST_REQUIRE(begin_it != cset.end());
  BOOST_CHECK_EQUAL(begin_it->first, 10);
}

BOOST_AUTO_TEST_CASE(bounds)
{
  vasyakin::AVLSet< int > set;

  set.insert(10);
  set.insert(20);
  set.insert(30);

  auto lb = set.lower_bound(15);
  BOOST_CHECK_EQUAL(lb->first, 20);

  auto ub = set.upper_bound(20);
  BOOST_CHECK_EQUAL(ub->first, 30);
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
  vasyakin::AVLSet< int > set = {5, 1, 3, 1, 5};
  BOOST_CHECK_EQUAL(set.size(), 3u);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AVLMultiMapTests)

BOOST_AUTO_TEST_CASE(duplicates_allowed)
{
  vasyakin::AVLMultiMap< int, std::string > multimap;

  multimap.insert({1, "one"});
  multimap.insert({1, "uno"});
  multimap.insert({1, "eins"});
  multimap.insert({2, "two"});

  BOOST_CHECK_EQUAL(multimap.size(), 2u);
  BOOST_CHECK_EQUAL(multimap.count(1), 3u);
  BOOST_CHECK_EQUAL(multimap.count(2), 1u);
  BOOST_CHECK_EQUAL(multimap.count(99), 0u);
}

BOOST_AUTO_TEST_CASE(insert_returns_iterator)
{
  vasyakin::AVLMultiMap< int, int > multimap;

  auto it = multimap.insert({10, 100});
  BOOST_CHECK(it != multimap.end());
  BOOST_CHECK_EQUAL(it->first, 10);
}

BOOST_AUTO_TEST_CASE(erase_removes_all)
{
  vasyakin::AVLMultiMap< int, int > multimap;

  multimap.insert({1, 10});
  multimap.insert({1, 20});
  multimap.insert({1, 30});

  BOOST_CHECK_EQUAL(multimap.erase(1), 3u);
  BOOST_CHECK_EQUAL(multimap.size(), 0u);
  BOOST_CHECK_EQUAL(multimap.count(1), 0u);
}

BOOST_AUTO_TEST_CASE(values_stored_in_vector)
{
  vasyakin::AVLMultiMap< int, std::string > multimap;

  multimap.insert({1, "a"});
  multimap.insert({1, "b"});
  multimap.insert({1, "c"});

  auto it = multimap.find(1);
  BOOST_REQUIRE(it != multimap.end());

  const auto& vec = it->second;
  BOOST_CHECK_EQUAL(vec.getSize(), 3u);
  BOOST_CHECK_EQUAL(vec[0], "a");
  BOOST_CHECK_EQUAL(vec[1], "b");
  BOOST_CHECK_EQUAL(vec[2], "c");
}

BOOST_AUTO_TEST_CASE(multimap_empty)
{
  vasyakin::AVLMultiMap< int, int > multimap;

  BOOST_CHECK(multimap.empty());
  BOOST_CHECK_EQUAL(multimap.size(), 0u);

  BOOST_CHECK_EQUAL(multimap.count(42), 0u);
  BOOST_CHECK(!multimap.count(42));

  BOOST_CHECK_EQUAL(multimap.erase(42), 0u);
  BOOST_CHECK(multimap.begin() == multimap.end());
}


BOOST_AUTO_TEST_CASE(multimap_const_correctness)
{
  vasyakin::AVLMultiMap< int, int > multimap;

  multimap.insert({1, 10});
  multimap.insert({1, 20});

  const auto& cmultimap = multimap;
  BOOST_CHECK_EQUAL(cmultimap.count(1), 2u);
  BOOST_CHECK(cmultimap.count(1));
  BOOST_CHECK(!cmultimap.count(99));

  auto it = cmultimap.find(1);
  BOOST_REQUIRE(it != cmultimap.end());
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second.getSize(), 2u);
}

BOOST_AUTO_TEST_CASE(bounds_and_equal_range)
{
  vasyakin::AVLMultiMap< int, int > multimap;

  multimap.insert({10, 1});
  multimap.insert({20, 2});
  multimap.insert({30, 3});

  auto lb = multimap.lower_bound(15);
  BOOST_CHECK_EQUAL(lb->first, 20);

  auto ub = multimap.upper_bound(20);
  BOOST_CHECK_EQUAL(ub->first, 30);

  auto range = multimap.equal_range(20);
  BOOST_CHECK_EQUAL(range.first->first, 20);
  BOOST_CHECK_EQUAL(range.second->first, 30);
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
  vasyakin::AVLMultiMap< int, std::string > multimap =
    {{1, "a"}, {1, "b"}, {2, "c"}};

  BOOST_CHECK_EQUAL(multimap.size(), 2u);
  BOOST_CHECK_EQUAL(multimap.count(1), 2u);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AVLMultiSetTests)

BOOST_AUTO_TEST_CASE(duplicates_allowed)
{
  vasyakin::AVLMultiSet< int > multiset;

  multiset.insert(10);
  multiset.insert(10);
  multiset.insert(10);
  multiset.insert(20);

  BOOST_CHECK_EQUAL(multiset.size(), 2u);
  BOOST_CHECK_EQUAL(multiset.count(10), 3u);
  BOOST_CHECK_EQUAL(multiset.count(20), 1u);
  BOOST_CHECK_EQUAL(multiset.count(99), 0u);
}

BOOST_AUTO_TEST_CASE(values_in_vector)
{
  vasyakin::AVLMultiSet< int > multiset;

  multiset.insert(5);
  multiset.insert(5);
  multiset.insert(5);

  auto it = multiset.find(5);
  BOOST_REQUIRE(it != multiset.end());

  const auto& vec = it->second;
  BOOST_CHECK_EQUAL(vec.getSize(), 3u);
  BOOST_CHECK_EQUAL(vec[0], 5);
  BOOST_CHECK_EQUAL(vec[1], 5);
  BOOST_CHECK_EQUAL(vec[2], 5);
}

BOOST_AUTO_TEST_CASE(erase_removes_all)
{
  vasyakin::AVLMultiSet< int > multiset;

  multiset.insert(1);
  multiset.insert(1);
  multiset.insert(2);

  BOOST_CHECK_EQUAL(multiset.erase(1), 2u);
  BOOST_CHECK_EQUAL(multiset.size(), 1u);
  BOOST_CHECK_EQUAL(multiset.count(1), 0u);
}

BOOST_AUTO_TEST_CASE(multiset_const_correctness)
{
  vasyakin::AVLMultiSet< int > multiset;

  multiset.insert(10);
  multiset.insert(10);

  const auto& cmultiset = multiset;
  BOOST_CHECK_EQUAL(cmultiset.count(10), 2u);
  BOOST_CHECK(cmultiset.count(10));

  auto it = cmultiset.find(10);
  BOOST_REQUIRE(it != cmultiset.end());
  BOOST_CHECK_EQUAL(it->first, 10);
}

BOOST_AUTO_TEST_CASE(sorted_iteration)
{
  vasyakin::AVLMultiSet< int > multiset;

  multiset.insert(30);
  multiset.insert(10);
  multiset.insert(20);
  multiset.insert(10);

  std::vector< int > keys;
  for (auto it = multiset.begin(); it != multiset.end(); ++it)
  {
    keys.push_back(it->first);
  }

  std::vector< int > expected = {10, 20, 30};

  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(),
    expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(multiset_empty)
{
  vasyakin::AVLMultiSet< int > multiset;

  BOOST_CHECK(multiset.empty());
  BOOST_CHECK_EQUAL(multiset.size(), 0u);

  BOOST_CHECK_EQUAL(multiset.count(42), 0u);
  BOOST_CHECK(!multiset.count(42));

  BOOST_CHECK_EQUAL(multiset.erase(42), 0u);
  BOOST_CHECK(multiset.begin() == multiset.end());
}

BOOST_AUTO_TEST_CASE(multiset_bounds)
{
  vasyakin::AVLMultiSet< int > multiset;

  multiset.insert(10);
  multiset.insert(20);
  multiset.insert(30);

  auto lb = multiset.lower_bound(15);
  BOOST_REQUIRE(lb != multiset.end());
  BOOST_CHECK_EQUAL(lb->first, 20);

  auto ub = multiset.upper_bound(20);
  BOOST_REQUIRE(ub != multiset.end());
  BOOST_CHECK_EQUAL(ub->first, 30);

  auto range = multiset.equal_range(20);
  BOOST_CHECK_EQUAL(range.first->first, 20);
  BOOST_CHECK_EQUAL(range.second->first, 30);
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
  vasyakin::AVLMultiSet< int > multiset = {1, 2, 2, 3, 3, 3};

  BOOST_CHECK_EQUAL(multiset.size(), 3u);
  BOOST_CHECK_EQUAL(multiset.count(3), 3u);
}

BOOST_AUTO_TEST_SUITE_END()
