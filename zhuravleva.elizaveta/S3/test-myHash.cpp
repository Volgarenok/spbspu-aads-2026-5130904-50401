#include <boost/test/unit_test.hpp>
#include <functional>
#include <stdexcept>
#include "myHash.hpp"
#include "hasher.hpp"

using TestTable = zhuravleva::HashTable<
    std::string, int,
    zhuravleva::Blake2Hasher< std::string >,
    std::equal_to< std::string > >;

BOOST_AUTO_TEST_CASE(hash_table_add_get_has_test)
{
  TestTable table;
  BOOST_TEST(table.empty());
  table.add("one", 1);
  table.add("two", 2);
  BOOST_TEST(table.size() == 2);
  BOOST_TEST(table.has("one"));
  BOOST_TEST(table.has("two"));
  BOOST_TEST(!table.has("three"));
  BOOST_TEST(table.get("one") == 1);
  BOOST_TEST(table.get("two") == 2);
}

BOOST_AUTO_TEST_CASE(hash_table_replace_value_test)
{
  TestTable table;
  table.add("key", 1);
  table.add("key", 10);
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.get("key") == 10);
}

BOOST_AUTO_TEST_CASE(hash_table_drop_test)
{
  TestTable table;
  table.add("a", 1);
  table.add("b", 2);
  BOOST_TEST(table.drop("a"));
  BOOST_TEST(!table.has("a"));
  BOOST_TEST(table.has("b"));
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(!table.drop("a"));
}

BOOST_AUTO_TEST_CASE(hash_table_get_missing_key_test)
{
  TestTable table;
  table.add("a", 1);
  BOOST_CHECK_THROW(table.get("b"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(hash_table_rehash_test)
{
  TestTable table(2);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.rehash(16);
  BOOST_TEST(table.bucket_count() == 16);
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.get("a") == 1);
  BOOST_TEST(table.get("b") == 2);
  BOOST_TEST(table.get("c") == 3);
}

BOOST_AUTO_TEST_CASE(hash_table_iterator_test)
{
  TestTable table;
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  size_t count = 0;
  for (TestTable::Iterator it = table.begin(); it != table.end(); ++it)
  {
    count++;
  }
  BOOST_TEST(count == 3);
}
