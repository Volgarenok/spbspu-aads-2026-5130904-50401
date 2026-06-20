#include "robin_hood_hash_table.hpp"
#include <cstddef>
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(robin_hood_hash_table_suite)

BOOST_AUTO_TEST_CASE(test_create_empty_table)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_EQUAL(table.capacity(), 0);
}

BOOST_AUTO_TEST_CASE(test_create_with_capacity)
{
  hvostov::RobinHoodHashTable< int, std::string > table(100);
  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.capacity() >= 100);
}

BOOST_AUTO_TEST_CASE(test_create_with_small_capacity)
{
  hvostov::RobinHoodHashTable< int, std::string > table(2);
  BOOST_CHECK(table.capacity() >= 8);
}

BOOST_AUTO_TEST_CASE(test_emplace_and_contains)
{
  hvostov::RobinHoodHashTable< std::string, int > table;
  table.emplace("apple", 5);
  BOOST_CHECK(!table.empty());
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.contains("apple"));
  BOOST_CHECK(!table.contains("banana"));
}

BOOST_AUTO_TEST_CASE(test_emplace_multiple_elements)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  table.emplace(2, "two");
  table.emplace(3, "three");
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK(table.contains(3));
}

BOOST_AUTO_TEST_CASE(test_emplace_update_existing_key)
{
  hvostov::RobinHoodHashTable< std::string, int > table;
  table.emplace("key", 10);
  BOOST_CHECK_EQUAL(table.at("key"), 10);
  table.emplace("key", 20);
  BOOST_CHECK_EQUAL(table.at("key"), 20);
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_emplace_with_move)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  std::string value = "hello";
  table.emplace(1, std::move(value));
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK_EQUAL(table.at(1), "hello");
}

BOOST_AUTO_TEST_CASE(test_emplace_default_constructed)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(42);
  BOOST_CHECK(table.contains(42));
  BOOST_CHECK_EQUAL(table.at(42), 0);
}

BOOST_AUTO_TEST_CASE(test_at_method)
{
  hvostov::RobinHoodHashTable< std::string, int > table;
  table.emplace("first", 1);
  table.emplace("second", 2);
  BOOST_CHECK_EQUAL(table.at("first"), 1);
  BOOST_CHECK_EQUAL(table.at("second"), 2);
  table.at("first") = 100;
  BOOST_CHECK_EQUAL(table.at("first"), 100);
}

BOOST_AUTO_TEST_CASE(test_at_throws_on_missing_key)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  BOOST_CHECK_THROW(table.at(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_const_at_throws_on_missing_key)
{
  const hvostov::RobinHoodHashTable< int, std::string > table;
  BOOST_CHECK_THROW(table.at(42), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets)
{
  hvostov::RobinHoodHashTable< std::string, std::string > table;
  table["hello"] = "world";
  BOOST_CHECK_EQUAL(table["hello"], "world");
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets_creates_default)
{
  hvostov::RobinHoodHashTable< int, int > table;
  BOOST_CHECK_EQUAL(table[5], 0);
  BOOST_CHECK(table.contains(5));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_const_operator_brackets)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  const auto& ctable = table;
  BOOST_CHECK_EQUAL(ctable[1], "one");
}

BOOST_AUTO_TEST_CASE(test_find_existing)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  table.emplace(2, "two");
  auto it = table.find(1);
  BOOST_CHECK(it != table.end());
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second, "one");
}

BOOST_AUTO_TEST_CASE(test_find_missing)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  auto it = table.find(42);
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(test_const_find)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  const auto& ctable = table;
  auto it = ctable.find(1);
  BOOST_CHECK(it != ctable.end());
  BOOST_CHECK_EQUAL(it->second, "one");
}

BOOST_AUTO_TEST_CASE(test_find_modify_value)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "old");
  auto it = table.find(1);
  it->second = "new";
  BOOST_CHECK_EQUAL(table.at(1), "new");
}

BOOST_AUTO_TEST_CASE(test_erase_existing_key)
{
  hvostov::RobinHoodHashTable< std::string, int > table;
  table.emplace("apple", 5);
  table.emplace("banana", 3);
  table.erase("apple");
  BOOST_CHECK(!table.contains("apple"));
  BOOST_CHECK(table.contains("banana"));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_erase_missing_key)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  table.erase(42);
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.contains(1));
}

