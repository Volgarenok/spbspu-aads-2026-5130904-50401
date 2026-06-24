#include <boost/test/unit_test.hpp>
#include <string>
#include "hash_table.hpp"
#include "hmac_hash.hpp"

namespace
{
  using HTable = tarasenko::HashTable< int, int >;

  struct ConstHash
  {
    size_t operator()(int) const
    {
      return 777;
    }
  };

  using CollisionTable = tarasenko::HashTable< int, int, ConstHash >;
}

BOOST_AUTO_TEST_CASE(default_constructor)
{
  HTable table;
  BOOST_TEST(table.size() == 0);
  BOOST_TEST(table.capacity() == 64);
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(add)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(drop)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(!table.drop(2));
  BOOST_TEST(table.drop(1));
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(get)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(table.at(1) == 1);
  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(get_empty)
{
  HTable table;
  BOOST_CHECK_THROW(table.at(1), std::runtime_error);
  table.add(1, 1);
  table.drop(1);
  BOOST_CHECK_THROW(table.at(1), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(has)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(table.has(1));
  BOOST_TEST(!table.has(2));
}

BOOST_AUTO_TEST_CASE(rehash)
{
  HTable table;
  table.add(1, 1);
  table.rehash(128);
  BOOST_TEST(table.capacity() == 128);
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.has(1));
}

BOOST_AUTO_TEST_CASE(rehash_many_elems)
{
  HTable table(2);
  for (int i = 0; i < 20; ++i)
  {
    table.add(i, i * 5);
  }
  HTable copy = table;
  table.rehash(77);
  BOOST_TEST(table.capacity() == 77);
  BOOST_TEST(table.size() == 20);
  for (int i = 0; i < 20; ++i)
  {
    BOOST_TEST(table.at(i) == i * 5);
  }
  BOOST_CHECK(table == copy);
}

BOOST_AUTO_TEST_CASE(default_template_parameters)
{
  tarasenko::HashTable< int, std::string > table;
  table.add(1, "hello");
  BOOST_TEST(table.at(1) == "hello");
}

BOOST_AUTO_TEST_CASE(trying_to_create_empty_table)
{
  HTable table(0);
  BOOST_TEST(table.capacity() == 1);
}

BOOST_AUTO_TEST_CASE(add_elems_by_same_key)
{
  HTable table;
  table.add(1, 1);
  table.add(1, 2);
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.at(1) == 1);
}

BOOST_AUTO_TEST_CASE(collision_add_and_get)
{
  CollisionTable table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.at(1) == 10);
  BOOST_TEST(table.at(2) == 20);
  BOOST_TEST(table.at(3) == 30);
}

BOOST_AUTO_TEST_CASE(collision_drop)
{
  CollisionTable table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);
  BOOST_TEST(table.drop(2));
  BOOST_TEST(table.size() == 2);
  BOOST_TEST(!table.has(2));
  BOOST_TEST(table.at(1) == 10);
  BOOST_TEST(table.at(3) == 30);
}

BOOST_AUTO_TEST_CASE(swap)
{
  HTable table1;
  HTable table2;
  table1.add(1, 1);
  table2.add(2, 2);
  table1.swap(table2);
  BOOST_TEST(table1.at(2) == 2);
  BOOST_TEST(table2.at(1) == 1);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2(table1);
  BOOST_TEST(table2.at(1) == 1);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2(std::move(table1));
  BOOST_TEST(table2.at(1) == 1);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2;
  table2 = table1;
  BOOST_TEST(table2.at(1) == 1);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  HTable table1;
  table1.add(1, 1);
  HTable table2;
  table2 = std::move(table1);
  BOOST_TEST(table2.at(1) == 1);
}

BOOST_AUTO_TEST_CASE(begin)
{
  HTable table;
  table.add(1, 2);
  BOOST_CHECK(table.begin() != table.end());
}

BOOST_AUTO_TEST_CASE(begin_empty)
{
  HTable table;
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(dereference)
{
  HTable table;
  table.add(1, 2);
  auto it = table.begin();
  BOOST_CHECK(*it == std::make_pair(1, 2));
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 2);
}

BOOST_AUTO_TEST_CASE(pre_increment)
{
  HTable table;
  table.add(1, 2);
  auto it = table.begin();
  BOOST_CHECK(++it == table.end());
}

BOOST_AUTO_TEST_CASE(post_increment)
{
  HTable table;
  table.add(1, 2);
  auto it = table.begin();
  BOOST_CHECK(it++ == table.begin());
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(cycle_by_iterators)
{
  HTable table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);
  int i = 0;
  for (auto it = table.begin(); it != table.end(); ++it, ++i)
  {
    BOOST_TEST(it->second == it->first * 10);
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(comparison_operator_for_table)
{
  tarasenko::HmacHash< int > hash1("some_key");
  tarasenko::HmacHash< int > hash2("another_key");
  HTable table1(64, hash1);
  HTable table2(64, hash2);
  for (int i = 0; i < 10; ++i)
  {
    table1.add(i, i * 67);
    table2.add(i, i * 67);
  }
  BOOST_CHECK(table1 == table2);
  table2.drop(3);
  BOOST_CHECK(table1 != table2);
  table2.add(10, 10 * 67);
  BOOST_CHECK(table1 != table2);
}

BOOST_AUTO_TEST_CASE(cbegin)
{
  HTable table;
  table.add(1, 2);
  BOOST_CHECK(table.cbegin() != table.cend());
}

BOOST_AUTO_TEST_CASE(cbegin_empty)
{
  HTable table;
  BOOST_CHECK(table.cbegin() == table.cend());
}

BOOST_AUTO_TEST_CASE(cdereference)
{
  HTable table;
  table.add(1, 2);
  auto it = table.cbegin();
  BOOST_CHECK(*it == std::make_pair(1, 2));
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 2);
}

BOOST_AUTO_TEST_CASE(cpre_increment)
{
  HTable table;
  table.add(1, 2);
  auto it = table.cbegin();
  BOOST_CHECK(++it == table.cend());
}

BOOST_AUTO_TEST_CASE(cpost_increment)
{
  HTable table;
  table.add(1, 2);
  auto it = table.cbegin();
  BOOST_CHECK(it++ == table.cbegin());
  BOOST_CHECK(it == table.cend());
}

BOOST_AUTO_TEST_CASE(ccycle_by_iterators)
{
  HTable table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);
  int i = 0;
  for (auto it = table.cbegin(); it != table.cend(); ++it, ++i)
  {
    BOOST_TEST(it->second == it->first * 10);
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(square_brackets)
{
  HTable table;
  int key = 1;
  BOOST_TEST(table[key] == 0);
  BOOST_TEST(table.size() == 1);
  table[key] = 10;
  BOOST_TEST(table.at(1) == 10);
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table[2] == 0);
  BOOST_TEST(table.size() == 2);
  table[2] = 20;
  BOOST_TEST(table.at(2) == 20);
}
