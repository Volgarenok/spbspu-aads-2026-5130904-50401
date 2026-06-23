#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

#include "HashTable.hpp"

struct LongComp {
  bool operator()(const long a, const long b) const
  {
    return a == b;
  }
};

BOOST_AUTO_TEST_CASE(Default_Construct_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  BOOST_TEST(map.size() == 0);
  BOOST_TEST(map.capacity() == 16);
}

BOOST_AUTO_TEST_CASE(Copy_Construct_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  long keys[]{ 1, 2, 3, 4 };
  for (size_t i = 0; i < 4; ++i) {
    map.add(keys[i], i * 2);
  }
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(map);
  BOOST_TEST(map.size() == map2.size());
  BOOST_TEST(map.capacity() == map2.capacity());
  for (size_t i = 0; i < 4; ++i) {
    BOOST_TEST(map.at(keys[i]) == map2.at(keys[i]));
  }
}

BOOST_AUTO_TEST_CASE(Move_Construct_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(32);
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(std::move(map));
  BOOST_TEST(map.capacity() == 0);
  BOOST_TEST(map2.capacity() == 32);
}

BOOST_AUTO_TEST_CASE(Copy_Assignment_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(16);
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2;

  map2 = map;

  BOOST_TEST(map2.capacity() == map.capacity());
  BOOST_TEST(map2.size() == map.size());
}

BOOST_AUTO_TEST_CASE(Move_Assignment_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(32);
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2;
  size_t s = map.size();
  map2 = std::move(map);

  BOOST_TEST(map2.capacity() == 32);
  BOOST_TEST(map2.size() == s);
  BOOST_TEST(map.capacity() == 0);
  BOOST_TEST(map.size() == 0);
}

BOOST_AUTO_TEST_CASE(Add_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(2);
  BOOST_TEST(map.size() == 0);
  map.add(1, 3);
  BOOST_TEST(map.size() == 1);
  BOOST_CHECK_THROW(map.add(1, 4), std::logic_error);
  map.add(2, 2);
  BOOST_TEST(map.size() == 2);
  map.add(3, 4);
  BOOST_TEST(map.capacity() == 4);
}

BOOST_AUTO_TEST_CASE(Has_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 21);
  map.add(2, 34);
  BOOST_TEST(map.contains(1) == true);
  BOOST_TEST(map.contains(3) == false);
  BOOST_TEST(map.contains(2) == true);
}

BOOST_AUTO_TEST_CASE(Drop_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 21);
  map.add(2, 34);
  BOOST_TEST(map.size() == 2);
  map.remove(1);
  BOOST_TEST(map.size() == 1);
  BOOST_CHECK_THROW(map.remove(3), std::logic_error);
  map.remove(2);
  BOOST_TEST(map.size() == 0);
  BOOST_CHECK_THROW(map.remove(3), std::logic_error);
}

BOOST_AUTO_TEST_CASE(Rehash_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(4);
  map.add(1, 2);
  map.add(2, 3);
  map.add(3, 4);
  BOOST_TEST(map.capacity() == 4);
  map.rehash();
  BOOST_TEST(map.capacity() == 8);
  BOOST_TEST(map.size() == 3);
}

BOOST_AUTO_TEST_CASE(At_Method_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 34);
  map.add(2, 52);
  map.add(3, 67);
  BOOST_TEST(map.at(1) == 34);
  BOOST_TEST(map.at(2) == 52);
  BOOST_TEST(map.at(3) == 67);
  BOOST_CHECK_THROW(map.at(4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(At_Const_Method_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 34);
  map.add(2, 52);
  map.add(3, 67);
  const kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(map);
  BOOST_TEST(map2.at(1) == 34);
  BOOST_TEST(map2.at(2) == 52);
  BOOST_TEST(map2.at(3) == 67);
  BOOST_CHECK_THROW(map2.at(4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Operator_Get_Access_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 34);
  map.add(2, 52);
  map[3] = 67;
  BOOST_TEST(map[1] == 34);
  BOOST_TEST(map[2] == 52);
  BOOST_TEST(map[3] == 67);
}

BOOST_AUTO_TEST_CASE(Const_Operator_Get_Access)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map[1] = 34;
  map[2] = 52;
  map[3] = 67;
  const kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(map);
  BOOST_TEST(map2[1] == 34);
  BOOST_TEST(map2[2] == 52);
  BOOST_TEST(map2[3] == 67);
}

BOOST_AUTO_TEST_CASE(Const_Find_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map[1] = 34;
  map[2] = 52;
  map[3] = 67;
  const kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(map);
  BOOST_TEST(map.find(1)->second == 34);
  BOOST_TEST(map.find(2)->second == 52);
  BOOST_TEST(map.find(3)->second == 67);
}

BOOST_AUTO_TEST_CASE(Iterators_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 34);
  map.add(2, 52);
  map.add(3, 67);
  long keys[]{ 1, 2, 3 };
  int values[]{ 34, 52, 67 };
  size_t i = 0;
  kuznetsov::Iterator< long, int, true > it = map.cbegin();
  for (; i < 3; ++i) {
    BOOST_TEST(it->first == keys[i]);
    BOOST_TEST((*it).second == values[i]);
    ++it;
  }
}