BOOST_AUTO_TEST_CASE(test_erase_from_empty_table)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.erase(1);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_erase_all_elements)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  std::vector< int > keys = {1, 2, 3, 4, 5};
  for (int key : keys) {
    table.emplace(key, "val" + std::to_string(key));
  }
  for (int key : keys) {
    BOOST_CHECK(table.contains(key));
    table.erase(key);
  }
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_erase_and_reinsert)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "first");
  table.erase(1);
  table.emplace(1, "second");
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), "second");
}

BOOST_AUTO_TEST_CASE(test_erase_many_elements)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 100; ++i) {
    table.emplace(i, i * 10);
  }
  for (int i = 0; i < 50; ++i) {
    table.erase(i);
  }
  BOOST_CHECK_EQUAL(table.size(), 50);
  for (int i = 50; i < 100; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_robin_hood_ordering)
{
  hvostov::RobinHoodHashTable< int, int > table(8);
  for (int i = 0; i < 6; ++i) {
    table.emplace(i, i * 10);
  }
  BOOST_CHECK_EQUAL(table.size(), 6);
  for (int i = 0; i < 6; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_robin_hood_probe_sequence)
{
  hvostov::RobinHoodHashTable< int, int > table(8);
  for (int i = 0; i < 10; ++i) {
    table.emplace(i * 8, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 10);
  for (int i = 0; i < 10; ++i) {
    BOOST_CHECK(table.contains(i * 8));
  }
}

BOOST_AUTO_TEST_CASE(test_robin_hood_rich_poor_swap)
{
  hvostov::RobinHoodHashTable< int, std::string > table(4);
  table.emplace(0, "zero");
  table.emplace(4, "four");
  table.emplace(8, "eight");
  BOOST_CHECK(table.contains(0));
  BOOST_CHECK(table.contains(4));
  BOOST_CHECK(table.contains(8));
}

BOOST_AUTO_TEST_CASE(test_automatic_rehash)
{
  hvostov::RobinHoodHashTable< int, int > table(8);
  size_t initial_capacity = table.capacity();
  for (int i = 0; i < 100; ++i) {
    table.emplace(i, i * 10);
  }
  BOOST_CHECK(table.capacity() > initial_capacity);
  BOOST_CHECK_EQUAL(table.size(), 100);
  for (int i = 0; i < 100; ++i) {
    BOOST_CHECK(table.contains(i));
    BOOST_CHECK_EQUAL(table.at(i), i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_manual_rehash)
{
  hvostov::RobinHoodHashTable< int, int > table(8);
  for (int i = 0; i < 20; ++i) {
    table.emplace(i, i * 10);
  }
  table.rehash(50);
  BOOST_CHECK(table.capacity() >= 50);
  BOOST_CHECK_EQUAL(table.size(), 20);
  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(table.contains(i));
    BOOST_CHECK_EQUAL(table.at(i), i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_rehash_without_arguments)
{
  hvostov::RobinHoodHashTable< int, int > table(16);
  for (int i = 0; i < 12; ++i) {
    table.emplace(i, i);
  }
  size_t old_cap = table.capacity();
  table.rehash();
  BOOST_CHECK(table.capacity() > old_cap);
  BOOST_CHECK_EQUAL(table.size(), 12);
}

BOOST_AUTO_TEST_CASE(test_rehash_smaller_throws)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 20; ++i) {
    table.emplace(i, i);
  }
  BOOST_CHECK_THROW(table.rehash(10), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_reserve)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.reserve(100);
  BOOST_CHECK(table.capacity() >= 100 / 0.75);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_reserve_smaller_does_nothing)
{
  hvostov::RobinHoodHashTable< int, int > table(100);
  size_t old_cap = table.capacity();
  table.reserve(10);
  BOOST_CHECK_EQUAL(table.capacity(), old_cap);
}

BOOST_AUTO_TEST_CASE(test_rehash_preserves_robin_hood)
{
  hvostov::RobinHoodHashTable< int, int > table(8);
  for (int i = 0; i < 50; ++i) {
    table.emplace(i, i * 10);
  }
  table.rehash(100);
  BOOST_CHECK_EQUAL(table.size(), 50);
  for (int i = 0; i < 50; ++i) {
    BOOST_CHECK(table.contains(i));
    BOOST_CHECK_EQUAL(table.at(i), i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_iterator_begin_end)
{
  hvostov::RobinHoodHashTable< int, int > table;
  BOOST_CHECK(table.begin() == table.end());
  table.emplace(1, 10);
  BOOST_CHECK(table.begin() != table.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_traversal)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 10; ++i) {
    table.emplace(i, i * 10);
  }
  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    BOOST_CHECK(table.contains(it->first));
    BOOST_CHECK_EQUAL(it->second, it->first * 10);
    ++count;
  }
  BOOST_CHECK_EQUAL(count, table.size());
}

BOOST_AUTO_TEST_CASE(test_iterator_increment)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  table.emplace(2, 20);
  auto it = table.begin();
  auto first_key = it->first;
  ++it;
  BOOST_CHECK(it->first != first_key);
}

BOOST_AUTO_TEST_CASE(test_iterator_post_increment)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  table.emplace(2, 20);
  auto it = table.begin();
  auto old_it = it++;
  BOOST_CHECK_EQUAL(old_it->first, 1);
  BOOST_CHECK(it != table.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_equality)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  table.emplace(2, 20);
  auto it1 = table.find(1);
  auto it2 = table.find(1);
  auto it3 = table.find(2);
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
}

BOOST_AUTO_TEST_CASE(test_iterator_dereference)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  auto it = table.begin();
  BOOST_CHECK_EQUAL((*it).first, 1);
  BOOST_CHECK_EQUAL(it->second, "one");
}

