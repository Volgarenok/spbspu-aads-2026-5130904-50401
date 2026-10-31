#include "AutoHashTable.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace khasnulin;

namespace
{
  struct AutoPerson
  {
    size_t age;
    std::string name;
    AutoPerson():
        age(0),
        name("unknown")
    {
    }
    AutoPerson(size_t a, const std::string &n):
        age(a),
        name(n)
    {
    }

    bool operator==(const AutoPerson &rhs) const
    {
      return age == rhs.age && name == rhs.name;
    }
    bool operator!=(const AutoPerson &rhs) const
    {
      return !(*this == rhs);
    }
  };

  struct ConstantHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  constexpr int SINGLE_BUCKET_MAX = 14;
}

BOOST_AUTO_TEST_SUITE(auto_hash_table_test_suite)

BOOST_AUTO_TEST_CASE(test_empty_creation)
{
  AutoHashTable< int, std::string > ht;

  BOOST_REQUIRE(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK_EQUAL(ht.bucketSize(), 16);

  BOOST_CHECK(ht.has(42) == false);
  BOOST_CHECK(ht.find(42) == nullptr);
}

BOOST_AUTO_TEST_CASE(test_custom_initial_buckets)
{
  AutoHashTable< int, std::string > ht(4);

  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.bucketSize(), 4);
}

BOOST_AUTO_TEST_CASE(test_constructor_zero_buckets_negative)
{
  BOOST_CHECK_THROW((AutoHashTable< int, std::string >(0)), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_add_and_has)
{
  AutoHashTable< int, std::string > ht;

  ht.add(1, "one");
  ht.add(2, "two");

  BOOST_REQUIRE(ht.empty() == false);
  BOOST_CHECK_EQUAL(ht.size(), 2);

  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(ht.has(3) == false);
}

BOOST_AUTO_TEST_CASE(test_update_existing_key)
{
  AutoHashTable< int, std::string > ht;

  ht.add(1, "initial");
  ht.add(1, "updated");

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_REQUIRE(ht.find(1) != nullptr);
  BOOST_CHECK_EQUAL(*ht.find(1), "updated");
}

BOOST_AUTO_TEST_CASE(test_add_lvalue_copies_value)
{
  AutoHashTable< int, AutoPerson > ht;
  AutoPerson person{25, "Oleg"};

  ht.add(1, person);

  BOOST_CHECK(person == AutoPerson(25, "Oleg"));
  BOOST_REQUIRE(ht.find(1) != nullptr);
  BOOST_CHECK(*ht.find(1) == AutoPerson(25, "Oleg"));
}

BOOST_AUTO_TEST_CASE(test_add_rvalue_perfect_forwarding)
{
  AutoHashTable< int, AutoPerson > ht;
  AutoPerson person{25, "Oleg"};

  ht.add(1, std::move(person));

  BOOST_REQUIRE(ht.find(1) != nullptr);
  BOOST_CHECK(*ht.find(1) == AutoPerson(25, "Oleg"));
  BOOST_CHECK(person.name.empty() == true);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets)
{
  AutoHashTable< std::string, int > ht;

  ht["apples"] = 10;
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has("apples"));
  BOOST_CHECK_EQUAL(ht["apples"], 10);

  ht["apples"] += 5;
  BOOST_CHECK_EQUAL(ht["apples"], 15);
  BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets_default_value)
{
  AutoHashTable< std::string, int > ht;

  BOOST_CHECK_EQUAL(ht["missing"], 0);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has("missing"));
}

BOOST_AUTO_TEST_CASE(test_find_existing_key_positive)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");

  std::string *found = ht.find(1);

  BOOST_REQUIRE(found != nullptr);
  BOOST_CHECK_EQUAL(*found, "one");
}

BOOST_AUTO_TEST_CASE(test_find_missing_key_does_not_insert_negative)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");

  BOOST_CHECK(ht.find(42) == nullptr);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has(42) == false);
}

BOOST_AUTO_TEST_CASE(test_find_modify_through_pointer)
{
  AutoHashTable< std::string, int > ht;
  ht.add("apples", 10);

  int *found = ht.find("apples");
  BOOST_REQUIRE(found != nullptr);
  *found = 42;

  BOOST_CHECK_EQUAL(ht["apples"], 42);
}

