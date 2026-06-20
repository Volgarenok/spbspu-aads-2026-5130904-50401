#include "hash_table.hpp"
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <string>

BOOST_AUTO_TEST_SUITE(hash_table_suite)

BOOST_AUTO_TEST_CASE(test_create_hash_table)
{
  hvostov::HashTable< int, std::string > table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_and_has)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("apple", 5);
  BOOST_CHECK(!table.empty());
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(table.has("apple"));
  BOOST_CHECK(!table.has("banana"));
}

BOOST_AUTO_TEST_CASE(test_add_multiple_elements)
{
  hvostov::HashTable< int, std::string > table(3);
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK(table.has(1));
  BOOST_CHECK(table.has(2));
  BOOST_CHECK(table.has(3));
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
  BOOST_CHECK(table.has(5));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_drop_existing_key)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("apple", 5);
  table.add("banana", 3);
  int value = table.drop("apple");
  BOOST_CHECK_EQUAL(value, 5);
  BOOST_CHECK(!table.has("apple"));
  BOOST_CHECK(table.has("banana"));
  BOOST_CHECK_EQUAL(table.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_drop_throws_on_missing_key)
{
  hvostov::HashTable< int, std::string > table(0);
  BOOST_CHECK_THROW(table.drop(42), std::out_of_range);
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
  BOOST_CHECK(!table.has(1));
}

BOOST_AUTO_TEST_CASE(test_overflow_throws)
{
  hvostov::HashTable< int, int > table(1);
  size_t cap = table.getCapacity();
  for (size_t i = 0; i < cap; ++i) {
    table.add(i, i * 10);
  }
  BOOST_CHECK_THROW(table.add(cap, cap * 10), std::overflow_error);
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
    BOOST_CHECK(table.has(i));
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
  BOOST_CHECK(table2.has("one"));
  BOOST_CHECK(table2.has("two"));
  BOOST_CHECK(table2.has("three"));
  BOOST_CHECK_EQUAL(table2.at("one"), 1);
  BOOST_CHECK_EQUAL(table2.at("two"), 2);
  BOOST_CHECK_EQUAL(table2.at("three"), 3);

  table2.add("four", 4);
  BOOST_CHECK(!table1.has("four"));
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  hvostov::HashTable< int, std::string > table1(10);
  table1.add(1, "one");
  table1.add(2, "two");
  hvostov::HashTable< int, std::string > table2(std::move(table1));
  BOOST_CHECK_EQUAL(table2.size(), 2);
  BOOST_CHECK(table2.has(1));
  BOOST_CHECK(table2.has(2));
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
  BOOST_CHECK(table2.has("a"));
  BOOST_CHECK(table2.has("b"));
  BOOST_CHECK(!table2.has("c"));
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
  BOOST_CHECK(table2.has(1));
  BOOST_CHECK(!table2.has(2));
  BOOST_CHECK(table1.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  hvostov::HashTable< std::string, int > table1(10);
  table1.add("first", 1);
  hvostov::HashTable< std::string, int > table2(10);
  table2.add("second", 2);
  table1.swap(table2);
  BOOST_CHECK(table1.has("second"));
  BOOST_CHECK(table2.has("first"));
  BOOST_CHECK(!table1.has("first"));
  BOOST_CHECK(!table2.has("second"));
}

BOOST_AUTO_TEST_CASE(test_empty_table_operations)
{
  hvostov::HashTable< int, std::string > table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_THROW(table.at(1), std::out_of_range);
  BOOST_CHECK_THROW(table.drop(1), std::out_of_range);
  BOOST_CHECK(!table.has(1));
}

BOOST_AUTO_TEST_CASE(test_different_key_types)
{
  hvostov::HashTable< double, std::string > table(10);
  table.add(3.14, "pi");
  table.add(2.71, "e");
  BOOST_CHECK(table.has(3.14));
  BOOST_CHECK(table.has(2.71));
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

BOOST_AUTO_TEST_CASE(test_drop_all_elements)
{
  hvostov::HashTable< int, std::string > table(10);
  std::vector< int > keys = {1, 2, 3, 4, 5};
  for (int key : keys) {
    table.add(key, "val" + std::to_string(key));
  }
  for (int key : keys) {
    BOOST_CHECK(table.has(key));
    table.drop(key);
  }
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_SUITE_END()