BOOST_AUTO_TEST_CASE(test_iterator_modify_value)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  auto it = table.begin();
  it->second = 100;
  BOOST_CHECK_EQUAL(table.at(1), 100);
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 5; ++i) {
    table.emplace(i, i * 10);
  }
  const auto& ctable = table;
  size_t count = 0;
  for (auto it = ctable.begin(); it != ctable.end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, table.size());
}

BOOST_AUTO_TEST_CASE(test_const_iterator_cbegin_cend)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  const auto& ctable = table;
  BOOST_CHECK(ctable.cbegin() != ctable.cend());
}

BOOST_AUTO_TEST_CASE(test_iterator_empty_table)
{
  hvostov::RobinHoodHashTable< int, int > table;
  BOOST_CHECK(table.begin() == table.end());
  BOOST_CHECK(table.cbegin() == table.cend());
}

BOOST_AUTO_TEST_CASE(test_range_based_for_loop)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 10; ++i) {
    table.emplace(i, i * 10);
  }
  size_t count = 0;
  for (const auto& pair : table) {
    BOOST_CHECK(table.contains(pair.first));
    ++count;
  }
  BOOST_CHECK_EQUAL(count, table.size());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  hvostov::RobinHoodHashTable< std::string, int > table1;
  table1.emplace("one", 1);
  table1.emplace("two", 2);
  table1.emplace("three", 3);

  hvostov::RobinHoodHashTable< std::string, int > table2(table1);
  BOOST_CHECK_EQUAL(table2.size(), 3);
  BOOST_CHECK(table2.contains("one"));
  BOOST_CHECK(table2.contains("two"));
  BOOST_CHECK(table2.contains("three"));
  BOOST_CHECK_EQUAL(table2.at("one"), 1);
  BOOST_CHECK_EQUAL(table2.at("two"), 2);
  BOOST_CHECK_EQUAL(table2.at("three"), 3);

  table2.emplace("four", 4);
  BOOST_CHECK(!table1.contains("four"));
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  hvostov::RobinHoodHashTable< int, std::string > table1;
  table1.emplace(1, "one");
  table1.emplace(2, "two");
  size_t old_size = table1.size();
  size_t old_cap = table1.capacity();

  hvostov::RobinHoodHashTable< int, std::string > table2(std::move(table1));
  BOOST_CHECK_EQUAL(table2.size(), old_size);
  BOOST_CHECK_EQUAL(table2.capacity(), old_cap);
  BOOST_CHECK(table2.contains(1));
  BOOST_CHECK(table2.contains(2));
  BOOST_CHECK(table1.empty());
  BOOST_CHECK_EQUAL(table1.capacity(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  hvostov::RobinHoodHashTable< std::string, int > table1;
  table1.emplace("a", 1);
  table1.emplace("b", 2);
  hvostov::RobinHoodHashTable< std::string, int > table2;
  table2.emplace("c", 3);
  table2 = table1;
  BOOST_CHECK_EQUAL(table2.size(), 2);
  BOOST_CHECK(table2.contains("a"));
  BOOST_CHECK(table2.contains("b"));
  BOOST_CHECK(!table2.contains("c"));
  BOOST_CHECK_EQUAL(table2.at("a"), 1);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  hvostov::RobinHoodHashTable< int, std::string > table1;
  table1.emplace(1, "one");
  size_t old_size = table1.size();

  hvostov::RobinHoodHashTable< int, std::string > table2;
  table2.emplace(2, "two");
  table2 = std::move(table1);
  BOOST_CHECK_EQUAL(table2.size(), old_size);
  BOOST_CHECK(table2.contains(1));
  BOOST_CHECK(!table2.contains(2));
}

BOOST_AUTO_TEST_CASE(test_self_copy_assignment)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  table = table;
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), 10);
}

