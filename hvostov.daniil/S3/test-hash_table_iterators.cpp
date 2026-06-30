#include <boost/test/unit_test.hpp>
#include <string>
#include "hash_table.hpp"

BOOST_AUTO_TEST_SUITE(hash_table_iterators_suite)

BOOST_AUTO_TEST_CASE(test_iterator_basic)
{
  hvostov::HashTable< std::string, int > table(3);
  table.add("one", 1);
  table.add("two", 2);
  table.add("three", 3);

  int count = 0;
  int sum = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    sum += (*it).second;
    count++;
  }

  BOOST_CHECK_EQUAL(count, 3);
  BOOST_CHECK_EQUAL(sum, 6);
}

BOOST_AUTO_TEST_CASE(test_iterator_empty_table)
{
  hvostov::HashTable< std::string, int > table;

  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    count++;
  }

  BOOST_CHECK_EQUAL(count, 0);
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_single_element)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("only", 42);

  auto it = table.begin();
  BOOST_CHECK(it != table.end());
  BOOST_CHECK_EQUAL((*it).first, "only");
  BOOST_CHECK_EQUAL((*it).second, 42);

  ++it;
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_arrow_operator)
{
  hvostov::HashTable< std::string, std::string > table(1);
  table.add("key", "value");

  auto it = table.begin();
  BOOST_CHECK_EQUAL(it->first, "key");
  BOOST_CHECK_EQUAL(it->second, "value");
}

BOOST_AUTO_TEST_CASE(test_iterator_modify_value)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("modify", 10);

  auto it = table.begin();
  (*it).second = 20;

  BOOST_CHECK_EQUAL(table.at("modify"), 20);
}

BOOST_AUTO_TEST_CASE(test_iterator_postfix_increment)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("first", 1);
  table.add("second", 2);

  auto it = table.begin();
  auto old = it++;

  BOOST_CHECK_EQUAL((*old).second, 1);
  BOOST_CHECK(it != table.begin());
}

BOOST_AUTO_TEST_CASE(test_iterator_copy_constructor)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("copy", 100);

  auto it1 = table.begin();
  auto it2(it1);

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK_EQUAL((*it2).second, 100);
}

BOOST_AUTO_TEST_CASE(test_iterator_assignment)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("assign", 50);

  auto it1 = table.begin();
  decltype(it1) it2;
  it2 = it1;

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK_EQUAL((*it2).second, 50);
}

BOOST_AUTO_TEST_CASE(test_iterator_move_constructor)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("move", 75);

  auto it1 = table.begin();
  auto it2(std::move(it1));

  BOOST_CHECK_EQUAL((*it2).second, 75);
}

BOOST_AUTO_TEST_CASE(test_iterator_inequality)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("a", 1);
  table.add("b", 2);

  auto it1 = table.begin();
  auto it2 = table.begin();
  ++it2;

  BOOST_CHECK(it1 != it2);
}

BOOST_AUTO_TEST_CASE(test_const_iterator_basic)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("const1", 10);
  table.add("const2", 20);

  const auto& ctable = table;
  int count = 0;
  int sum = 0;

  for (auto it = ctable.begin(); it != ctable.end(); ++it) {
    sum += (*it).second;
    count++;
  }

  BOOST_CHECK_EQUAL(count, 2);
  BOOST_CHECK_EQUAL(sum, 30);
}

BOOST_AUTO_TEST_CASE(test_const_iterator_from_iterator)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("convert", 99);

  hvostov::HashTable< std::string, int >::Iterator it = table.begin();
  hvostov::HashTable< std::string, int >::ConstIterator cit(it);

  BOOST_CHECK_EQUAL((*cit).second, 99);
}

BOOST_AUTO_TEST_CASE(test_cbegin_cend)
{
  hvostov::HashTable< std::string, int > table(2);
  table.add("cbegin", 1);
  table.add("cend", 2);

  int count = 0;
  for (auto it = table.cbegin(); it != table.cend(); ++it) {
    count++;
  }

  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_range_based_for)
{
  hvostov::HashTable< std::string, int > table(3);
  table.add("range1", 10);
  table.add("range2", 20);
  table.add("range3", 30);

  int sum = 0;
  for (const auto& pair : table) {
    sum += pair.second;
  }

  BOOST_CHECK_EQUAL(sum, 60);
}

BOOST_AUTO_TEST_CASE(test_iterator_with_overflow)
{
  hvostov::HashTable< int, int > table(8);

  for (int i = 0; i < 8; ++i) {
    table.add(i, i * 10);
  }

  int count = 0;
  int sum = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    sum += (*it).second;
    count++;
  }

  BOOST_CHECK_EQUAL(count, 8);
  BOOST_CHECK_EQUAL(sum, 280);
}

BOOST_AUTO_TEST_CASE(test_iterator_after_clear)
{
  hvostov::HashTable< std::string, int > table(1);
  table.add("before", 1);
  table.clear();

  BOOST_CHECK(table.begin() == table.end());

  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    count++;
  }
  BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_CASE(test_iterator_after_rehash)
{
  hvostov::HashTable< int, int > table(2);

  for (int i = 0; i < 5; ++i) {
    table.add(i, i);
  }

  table.rehash(10);

  int count = 0;
  for (auto it = table.begin(); it != table.end(); ++it) {
    count++;
  }

  BOOST_CHECK_EQUAL(count, 5);
}

BOOST_AUTO_TEST_SUITE_END()
