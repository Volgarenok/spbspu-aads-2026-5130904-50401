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
  BOOST_CHECK_EQUAL((*it1).first, 1);
  BOOST_CHECK_EQUAL((*it1).second, "one");

  auto result2 = map.insert({1, "uno"});
  auto it2 = result2.first;
  auto ok2 = result2.second;

  BOOST_CHECK(!ok2);
  BOOST_CHECK_EQUAL((*it2).second, "one");

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
  BOOST_CHECK(!map.has(2));

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
  BOOST_CHECK_EQUAL((*lb).first, 20);

  auto ub = map.upper_bound(20);
  BOOST_CHECK_EQUAL((*ub).first, 30);

  auto range = map.equal_range(25);
  BOOST_CHECK_EQUAL((*range.first).first, 30);
  BOOST_CHECK_EQUAL((*range.second).first, 30);
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
    keys.push_back((*it).first);
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
  BOOST_CHECK_EQUAL((*it).second, 100);

  auto range = cmap.equal_range(1);
  BOOST_CHECK_EQUAL((*range.first).first, 1);
}

BOOST_AUTO_TEST_SUITE_END()
