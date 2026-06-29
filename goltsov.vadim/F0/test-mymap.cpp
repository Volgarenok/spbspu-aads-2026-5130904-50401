#include <boost/test/unit_test.hpp>
#include <string>
#include <utility>
#include "mymap.hpp"

using map_is = goltsov::Map< int, std::string >;
using map_is_it = goltsov::MapIterator< int, std::string >;
using map_is_cit = goltsov::MapConstIterator< int, std::string >;

BOOST_AUTO_TEST_SUITE(Map_suite)

BOOST_AUTO_TEST_CASE(constructor_empty_test)
{
  map_is m;
  BOOST_CHECK(m.size() == 0);
  BOOST_CHECK(m.empty());
  BOOST_CHECK(m.begin() == m.end());
}

BOOST_AUTO_TEST_CASE(insert_and_size_test)
{
  map_is m;
  auto res1 = m.insert({10, "ten"});
  BOOST_CHECK(m.size() == 1);
  BOOST_CHECK(!m.empty());
  BOOST_CHECK(res1.second == true);
  BOOST_CHECK(res1.first->first == 10);
  BOOST_CHECK(res1.first->second == "ten");
  auto res2 = m.insert({10, "duplicate"});
  BOOST_CHECK(m.size() == 1);
  BOOST_CHECK(res2.second == false);
  BOOST_CHECK(res2.first->second == "ten");
  m.insert({20, "twenty"});
  BOOST_CHECK(m.size() == 2);
}

BOOST_AUTO_TEST_CASE(operator_square_brackets_test)
{
  map_is m;
  m[10] = "ten";
  BOOST_CHECK(m.size() == 1);
  BOOST_CHECK(m[10] == "ten");
  m[10] = "updated_ten";
  BOOST_CHECK(m.size() == 1);
  BOOST_CHECK(m[10] == "updated_ten");
  BOOST_CHECK(m[20] == "");
  BOOST_CHECK(m.size() == 2);
}

BOOST_AUTO_TEST_CASE(at_test)
{
  map_is m;
  m.insert({10, "ten"});
  BOOST_CHECK(m.at(10) == "ten");
  m.at(10) = "new_ten";
  BOOST_CHECK(m.at(10) == "new_ten");
  const map_is const_m = m;
  BOOST_CHECK(const_m.at(10) == "new_ten");
  BOOST_CHECK_THROW(m.at(999), std::logic_error);
  BOOST_CHECK_THROW(const_m.at(999), std::logic_error);
}

BOOST_AUTO_TEST_CASE(contains_and_count_test)
{
  map_is m;
  m.insert({10, "ten"});
  BOOST_CHECK(m.contains(10) == true);
  BOOST_CHECK(m.contains(20) == false);
  BOOST_CHECK(m.count(10) == 1);
  BOOST_CHECK(m.count(20) == 0);
}

BOOST_AUTO_TEST_CASE(find_test)
{
  map_is m;
  m.insert({10, "ten"});
  m.insert({20, "twenty"});
  map_is_it it = m.find(10);
  BOOST_CHECK(it != m.end());
  BOOST_CHECK(it->second == "ten");
  BOOST_CHECK(m.find(999) == m.end());
  const map_is const_m = m;
  map_is_cit cit = const_m.find(20);
  BOOST_CHECK(cit != const_m.end());
  BOOST_CHECK(cit->second == "twenty");
  BOOST_CHECK(const_m.find(999) == const_m.end());
}

BOOST_AUTO_TEST_CASE(get_last_test)
{
  map_is m;
  m.insert({10, "ten"});
  m.insert({20, "twenty"});
  m.insert({5, "five"});
  map_is_it it = m.getLast();
  BOOST_CHECK(it->first == 20);
  const map_is const_m = m;
  map_is_cit cit = const_m.getLast();
  BOOST_CHECK(cit->first == 20);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  map_is m;
  m.insert({10, "ten"});
  m.insert({20, "twenty"});
  m.erase(10);
  BOOST_CHECK(m.size() == 1);
  BOOST_CHECK(m.contains(10) == false);
  BOOST_CHECK(m.erase(999) == 0);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  map_is m;
  m.insert({10, "ten"});
  m.insert({20, "twenty"});
  BOOST_CHECK(m.size() == 2);
  m.clear();
  BOOST_CHECK(m.size() == 0);
  BOOST_CHECK(m.empty());
  BOOST_CHECK(m.begin() == m.end());
}

BOOST_AUTO_TEST_CASE(copy_move_test)
{
  map_is m1;
  m1.insert({10, "ten"});
  m1.insert({20, "twenty"});
  map_is m2(m1);
  BOOST_CHECK(m2.size() == 2);
  BOOST_CHECK(m2.at(10) == "ten");
  map_is m3;
  m3 = m1;
  BOOST_CHECK(m3.size() == 2);
  map_is m4(std::move(m2));
  BOOST_CHECK(m4.size() == 2);
  map_is m5;
  m5 = std::move(m3);
  BOOST_CHECK(m5.size() == 2);
  BOOST_CHECK(m3.size() == 0);
}

BOOST_AUTO_TEST_CASE(iterators_cbegin_cend_test)
{
  map_is m;
  m.insert({10, "ten"});
  const map_is const_m = m;
  BOOST_CHECK(const_m.begin() == const_m.cbegin());
  BOOST_CHECK(const_m.end() == const_m.cend());
}

BOOST_AUTO_TEST_SUITE_END()