BOOST_AUTO_TEST_CASE(test_find_const_table)
{
  AutoHashTable< int, int > ht;
  ht.add(1, 100);

  const AutoHashTable< int, int > &const_ht = ht;

  const int *found = const_ht.find(1);
  BOOST_REQUIRE(found != nullptr);
  BOOST_CHECK_EQUAL(*found, 100);
  BOOST_CHECK(const_ht.find(2) == nullptr);
  BOOST_CHECK(const_ht.has(1));
}

BOOST_AUTO_TEST_CASE(test_erase_existing_key_positive)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");
  ht.add(2, "two");

  BOOST_CHECK(ht.erase(1) == true);

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has(1) == false);
  BOOST_CHECK(ht.find(1) == nullptr);
  BOOST_CHECK(ht.has(2));
}

BOOST_AUTO_TEST_CASE(test_erase_missing_key_negative)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");

  BOOST_CHECK_NO_THROW(ht.erase(42));
  BOOST_CHECK(ht.erase(42) == false);
  BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_erase_in_empty_table_negative)
{
  AutoHashTable< int, std::string > ht;

  BOOST_CHECK(ht.erase(1) == false);
  BOOST_CHECK(ht.empty());
}

BOOST_AUTO_TEST_CASE(test_erase_twice)
{
  AutoHashTable< int, int > ht;
  ht.add(1, 10);

  BOOST_CHECK(ht.erase(1) == true);
  BOOST_CHECK(ht.erase(1) == false);
  BOOST_CHECK(ht.empty());
}

BOOST_AUTO_TEST_CASE(test_erase_from_overflow)
{
  AutoHashTable< int, int, ConstantHash > ht;
  for (int i = 1; i <= 6; ++i)
  {
    ht.add(i, i * 10);
  }

  BOOST_CHECK(ht.erase(6) == true);
  BOOST_CHECK(ht.erase(2) == true);

  BOOST_CHECK_EQUAL(ht.size(), 4);
  BOOST_CHECK(ht.has(6) == false);
  BOOST_CHECK(ht.has(2) == false);
  BOOST_REQUIRE(ht.find(5) != nullptr);
  BOOST_CHECK_EQUAL(*ht.find(5), 50);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");
  ht.add(2, "two");

  ht.clear();

  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK(ht.has(1) == false);
  BOOST_CHECK(ht.begin() == ht.end());

  ht.add(3, "three");
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has(3));
}

BOOST_AUTO_TEST_CASE(test_clear_after_growth)
{
  AutoHashTable< int, int > ht(1);
  for (int i = 0; i < 100; ++i)
  {
    ht.add(i, i);
  }

  ht.clear();

  BOOST_CHECK(ht.empty());
  for (int i = 0; i < 100; ++i)
  {
    BOOST_CHECK(ht.has(i) == false);
  }
}

BOOST_AUTO_TEST_CASE(test_add_triggers_growth)
{
  AutoHashTable< int, int > ht(1);
  for (int i = 0; i < SINGLE_BUCKET_MAX; ++i)
  {
    ht.add(i, i * 10);
  }

  BOOST_REQUIRE_NO_THROW(ht.add(SINGLE_BUCKET_MAX, 999));

  BOOST_CHECK(ht.bucketSize() > 1);
  BOOST_CHECK_EQUAL(ht.size(), SINGLE_BUCKET_MAX + 1);
  BOOST_REQUIRE(ht.find(SINGLE_BUCKET_MAX) != nullptr);
  BOOST_CHECK_EQUAL(*ht.find(SINGLE_BUCKET_MAX), 999);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets_triggers_growth)
{
  AutoHashTable< int, int > ht(1);
  for (int i = 0; i < SINGLE_BUCKET_MAX; ++i)
  {
    ht[i] = i * 10;
  }

  BOOST_REQUIRE_NO_THROW(ht[SINGLE_BUCKET_MAX] = 999);

  BOOST_CHECK(ht.bucketSize() > 1);
  BOOST_CHECK_EQUAL(ht.size(), SINGLE_BUCKET_MAX + 1);
  BOOST_CHECK_EQUAL(ht[SINGLE_BUCKET_MAX], 999);
}