BOOST_AUTO_TEST_CASE(test_self_move_assignment)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);

  auto& ref = table;
  table = std::move(ref);

  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), 10);
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  hvostov::RobinHoodHashTable< std::string, int > table1;
  table1.emplace("first", 1);
  hvostov::RobinHoodHashTable< std::string, int > table2;
  table2.emplace("second", 2);
  table1.swap(table2);
  BOOST_CHECK(table1.contains("second"));
  BOOST_CHECK(table2.contains("first"));
  BOOST_CHECK(!table1.contains("first"));
  BOOST_CHECK(!table2.contains("second"));
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "one");
  table.emplace(2, "two");
  table.emplace(3, "three");
  BOOST_CHECK_EQUAL(table.size(), 3);
  table.clear();
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_EQUAL(table.capacity(), 0);
  BOOST_CHECK(!table.contains(1));
}

BOOST_AUTO_TEST_CASE(test_clear_empty_table)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.clear();
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_empty_after_clear)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 10; ++i) {
    table.emplace(i, i);
  }
  table.clear();
  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(test_string_keys)
{
  hvostov::RobinHoodHashTable< std::string, int > table;
  table.emplace("hello world", 1);
  table.emplace("foo bar", 2);
  table.emplace("test", 3);
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK_EQUAL(table.at("hello world"), 1);
}

BOOST_AUTO_TEST_CASE(test_double_keys)
{
  hvostov::RobinHoodHashTable< double, std::string > table;
  table.emplace(3.14, "pi");
  table.emplace(2.71, "e");
  BOOST_CHECK(table.contains(3.14));
  BOOST_CHECK(table.contains(2.71));
}

BOOST_AUTO_TEST_CASE(test_pointer_keys)
{
  int a = 1, b = 2, c = 3;
  hvostov::RobinHoodHashTable< int*, std::string > table;
  table.emplace(&a, "a");
  table.emplace(&b, "b");
  table.emplace(&c, "c");
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK(table.contains(&a));
}

BOOST_AUTO_TEST_CASE(test_large_number_of_elements)
{
  size_t N = 1000;
  hvostov::RobinHoodHashTable< int, int > table;
  for (size_t i = 0; i < N; ++i) {
    table.emplace(static_cast< int >(i), static_cast< int >(i * i));
  }
  BOOST_CHECK_EQUAL(table.size(), N);
  for (size_t i = 0; i < N; ++i) {
    BOOST_CHECK(table.contains(static_cast< int >(i)));
    BOOST_CHECK_EQUAL(table.at(static_cast< int >(i)), static_cast< int >(i * i));
  }
}

