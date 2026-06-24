#include <boost/test/unit_test.hpp>
#include <string>
#include "robinHoodTable.hpp"

using namespace tarasenko;

using HTable = RobinHoodTable< int, int >;

struct ConstHash
{
  size_t operator()(int) const
  {
    return 777;
  }
};

using CollisionTable = RobinHoodTable< int, int, ConstHash >;

BOOST_AUTO_TEST_CASE(hood_default_constructor)
{
  HTable table;
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0ul);
  BOOST_TEST(table.bucket_count() > 0ul);
}

BOOST_AUTO_TEST_CASE(bucket_count_constructor)
{
  HTable table(16);
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0ul);
  BOOST_TEST(table.bucket_count() == 16ul);
}

BOOST_AUTO_TEST_CASE(empty_table)
{
  HTable table(0);
  BOOST_TEST(table.bucket_count() == 1ul);
}

BOOST_AUTO_TEST_CASE(insert)
{
  HTable table;
  BOOST_TEST(table.insert({1, 1}));
  BOOST_TEST(table.size() == 1ul);
  BOOST_TEST(!table.empty());
}

BOOST_AUTO_TEST_CASE(find)
{
  HTable table;
  table.insert({1, 1});
  BOOST_CHECK(table.find(1) != table.end());
  BOOST_TEST(table.find(1)->second == 1);
  BOOST_TEST(table.size() == 1ul);
}

BOOST_AUTO_TEST_CASE(find_empty)
{
  HTable table;
  BOOST_CHECK(table.find(1) == table.end());
}

BOOST_AUTO_TEST_CASE(count)
{
  HTable table;
  table.insert({1, 1});
  BOOST_TEST(table.count(1) == 1ul);
  BOOST_TEST(table.count(2) == 0ul);
}

BOOST_AUTO_TEST_CASE(at)
{
  HTable table;
  table.insert({1, 1});
  BOOST_TEST(table.at(1) == 1);
}