BOOST_AUTO_TEST_CASE(test_growth_keeps_all_values)
{
  AutoHashTable< int, int > ht(1);
  const int count = 1000;
  for (int i = 0; i < count; ++i)
  {
    ht.add(i, i * 10);
  }

  BOOST_CHECK_EQUAL(ht.size(), count);
  for (int i = 0; i < count; ++i)
  {
    const int *found = ht.find(i);
    BOOST_REQUIRE(found != nullptr);
    BOOST_CHECK_EQUAL(*found, i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_growth_with_string_keys)
{
  AutoHashTable< std::string, int > ht;
  const int count = 500;
  for (int i = 0; i < count; ++i)
  {
    ht.add("user_" + std::to_string(i), i);
  }

  BOOST_CHECK_EQUAL(ht.size(), count);
  for (int i = 0; i < count; ++i)
  {
    const int *found = ht.find("user_" + std::to_string(i));
    BOOST_REQUIRE(found != nullptr);
    BOOST_CHECK_EQUAL(*found, i);
  }
}

// Ретрай после роста не должен потерять перемещаемое значение:
// HashTable::add при overflow_error не должен потреблять value.
BOOST_AUTO_TEST_CASE(test_growth_rvalue_not_lost_on_retry)
{
  AutoHashTable< int, AutoPerson > ht(1);
  for (int i = 0; i < SINGLE_BUCKET_MAX; ++i)
  {
    ht.add(i, AutoPerson(i, "filler"));
  }

  AutoPerson person{25, "Oleg"};
  BOOST_REQUIRE_NO_THROW(ht.add(SINGLE_BUCKET_MAX, std::move(person)));

  BOOST_CHECK(ht.bucketSize() > 1);
  BOOST_REQUIRE(ht.find(SINGLE_BUCKET_MAX) != nullptr);
  BOOST_CHECK(*ht.find(SINGLE_BUCKET_MAX) == AutoPerson(25, "Oleg"));
}

BOOST_AUTO_TEST_CASE(test_growth_impossible_add_strong_exception_safety)
{
  AutoHashTable< int, int, ConstantHash > ht;
  for (int i = 0; i < SINGLE_BUCKET_MAX; ++i)
  {
    ht.add(i, i * 10);
  }
  const size_t buckets_before = ht.bucketSize();

  BOOST_CHECK_THROW(ht.add(SINGLE_BUCKET_MAX, 999), std::overflow_error);

  BOOST_CHECK_EQUAL(ht.size(), SINGLE_BUCKET_MAX);
  BOOST_CHECK(ht.bucketSize() >= buckets_before);
  BOOST_CHECK(ht.has(SINGLE_BUCKET_MAX) == false);
  for (int i = 0; i < SINGLE_BUCKET_MAX; ++i)
  {
    const int *found = ht.find(i);
    BOOST_REQUIRE(found != nullptr);
    BOOST_CHECK_EQUAL(*found, i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_growth_impossible_operator_brackets_strong_exception_safety)
{
  AutoHashTable< int, std::string, ConstantHash > ht;
  for (int i = 0; i < SINGLE_BUCKET_MAX; ++i)
  {
    ht[i] = "value";
  }

  BOOST_CHECK_THROW(ht[SINGLE_BUCKET_MAX] = "E", std::overflow_error);

  BOOST_CHECK_EQUAL(ht.size(), SINGLE_BUCKET_MAX);
  BOOST_CHECK(ht.has(SINGLE_BUCKET_MAX) == false);
}

BOOST_AUTO_TEST_CASE(test_growth_impossible_update_existing_key_positive)
{
  AutoHashTable< int, int, ConstantHash > ht;
  for (int i = 0; i < SINGLE_BUCKET_MAX; ++i)
  {
    ht.add(i, i);
  }

  // обновление существующего ключа не требует места и не должно бросать
  BOOST_REQUIRE_NO_THROW(ht.add(SINGLE_BUCKET_MAX - 1, 777));
  BOOST_REQUIRE_NO_THROW(ht[0] = 555);

  BOOST_CHECK_EQUAL(*ht.find(SINGLE_BUCKET_MAX - 1), 777);
  BOOST_CHECK_EQUAL(*ht.find(0), 555);
  BOOST_CHECK_EQUAL(ht.size(), SINGLE_BUCKET_MAX);
}

BOOST_AUTO_TEST_CASE(test_iterators_after_growth)
{
  AutoHashTable< int, int > ht(1);
  const int count = 200;
  long long expected_sum = 0;
  for (int i = 0; i < count; ++i)
  {
    ht.add(i, i);
    expected_sum += i;
  }

  int visited = 0;
  long long sum = 0;
  for (auto it = ht.begin(); it != ht.end(); ++it)
  {
    ++visited;
    sum += it->second;
  }

  BOOST_CHECK_EQUAL(visited, count);
  BOOST_CHECK_EQUAL(sum, expected_sum);
}

BOOST_AUTO_TEST_CASE(test_iterator_modify_value)
{
  AutoHashTable< int, int > ht;
  ht.add(1, 10);
  ht.add(2, 20);

  for (auto it = ht.begin(); it != ht.end(); ++it)
  {
    it->second *= 2;
  }

  BOOST_CHECK_EQUAL(*ht.find(1), 20);
  BOOST_CHECK_EQUAL(*ht.find(2), 40);
}

BOOST_AUTO_TEST_CASE(test_const_iterators_and_range_for)
{
  AutoHashTable< int, int > ht;
  ht.add(1, 100);
  ht.add(2, 200);
  ht.add(3, 300);

  const AutoHashTable< int, int > &const_ht = ht;

  int sum = 0;
  for (const auto &item : const_ht)
  {
    sum += item.second;
  }
  BOOST_CHECK_EQUAL(sum, 600);

  int count = 0;
  for (auto it = const_ht.cbegin(); it != const_ht.cend(); ++it)
  {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(test_empty_table_iterators)
{
  AutoHashTable< int, int > ht;
  const AutoHashTable< int, int > &const_ht = ht;

  BOOST_CHECK(ht.begin() == ht.end());
  BOOST_CHECK(const_ht.cbegin() == const_ht.cend());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor_independent)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");
  ht.add(2, "two");

  AutoHashTable< int, std::string > copy(ht);
  ht.add(1, "changed");
  ht.add(3, "three");

  BOOST_CHECK_EQUAL(copy.size(), 2);
  BOOST_CHECK_EQUAL(*copy.find(1), "one");
  BOOST_CHECK(copy.has(3) == false);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment_independent)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");

  AutoHashTable< int, std::string > other;
  other.add(42, "answer");

  other = ht;
  ht.erase(1);

  BOOST_CHECK_EQUAL(other.size(), 1);
  BOOST_CHECK(other.has(42) == false);
  BOOST_REQUIRE(other.find(1) != nullptr);
  BOOST_CHECK_EQUAL(*other.find(1), "one");
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");
  ht.add(2, "two");

  AutoHashTable< int, std::string > moved(std::move(ht));

  BOOST_CHECK_EQUAL(moved.size(), 2);
  BOOST_CHECK_EQUAL(*moved.find(1), "one");
  BOOST_CHECK_EQUAL(*moved.find(2), "two");
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  AutoHashTable< int, std::string > ht;
  ht.add(1, "one");

  AutoHashTable< int, std::string > other;
  other.add(42, "answer");

  other = std::move(ht);

  BOOST_CHECK_EQUAL(other.size(), 1);
  BOOST_CHECK(other.has(1));
  BOOST_CHECK(other.has(42) == false);
}

BOOST_AUTO_TEST_CASE(test_move_operations_noexcept)
{
  using Table = AutoHashTable< int, std::string >;
  BOOST_CHECK(std::is_nothrow_move_constructible< Table >::value);
  BOOST_CHECK(std::is_nothrow_move_assignable< Table >::value);
}

BOOST_AUTO_TEST_CASE(test_stress_against_unordered_map)
{
  AutoHashTable< int, int > ht(1);
  std::unordered_map< int, int > reference;

  std::mt19937 gen(42);
  std::uniform_int_distribution< int > op_dist(0, 3);
  std::uniform_int_distribution< int > key_dist(0, 300);
  std::uniform_int_distribution< int > value_dist(-1000, 1000);

  for (int step = 0; step < 20000; ++step)
  {
    int key = key_dist(gen);
    switch (op_dist(gen))
    {
    case 0:
    {
      int value = value_dist(gen);
      ht.add(key, value);
      reference[key] = value;
      break;
    }
    case 1:
    {
      bool expected = reference.erase(key) > 0;
      BOOST_REQUIRE_EQUAL(ht.erase(key), expected);
      break;
    }
    case 2:
    {
      const int *found = ht.find(key);
      auto it = reference.find(key);
      BOOST_REQUIRE_EQUAL(found != nullptr, it != reference.end());
      if (found)
      {
        BOOST_REQUIRE_EQUAL(*found, it->second);
      }
      break;
    }
    default:
    {
      int value = value_dist(gen);
      ht[key] = value;
      reference[key] = value;
      break;
    }
    }
    BOOST_REQUIRE_EQUAL(ht.size(), reference.size());
  }

  for (const auto &item : reference)
  {
    const int *found = ht.find(item.first);
    BOOST_REQUIRE(found != nullptr);
    BOOST_CHECK_EQUAL(*found, item.second);
  }
}

BOOST_AUTO_TEST_SUITE_END()
