#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <string>
#include <vector>
#include "hash_table.hpp"

BOOST_AUTO_TEST_SUITE(hash_table_suite)

BOOST_AUTO_TEST_CASE(test_create_hash_table)
{
  hvostov::HashTable< int, std::string > table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_contains)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("apple", 5);
  BOOST_CHECK(!table.empty());
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.contains("apple"));
  BOOST_CHECK(!table.contains("banana"));
}

BOOST_AUTO_TEST_CASE(test_add_multiple_elements)
{
  hvostov::HashTable< int, std::string > table(3);
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK(table.contains(3));
}

BOOST_AUTO_TEST_CASE(test_update_existing_key)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("key", 10);
  BOOST_CHECK_EQUAL(table.at("key"), 10);
  table.add("key", 20);
  BOOST_CHECK_EQUAL(table.at("key"), 20);
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_at_method)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("first", 1);
  table.add("second", 2);
  BOOST_CHECK_EQUAL(table.at("first"), 1);
  BOOST_CHECK_EQUAL(table.at("second"), 2);
  table.at("first") = 100;
  BOOST_CHECK_EQUAL(table.at("first"), 100);
}

BOOST_AUTO_TEST_CASE(test_at_throws_on_missing_key)
{
  hvostov::HashTable< int, std::string > table(1);
  BOOST_CHECK_THROW(table.at(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets)
{
  hvostov::HashTable< std::string, std::string > table(1);
  table["hello"] = "world";
  BOOST_CHECK_EQUAL(table["hello"], "world");
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets_creates_default)
{
  hvostov::HashTable< int, int > table(10);
  BOOST_CHECK_EQUAL(table[5], 0);
  BOOST_CHECK(table.contains(5));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_remove_existing_key)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("apple", 5);
  table.add("banana", 3);
  bool result = table.remove("apple");
  BOOST_CHECK(result);
  BOOST_CHECK(!table.contains("apple"));
  BOOST_CHECK(table.contains("banana"));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_remove_missing_key)
{
  hvostov::HashTable< int, std::string > table(0);
  bool result = table.remove(42);
  BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_CASE(test_find_existing_key)
{
  hvostov::HashTable< std::string, int > table(10);
  table.add("one", 1);
  table.add("two", 2);
  auto it = table.find("one");
  BOOST_CHECK(it != table.end());
  BOOST_CHECK_EQUAL(it->second, 1);
}

BOOST_AUTO_TEST_CASE(test_find_missing_key)
{
  hvostov::HashTable< int, std::string > table(10);
  auto it = table.find(42);
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::HashTable< int, std::string > table(3);
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  BOOST_CHECK_EQUAL(table.size(), 3);
  table.clear();
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK(!table.contains(1));
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
  hvostov::HashTable< int, int > table(2);
  size_t old_cap = table.getCapacity();
  for (size_t i = 0; i < old_cap; ++i) {
    table.add(i, i * 10);
  }
  size_t old_size = table.size();
  table.rehash(20);
  size_t new_cap = table.getCapacity();
  BOOST_CHECK(new_cap > old_cap);
  BOOST_CHECK_EQUAL(table.size(), old_size);
  for (size_t i = 0; i < old_size; ++i) {
    BOOST_CHECK(table.contains(i));
    BOOST_CHECK_EQUAL(table.at(i), i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  hvostov::HashTable< std::string, int > table1(10);
  table1.add("one", 1);
  table1.add("two", 2);
  table1.add("three", 3);

  hvostov::HashTable< std::string, int > table2(table1);
  BOOST_CHECK_EQUAL(table2.size(), 3);
  BOOST_CHECK(table2.contains("one"));
  BOOST_CHECK(table2.contains("two"));
  BOOST_CHECK(table2.contains("three"));
  BOOST_CHECK_EQUAL(table2.at("one"), 1);
  BOOST_CHECK_EQUAL(table2.at("two"), 2);
  BOOST_CHECK_EQUAL(table2.at("three"), 3);

  table2.add("four", 4);
  BOOST_CHECK(!table1.contains("four"));
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  hvostov::HashTable< int, std::string > table1(10);
  table1.add(1, "one");
  table1.add(2, "two");
  hvostov::HashTable< int, std::string > table2(std::move(table1));
  BOOST_CHECK_EQUAL(table2.size(), 2);
  BOOST_CHECK(table2.contains(1));
  BOOST_CHECK(table2.contains(2));
  BOOST_CHECK(table1.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  hvostov::HashTable< std::string, int > table1(10);
  table1.add("a", 1);
  table1.add("b", 2);
  hvostov::HashTable< std::string, int > table2(20);
  table2.add("c", 3);
  table2 = table1;
  BOOST_CHECK_EQUAL(table2.size(), 2);
  BOOST_CHECK(table2.contains("a"));
  BOOST_CHECK(table2.contains("b"));
  BOOST_CHECK(!table2.contains("c"));
  BOOST_CHECK_EQUAL(table2.at("a"), 1);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  hvostov::HashTable< int, std::string > table1(10);
  table1.add(1, "one");
  hvostov::HashTable< int, std::string > table2(20);
  table2.add(2, "two");
  table2 = std::move(table1);
  BOOST_CHECK_EQUAL(table2.size(), 1);
  BOOST_CHECK(table2.contains(1));
  BOOST_CHECK(!table2.contains(2));
  BOOST_CHECK(table1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  hvostov::HashTable< std::string, int > table1(10);
  table1.add("first", 1);
  hvostov::HashTable< std::string, int > table2(10);
  table2.add("second", 2);
  table1.swap(table2);
  BOOST_CHECK(table1.contains("second"));
  BOOST_CHECK(table2.contains("first"));
  BOOST_CHECK(!table1.contains("first"));
  BOOST_CHECK(!table2.contains("second"));
}

BOOST_AUTO_TEST_CASE(test_empty_table_operations)
{
  hvostov::HashTable< int, std::string > table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_THROW(table.at(1), std::out_of_range);
  BOOST_CHECK(!table.remove(1));
  BOOST_CHECK(!table.contains(1));
}

BOOST_AUTO_TEST_CASE(test_different_key_types)
{
  hvostov::HashTable< double, std::string > table(10);
  table.add(3.14, "pi");
  table.add(2.71, "e");
  BOOST_CHECK(table.contains(3.14));
  BOOST_CHECK(table.contains(2.71));
  BOOST_CHECK_EQUAL(table.at(3.14), "pi");
}

BOOST_AUTO_TEST_CASE(test_large_number_of_elements)
{
  size_t N = 1000;
  hvostov::HashTable< int, int > table(N);
  for (size_t i = 0; i < N; ++i) {
    table.add(i, i * i);
  }
  BOOST_CHECK_EQUAL(table.size(), N);
  for (size_t i = 0; i < N; ++i) {
    BOOST_CHECK_EQUAL(table.at(i), i * i);
  }
}

BOOST_AUTO_TEST_CASE(test_remove_all_elements)
{
  hvostov::HashTable< int, std::string > table(10);
  std::vector< int > keys = {1, 2, 3, 4, 5};
  for (int key : keys) {
    table.add(key, "val" + std::to_string(key));
  }
  for (int key : keys) {
    BOOST_CHECK(table.contains(key));
    bool result = table.remove(key);
    BOOST_CHECK(result);
  }
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_load_factor)
{
  hvostov::HashTable< int, int > table(10);
  BOOST_CHECK(table.loadFactor() >= 0.0);
  for (int i = 0; i < 5; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK(table.loadFactor() > 0.0);
}

BOOST_AUTO_TEST_CASE(test_load_factor_empty)
{
  hvostov::HashTable< int, int > table;
  BOOST_CHECK_EQUAL(table.loadFactor(), 0.0);
}

BOOST_AUTO_TEST_CASE(test_average_bucket_size)
{
  hvostov::HashTable< int, int > table(10);
  BOOST_CHECK(table.averageBucketSize() >= 0.0);
  for (int i = 0; i < 5; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK(table.averageBucketSize() > 0.0);
}

BOOST_AUTO_TEST_CASE(test_average_bucket_size_empty)
{
  hvostov::HashTable< int, int > table;
  BOOST_CHECK_EQUAL(table.averageBucketSize(), 0.0);
}

BOOST_AUTO_TEST_CASE(test_overflow_size)
{
  hvostov::HashTable< int, int > table(2);
  BOOST_CHECK_EQUAL(table.overflowSize(), 0);
  table.maxLoadFactor(0.99);
  table.maxAverageBucketSize(100);
  table.maxOverflowSize(100);
  size_t cap = table.getCapacity();
  for (size_t i = 0; i < cap + 5; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK(table.overflowSize() > 0);
}

BOOST_AUTO_TEST_CASE(test_overflow_size_empty)
{
  hvostov::HashTable< int, int > table;
  BOOST_CHECK_EQUAL(table.overflowSize(), 0);
}

BOOST_AUTO_TEST_CASE(test_max_bucket_size)
{
  hvostov::HashTable< int, int > table(4);
  for (int i = 0; i < 10; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK(table.maxBucketSize() > 0);
}

BOOST_AUTO_TEST_CASE(test_max_bucket_size_empty)
{
  hvostov::HashTable< int, int > table;
  BOOST_CHECK_EQUAL(table.maxBucketSize(), 0);
}

BOOST_AUTO_TEST_CASE(test_max_load_factor)
{
  hvostov::HashTable< int, int > table(1);
  table.maxLoadFactor(0.5);
  BOOST_CHECK_EQUAL(table.maxLoadFactor(), 0.5);
  BOOST_CHECK_THROW(table.maxLoadFactor(0.0), std::logic_error);
  BOOST_CHECK_THROW(table.maxLoadFactor(1.0), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_max_average_bucket_size)
{
  hvostov::HashTable< int, int > table(1);
  table.maxAverageBucketSize(2.5);
  BOOST_CHECK_EQUAL(table.maxAverageBucketSize(), 2.5);
}

BOOST_AUTO_TEST_CASE(test_max_overflow_size)
{
  hvostov::HashTable< int, int > table(1);
  table.maxOverflowSize(2);
  BOOST_CHECK_EQUAL(table.maxOverflowSize(), 2);
}

BOOST_AUTO_TEST_CASE(test_set_buckets_updater)
{
  hvostov::HashTable< int, int > table(1);
  table.setBucketsUpdater([](size_t before) { return before == 0 ? 4 : before * 2; });
  for (int i = 0; i < 20; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK(table.size() == 20);
  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_set_bucket_size_updater)
{
  hvostov::HashTable< int, int > table(1);
  table.setBucketSizeUpdater([](size_t before) { return before + 2; });
  for (int i = 0; i < 30; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK(table.size() == 30);
  for (int i = 0; i < 30; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_auto_rehash_on_load_factor)
{
  hvostov::HashTable< int, int > table(1);
  table.maxLoadFactor(0.3);
  table.maxAverageBucketSize(100);
  table.maxOverflowSize(100);
  for (int i = 0; i < 20; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 20);
  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_auto_rehash_on_average_bucket_size)
{
  hvostov::HashTable< int, int > table(1);
  table.maxLoadFactor(0.99);
  table.maxAverageBucketSize(2.0);
  table.maxOverflowSize(100);
  for (int i = 0; i < 20; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 20);
  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_auto_rehash_on_overflow_size)
{
  hvostov::HashTable< int, int > table(1);
  table.maxLoadFactor(0.99);
  table.maxAverageBucketSize(100);
  table.maxOverflowSize(2);
  for (int i = 0; i < 20; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 20);
  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_add_remove_add)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("key", 10);
  table.remove("key");
  table.add("key", 20);
  BOOST_CHECK_EQUAL(table.at("key"), 20);
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_rehash_preserves_elements)
{
  hvostov::HashTable< int, int > table(1);
  for (int i = 0; i < 50; ++i) {
    table.add(i, i * 10);
  }
  table.rehash(100);
  BOOST_CHECK_EQUAL(table.size(), 50);
  for (int i = 0; i < 50; ++i) {
    BOOST_CHECK(table.contains(i));
    BOOST_CHECK_EQUAL(table.at(i), i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_copy_empty_table)
{
  hvostov::HashTable< int, std::string > table1;
  hvostov::HashTable< int, std::string > table2(table1);
  BOOST_CHECK(table2.empty());
  BOOST_CHECK_EQUAL(table2.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_empty_table)
{
  hvostov::HashTable< int, std::string > table1;
  hvostov::HashTable< int, std::string > table2(std::move(table1));
  BOOST_CHECK(table2.empty());
  BOOST_CHECK(table1.empty());
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  hvostov::HashTable< int, int > table(5);
  table.add(1, 10);
  table = table;
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), 10);
}

BOOST_AUTO_TEST_CASE(test_find_on_empty_table)
{
  hvostov::HashTable< int, std::string > table;
  BOOST_CHECK(table.find(42) == table.end());
}

BOOST_AUTO_TEST_CASE(test_remove_from_empty_table)
{
  hvostov::HashTable< int, std::string > table;
  BOOST_CHECK(!table.remove(42));
}

BOOST_AUTO_TEST_CASE(test_contains_on_empty_table)
{
  hvostov::HashTable< int, std::string > table;
  BOOST_CHECK(!table.contains(42));
}

BOOST_AUTO_TEST_CASE(test_rehash_on_empty_table)
{
  hvostov::HashTable< int, int > table;
  table.rehash(10);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_clear_empty_table)
{
  hvostov::HashTable< int, int > table;
  table.clear();
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_many_collisions)
{
  hvostov::HashTable< int, int > table(1);
  table.maxLoadFactor(0.9);
  for (int i = 0; i < 100; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 100);
  for (int i = 0; i < 100; ++i) {
    BOOST_CHECK(table.contains(i));
    BOOST_CHECK_EQUAL(table.at(i), i);
  }
}

BOOST_AUTO_TEST_CASE(test_get_capacity)
{
  hvostov::HashTable< int, int > table(5);
  BOOST_CHECK(table.getCapacity() > 0);
}

BOOST_AUTO_TEST_CASE(test_rehash_with_custom_bucket_size)
{
  hvostov::HashTable< int, int > table(1);
  table.rehash(10, 8);
  BOOST_CHECK(table.getCapacity() > 0);
  for (int i = 0; i < 50; ++i) {
    table.add(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 50);
  for (int i = 0; i < 50; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_SUITE_END()