BOOST_AUTO_TEST_CASE(at_empty)
{
  HTable table;
  BOOST_CHECK_THROW(table.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(index_operator)
{
  HTable table;
  table[1] = 10;
  BOOST_TEST(table.size() == 1ul);
  BOOST_TEST(table.at(1) == 10);
}

BOOST_AUTO_TEST_CASE(default_template_parameters)
{
  RobinHoodTable< int, std::string > table;
  table.insert({1, "hello"});
  BOOST_TEST(table.find(1)->second == "hello");
}

BOOST_AUTO_TEST_CASE(insert_elems_by_same_key)
{
  HTable table;
  BOOST_TEST(table.insert({1, 1}));
  BOOST_TEST(!table.insert({1, 2}));
  BOOST_TEST(table.size() == 1ul);
  BOOST_TEST(table.find(1)->second == 1);
}

BOOST_AUTO_TEST_CASE(collision_insert_and_find)
{
  CollisionTable table;
  table.insert({1, 10});
  table.insert({2, 20});
  table.insert({3, 30});
  BOOST_TEST(table.size() == 3ul);
  BOOST_TEST(table.find(1)->second == 10);
  BOOST_TEST(table.find(2)->second == 20);
  BOOST_TEST(table.find(3)->second == 30);
}

BOOST_AUTO_TEST_CASE(begin_empty)
{
  HTable table;
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(dereference)
{
  HTable table;
  table.insert({1, 2});
  auto it = table.begin();
  BOOST_CHECK(*it == std::make_pair(1, 2));
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 2);
}

BOOST_AUTO_TEST_CASE(pre_increment)
{
  HTable table;
  table.insert({1, 2});
  auto it = table.begin();
  BOOST_CHECK(++it == table.end());
}

BOOST_AUTO_TEST_CASE(post_increment)
{
  HTable table;
  table.insert({1, 2});
  auto it = table.begin();
  BOOST_CHECK(it++ == table.begin());
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(cycle_by_iterators)
{
  HTable table;
  table.insert({1, 10});
  table.insert({2, 20});
  table.insert({3, 30});
  int i = 0;
  for (auto it = table.begin(); it != table.end(); ++it, ++i)
  {
    BOOST_TEST(it->second == it->first * 10);
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(const_begin_empty)
{
  const HTable table;
  BOOST_CHECK(table.cbegin() == table.cend());
}

BOOST_AUTO_TEST_CASE(const_dereference)
{
  HTable table;
  table.insert({1, 2});
  const HTable& ctable = table;
  auto it = ctable.cbegin();
  BOOST_CHECK(*it == std::make_pair(1, 2));
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == 2);
}

BOOST_AUTO_TEST_CASE(const_pre_increment)
{
  HTable table;
  table.insert({1, 2});
  const HTable& ctable = table;
  auto it = ctable.cbegin();
  BOOST_CHECK(++it == ctable.cend());
}

BOOST_AUTO_TEST_CASE(const_post_increment)
{
  HTable table;
  table.insert({1, 2});
  const HTable& ctable = table;
  auto it = ctable.cbegin();
  BOOST_CHECK(it++ == ctable.cbegin());
  BOOST_CHECK(it == ctable.cend());
}

BOOST_AUTO_TEST_CASE(cycle_by_const_iterators)
{
  HTable table;
  table.insert({1, 10});
  table.insert({2, 20});
  table.insert({3, 30});
  const HTable& ctable = table;
  int i = 0;
  for (auto it = ctable.cbegin(); it != ctable.cend(); ++it, ++i)
  {
    BOOST_TEST(it->second == it->first * 10);
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(rehash)
{
  HTable table;
  table.insert({1, 1});
  table.rehash(16);
  BOOST_TEST(table.bucket_count() == 16ul);
  BOOST_TEST(table.size() == 1ul);
  BOOST_TEST(table.find(1)->second == 1);
}

BOOST_AUTO_TEST_CASE(rehash_empty)
{
  HTable table(4);
  table.rehash(0);
  BOOST_TEST(table.bucket_count() == 1ul);
  BOOST_TEST(table.size() == 0ul);
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(rehash_many_elems)
{
  HTable table(32);
  for (int i = 0; i < 20; ++i)
  {
    table.insert({i, i * 5});
  }
  table.rehash(64);
  BOOST_TEST(table.bucket_count() == 64ul);
  BOOST_TEST(table.size() == 20ul);
  for (int i = 0; i < 20; ++i)
  {
    BOOST_TEST(table.find(i)->second == i * 5);
  }
}

BOOST_AUTO_TEST_CASE(load_factor)
{
  HTable table(8);
  table.insert({1, 1});
  table.insert({2, 2});
  BOOST_TEST(table.load_factor() == 0.25);
}

BOOST_AUTO_TEST_CASE(max_load_factor)
{
  HTable table;
  BOOST_TEST(table.max_load_factor() == 0.75);
  table.max_load_factor(0.5);
  BOOST_TEST(table.max_load_factor() == 0.5);
}

BOOST_AUTO_TEST_CASE(reserve)
{
  HTable table(2);
  table.insert({1, 1});
  table.reserve(20);
  BOOST_TEST(table.bucket_count() > 2ul);
  BOOST_TEST(table.size() == 1ul);
  BOOST_TEST(table.find(1)->second == 1);
}

BOOST_AUTO_TEST_CASE(table_clear)
{
  HTable table;
  table.insert({1, 1});
  table.insert({2, 2});
  table.clear();
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0ul);
  BOOST_CHECK(table.find(1) == table.end());
}

BOOST_AUTO_TEST_CASE(erase)
{
  HTable table;
  table.insert({1, 1});
  BOOST_TEST(table.erase(1) == 1ul);
  BOOST_TEST(table.size() == 0ul);
  BOOST_CHECK(table.find(1) == table.end());
}

BOOST_AUTO_TEST_CASE(erase_missing)
{
  HTable table;
  table.insert({1, 1});
  BOOST_TEST(table.erase(2) == 0ul);
  BOOST_TEST(table.size() == 1ul);
}

BOOST_AUTO_TEST_CASE(collision_erase)
{
  CollisionTable table(8);
  table.insert({1, 10});
  table.insert({2, 20});
  table.insert({3, 30});
  BOOST_TEST(table.erase(2) == 1ul);
  BOOST_TEST(table.size() == 2ul);
  BOOST_CHECK(table.find(2) == table.end());
  BOOST_TEST(table.find(1)->second == 10);
  BOOST_TEST(table.find(3)->second == 30);
}