BOOST_AUTO_TEST_CASE(test_insert_and_erase_many)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 500; ++i) {
    table.emplace(i, i * 10);
  }
  for (int i = 0; i < 250; ++i) {
    table.erase(i);
  }
  BOOST_CHECK_EQUAL(table.size(), 250);
  for (int i = 250; i < 500; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_rehash_large_table)
{
  hvostov::RobinHoodHashTable< int, int > table(8);
  for (int i = 0; i < 500; ++i) {
    table.emplace(i, i);
  }
  BOOST_CHECK_EQUAL(table.size(), 500);
  for (int i = 0; i < 500; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_empty_table_operations)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_THROW(table.at(1), std::out_of_range);
  BOOST_CHECK(!table.contains(1));
  BOOST_CHECK(table.find(1) == table.end());
}

BOOST_AUTO_TEST_CASE(test_single_element)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(42, 100);
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.contains(42));
  BOOST_CHECK(!table.contains(0));
  table.erase(42);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_duplicate_keys)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  table.emplace(1, "first");
  table.emplace(1, "second");
  table.emplace(1, "third");
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), "third");
}

BOOST_AUTO_TEST_CASE(test_many_collisions)
{
  struct BadHash {
    size_t operator()(int key) const
    {
      return key % 4;
    }
  };

  hvostov::RobinHoodHashTable< int, int, BadHash > table(8);

  for (int i = 0; i < 20; ++i) {
    table.emplace(i, i);
  }

  BOOST_CHECK_EQUAL(table.size(), 20);
  for (int i = 0; i < 20; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_rehash_after_erase)
{
  hvostov::RobinHoodHashTable< int, int > table;
  for (int i = 0; i < 100; ++i) {
    table.emplace(i, i);
  }
  for (int i = 0; i < 90; ++i) {
    table.erase(i);
  }
  table.rehash();
  BOOST_CHECK_EQUAL(table.size(), 10);
  for (int i = 90; i < 100; ++i) {
    BOOST_CHECK(table.contains(i));
  }
}

BOOST_AUTO_TEST_CASE(test_initializer_list)
{
  hvostov::RobinHoodHashTable< int, std::string > table = {{1, "one"}, {2, "two"}, {3, "three"}};
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK_EQUAL(table.at(1), "one");
  BOOST_CHECK_EQUAL(table.at(2), "two");
  BOOST_CHECK_EQUAL(table.at(3), "three");
}

BOOST_AUTO_TEST_CASE(test_empty_initializer_list)
{
  hvostov::RobinHoodHashTable< int, std::string > table = {};
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(test_vector_value)
{
  hvostov::RobinHoodHashTable< int, std::vector< int > > table;
  table.emplace(1, std::vector< int >{1, 2, 3});
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK_EQUAL(table.at(1).size(), 3);
  table.at(1).push_back(4);
  BOOST_CHECK_EQUAL(table.at(1).size(), 4);
}

BOOST_AUTO_TEST_CASE(test_string_value_with_move)
{
  hvostov::RobinHoodHashTable< int, std::string > table;
  std::string long_string(1000, 'x');
  table.emplace(1, std::move(long_string));
  BOOST_CHECK_EQUAL(table.at(1).size(), 1000);
}

BOOST_AUTO_TEST_CASE(test_unique_ptr_value)
{
  hvostov::RobinHoodHashTable< int, std::unique_ptr< int > > table;
  table.emplace(1, std::make_unique< int >(42));
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK_EQUAL(*table.at(1), 42);
}

BOOST_AUTO_TEST_CASE(test_const_table_operations)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  const auto& ctable = table;
  BOOST_CHECK(ctable.contains(1));
  BOOST_CHECK(!ctable.contains(2));
  BOOST_CHECK_EQUAL(ctable.at(1), 10);
  BOOST_CHECK_EQUAL(ctable.size(), 1);
  BOOST_CHECK_EQUAL(ctable.capacity(), table.capacity());
  BOOST_CHECK(!ctable.empty());
}

BOOST_AUTO_TEST_CASE(test_const_iterator_dereference)
{
  hvostov::RobinHoodHashTable< int, int > table;
  table.emplace(1, 10);
  const auto& ctable = table;
  auto it = ctable.find(1);
  BOOST_CHECK_EQUAL(it->second, 10);
}

BOOST_AUTO_TEST_SUITE_END()
