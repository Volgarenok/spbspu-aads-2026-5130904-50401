#include "HashTable.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <string>

using namespace khasnulin;

struct Person
{
  size_t age;
  std::string name;
  Person():
      age(0),
      name("unknown")
  {
  }
  Person(size_t a):
      age(a),
      name("unknown")
  {
  }
  Person(const std::string &n):
      age(0),
      name(n)
  {
  }
  Person(size_t a, const std::string &n):
      age(a),
      name(n)
  {
  }

  bool operator==(const Person &rhs) const
  {
    return age == rhs.age && name == rhs.name;
  }
  bool operator!=(const Person &rhs) const
  {
    return !(*this == rhs);
  }
};

std::ostream &operator<<(std::ostream &out, const Person &rhs)
{
  return out << rhs.age << ", " << rhs.name << "; ";
}

BOOST_AUTO_TEST_SUITE(hash_table_test_suite)

BOOST_AUTO_TEST_CASE(test_empty_creation)
{
  HashTable< int, std::string > ht;

  BOOST_REQUIRE(ht.empty());
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK_EQUAL(ht.overflow_size(), 0);

  BOOST_CHECK(ht.has(42) == false);
}

BOOST_AUTO_TEST_CASE(test_add_and_has)
{
  HashTable< int, std::string > ht;

  ht.add(1, "one");
  ht.add(2, "two");

  BOOST_REQUIRE(ht.empty() == false);
  BOOST_CHECK_EQUAL(ht.size(), 2);

  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(ht.has(3) == false);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets)
{
  HashTable< std::string, int > ht;

  ht["apples"] = 10;
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has("apples"));
  BOOST_CHECK_EQUAL(ht["apples"], 10);

  ht["apples"] += 5;
  BOOST_CHECK_EQUAL(ht["apples"], 15);
}

BOOST_AUTO_TEST_CASE(test_add_rvalue_perfect_forwarding)
{
  HashTable< int, Person > ht;
  Person test_person{25, "Oleg"};

  ht.add(1, std::move(test_person));

  BOOST_REQUIRE(ht.empty() == false);
  BOOST_CHECK(ht[1] == Person(25, "Oleg"));

  BOOST_CHECK(test_person.name.empty() == true);
}

BOOST_AUTO_TEST_CASE(test_update_existing_key)
{
  HashTable< int, std::string > ht;

  ht.add(1, "initial");
  BOOST_CHECK_EQUAL(ht[1], "initial");

  ht.add(1, "updated");

  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht[1], "updated");
}

BOOST_AUTO_TEST_CASE(test_drop)
{
  HashTable< int, std::string > ht;

  ht.add(1, "data");

  BOOST_CHECK_EQUAL(ht.drop(1), 1);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK(ht.has(1) == false);
}

BOOST_AUTO_TEST_CASE(test_drop_missing_key_negative)
{
  HashTable< int, std::string > ht;
  ht.add(1, "data");

  BOOST_REQUIRE_NO_THROW(ht.drop(99));
  BOOST_CHECK_EQUAL(ht.drop(99), 0);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has(1));
}

BOOST_AUTO_TEST_CASE(test_drop_in_empty_table_negative)
{
  HashTable< int, std::string > ht;

  BOOST_CHECK_EQUAL(ht.drop(1), 0);
  BOOST_CHECK(ht.empty());
}

BOOST_AUTO_TEST_CASE(test_drop_twice_same_key)
{
  HashTable< int, std::string > ht;
  ht.add(1, "data");

  BOOST_CHECK_EQUAL(ht.drop(1), 1);
  BOOST_CHECK_EQUAL(ht.drop(1), 0);
  BOOST_CHECK(ht.empty());
}

BOOST_AUTO_TEST_CASE(test_overflow_trigger_and_rehash)
{
  HashTable< int, int > ht(1);
  ht.set_overflow_limit(2);

  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);
  ht.add(4, 40);

  ht.add(5, 50);
  ht.add(6, 60);

  BOOST_CHECK_THROW(ht.add(7, 70), std::overflow_error);

  BOOST_CHECK_EQUAL(ht.size(), 6);

  ht.rehash(10);

  BOOST_REQUIRE_NO_THROW(ht.add(7, 70));
  BOOST_CHECK_EQUAL(ht.size(), 7);
  BOOST_CHECK_EQUAL(ht[7], 70);
}

BOOST_AUTO_TEST_CASE(test_constructor_zero_buckets)
{
  BOOST_CHECK_THROW((HashTable< int, std::string >(0)), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_metrics_calculation)
{
  struct DummyHash
  {
    size_t operator()(int p) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(2);
  ht.set_overflow_limit(5);

  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);
  ht.add(4, 40);
  ht.add(5, 50);

  BOOST_CHECK_EQUAL(ht.size(), 5);
  BOOST_CHECK_EQUAL(ht.overflow_size(), 1);

  BOOST_CHECK_CLOSE(ht.average_bucket_size(), 2.0, 1e-5);
}

BOOST_AUTO_TEST_CASE(test_rehash_strong_exception_safety)
{
  HashTable< int, int > ht(5);
  ht.set_overflow_limit(2);
  for (int i = 0; i < 15; ++i)
  {
    ht.add(i, i * 10);
  }

  BOOST_CHECK_THROW(ht.rehash(1), std::overflow_error);

  BOOST_CHECK_EQUAL(ht.size(), 15);
  BOOST_CHECK(ht.has(0));
  BOOST_CHECK(ht.has(14));
}

BOOST_AUTO_TEST_CASE(test_drop_from_bucket_middle)
{
  struct DummyHash
  {
    size_t operator()(int p) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(0);
  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);
  ht.add(4, 40);

  BOOST_CHECK_EQUAL(ht.drop(2), 1);
  BOOST_CHECK_EQUAL(ht.size(), 3);
  BOOST_CHECK(ht.has(2) == false);

  BOOST_CHECK_EQUAL(ht[1], 10);
  BOOST_CHECK_EQUAL(ht[3], 30);
  BOOST_CHECK_EQUAL(ht[4], 40);

  // место в бакете освободилось: при нулевом overflow вставка проходит
  BOOST_REQUIRE_NO_THROW(ht.add(5, 50));
  BOOST_CHECK_EQUAL(ht.size(), 4);
  BOOST_CHECK_EQUAL(ht.overflow_size(), 0);
}

BOOST_AUTO_TEST_CASE(test_drop_last_in_bucket)
{
  struct DummyHash
  {
    size_t operator()(int p) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(0);
  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);

  BOOST_CHECK_EQUAL(ht.drop(3), 1);
  BOOST_CHECK_EQUAL(ht.size(), 2);
  BOOST_CHECK_EQUAL(ht[1], 10);
  BOOST_CHECK_EQUAL(ht[2], 20);
  BOOST_CHECK(ht.has(3) == false);
}

BOOST_AUTO_TEST_CASE(test_drop_from_overflow)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(2);
  for (int i = 1; i <= 5; ++i)
  {
    ht.add(i, i * 10);
  }

  BOOST_CHECK_EQUAL(ht.overflow_size(), 1);

  BOOST_CHECK_EQUAL(ht.drop(5), 1);
  BOOST_CHECK_EQUAL(ht.overflow_size(), 0);
  BOOST_CHECK_EQUAL(ht.size(), 4);
  BOOST_CHECK(ht.has(5) == false);
  BOOST_CHECK(ht.has(4));
}

BOOST_AUTO_TEST_CASE(test_iterator_skip_empty_buckets)
{
  HashTable< int, int > ht(10);

  ht.add(1, 10);
  ht.add(42, 420);

  int count = 0;
  for (auto it = ht.begin(); it != ht.end(); ++it)
  {
    count++;
  }

  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_const_iterators_and_range_for)
{
  HashTable< int, int > ht;
  ht.add(1, 100);
  ht.add(2, 200);
  ht.add(3, 300);

  const HashTable< int, int > &const_ht = ht;

  int sum = 0;

  for (const auto &item : const_ht)
  {
    sum += item.second;
  }

  BOOST_CHECK_EQUAL(sum, 600);
}

BOOST_AUTO_TEST_CASE(test_operator_brackets_creation_exception_safety)
{
  struct DummyHash
  {
    size_t operator()(int p) const
    {
      return 0;
    }
  };

  HashTable< int, std::string, DummyHash > ht(1);
  ht.set_overflow_limit(0);

  ht[1] = "A";
  ht[2] = "B";
  ht[3] = "C";
  ht[4] = "D";

  BOOST_CHECK_THROW(ht[5] = "E", std::overflow_error);
  BOOST_CHECK_EQUAL(ht.size(), 4);
  BOOST_CHECK(ht.has(5) == false);
}

BOOST_AUTO_TEST_CASE(test_find_existing_key_positive)
{
  HashTable< int, std::string > ht;
  ht.add(1, "one");
  ht.add(2, "two");

  HashTable< int, std::string >::iterator found = ht.find(1);

  BOOST_REQUIRE(found != ht.end());
  BOOST_CHECK_EQUAL(found->first, 1);
  BOOST_CHECK_EQUAL(found->second, "one");
}

BOOST_AUTO_TEST_CASE(test_find_every_slot_of_bucket)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(0);
  for (int i = 1; i <= 4; ++i)
  {
    ht.add(i, i * 10);
  }

  for (int i = 1; i <= 4; ++i)
  {
    HashTable< int, int, DummyHash >::iterator found = ht.find(i);
    BOOST_REQUIRE(found != ht.end());
    BOOST_CHECK_EQUAL(found->first, i);
    BOOST_CHECK_EQUAL(found->second, i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_find_missing_key_negative)
{
  HashTable< int, std::string > ht;
  ht.add(1, "one");

  BOOST_CHECK(ht.find(42) == ht.end());
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(ht.has(42) == false);
}

BOOST_AUTO_TEST_CASE(test_find_in_empty_table_negative)
{
  HashTable< int, std::string > ht;

  BOOST_CHECK(ht.find(1) == ht.end());
  BOOST_CHECK(ht.empty());
}

BOOST_AUTO_TEST_CASE(test_find_modify_through_iterator)
{
  HashTable< std::string, int > ht;
  ht.add("apples", 10);

  HashTable< std::string, int >::iterator found = ht.find("apples");
  BOOST_REQUIRE(found != ht.end());

  found->second += 5;

  BOOST_CHECK_EQUAL(ht["apples"], 15);
  BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_find_const_table)
{
  HashTable< int, int > ht;
  ht.add(1, 100);
  ht.add(2, 200);

  const HashTable< int, int > &const_ht = ht;

  HashTable< int, int >::const_iterator found = const_ht.find(2);
  BOOST_REQUIRE(found != const_ht.end());
  BOOST_CHECK_EQUAL(found->first, 2);
  BOOST_CHECK_EQUAL(found->second, 200);

  BOOST_CHECK(const_ht.find(3) == const_ht.end());
}

BOOST_AUTO_TEST_CASE(test_find_in_overflow)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(3);
  for (int i = 1; i <= 6; ++i)
  {
    ht.add(i, i * 10);
  }

  BOOST_REQUIRE_EQUAL(ht.overflow_size(), 2);

  HashTable< int, int, DummyHash >::iterator in_bucket = ht.find(2);
  HashTable< int, int, DummyHash >::iterator in_overflow = ht.find(6);

  BOOST_REQUIRE(in_bucket != ht.end());
  BOOST_REQUIRE(in_overflow != ht.end());
  BOOST_CHECK_EQUAL(in_bucket->second, 20);
  BOOST_CHECK_EQUAL(in_overflow->first, 6);
  BOOST_CHECK_EQUAL(in_overflow->second, 60);
  BOOST_CHECK(ht.find(7) == ht.end());
}

BOOST_AUTO_TEST_CASE(test_find_iterator_continues_traversal)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(2);
  for (int i = 1; i <= 6; ++i)
  {
    ht.add(i, i * 10);
  }

  // итератор от find должен вести себя как итератор обхода: от элемента бакета
  // проход до end() захватывает и остаток бакета, и весь overflow
  int count = 0;
  for (HashTable< int, int, DummyHash >::iterator it = ht.find(1); it != ht.end(); ++it)
  {
    count++;
  }
  BOOST_CHECK_EQUAL(count, 6);

  count = 0;
  for (HashTable< int, int, DummyHash >::iterator it = ht.find(5); it != ht.end(); ++it)
  {
    count++;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_find_in_overflow_after_bucket_drop)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  for (int i = 1; i <= 5; ++i)
  {
    ht.add(i, i * 10);
  }

  ht.drop(1);

  // 5 остался в overflow, хотя в бакете освободилось место
  HashTable< int, int, DummyHash >::iterator found = ht.find(5);
  BOOST_REQUIRE(found != ht.end());
  BOOST_CHECK_EQUAL(found->second, 50);
}

BOOST_AUTO_TEST_CASE(test_find_after_drop_negative)
{
  HashTable< int, std::string > ht;
  ht.add(1, "one");
  ht.drop(1);

  BOOST_CHECK(ht.find(1) == ht.end());
}

BOOST_AUTO_TEST_CASE(test_find_after_rehash)
{
  HashTable< int, int > ht(2);
  for (int i = 0; i < 8; ++i)
  {
    ht.add(i, i * 10);
  }

  ht.rehash(16);

  for (int i = 0; i < 8; ++i)
  {
    HashTable< int, int >::iterator found = ht.find(i);
    BOOST_REQUIRE(found != ht.end());
    BOOST_CHECK_EQUAL(found->second, i * 10);
  }
}

BOOST_AUTO_TEST_CASE(test_drop_by_iterator_end)
{
  HashTable< int, int > ht;
  ht.add(1, 10);

  HashTable< int, int >::iterator result = ht.drop(ht.end());

  BOOST_CHECK(result == ht.end());
  BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_drop_by_iterator_from_bucket)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(0);
  for (int i = 1; i <= 4; ++i)
  {
    ht.add(i, i * 10);
  }

  HashTable< int, int, DummyHash >::iterator next = ht.drop(ht.find(2));

  BOOST_CHECK_EQUAL(ht.size(), 3);
  BOOST_CHECK(ht.has(2) == false);
  BOOST_REQUIRE(next != ht.end());
  BOOST_CHECK(next->first != 2);
  BOOST_CHECK(ht.has(next->first));
}

BOOST_AUTO_TEST_CASE(test_drop_by_iterator_last_in_bucket_goes_to_overflow)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(1);
  for (int i = 1; i <= 5; ++i)
  {
    ht.add(i, i * 10);
  }
  BOOST_REQUIRE_EQUAL(ht.overflow_size(), 1);

  // 4 — последний в единственном бакете, следующий по обходу — 5 из overflow
  HashTable< int, int, DummyHash >::iterator next = ht.drop(ht.find(4));

  BOOST_REQUIRE(next != ht.end());
  BOOST_CHECK_EQUAL(next->first, 5);
  BOOST_CHECK_EQUAL(next->second, 50);
  BOOST_CHECK_EQUAL(ht.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_drop_by_iterator_from_overflow)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return 0;
    }
  };

  HashTable< int, int, DummyHash > ht(1);
  ht.set_overflow_limit(2);
  for (int i = 1; i <= 6; ++i)
  {
    ht.add(i, i * 10);
  }
  BOOST_REQUIRE_EQUAL(ht.overflow_size(), 2);

  HashTable< int, int, DummyHash >::iterator next = ht.drop(ht.find(5));

  BOOST_REQUIRE(next != ht.end());
  BOOST_CHECK_EQUAL(next->first, 6);
  BOOST_CHECK_EQUAL(ht.overflow_size(), 1);
  BOOST_CHECK_EQUAL(ht.size(), 5);

  // последний элемент обхода: дальше только end()
  next = ht.drop(next);
  BOOST_CHECK(next == ht.end());
  BOOST_CHECK_EQUAL(ht.overflow_size(), 0);
  BOOST_CHECK_EQUAL(ht.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_find_then_drop_takes_value)
{
  HashTable< int, std::string > ht;
  ht.add(1, "data");
  ht.add(2, "other");

  HashTable< int, std::string >::iterator found = ht.find(1);
  BOOST_REQUIRE(found != ht.end());
  std::string taken = std::move(found->second);
  ht.drop(found);

  BOOST_CHECK_EQUAL(taken, "data");
  BOOST_CHECK(ht.has(1) == false);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht[2], "other");
}

BOOST_AUTO_TEST_CASE(test_drop_all_while_iterating)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return a % 2;
    }
  };

  HashTable< int, int, DummyHash > ht(2);
  ht.set_overflow_limit(4);
  for (int i = 0; i < 12; ++i)
  {
    ht.add(i, i * 10);
  }
  BOOST_REQUIRE_EQUAL(ht.overflow_size(), 4);

  int dropped = 0;
  HashTable< int, int, DummyHash >::iterator it = ht.begin();
  while (it != ht.end())
  {
    it = ht.drop(it);
    dropped++;
  }

  BOOST_CHECK_EQUAL(dropped, 12);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.overflow_size(), 0);
}

BOOST_AUTO_TEST_CASE(test_drop_some_while_iterating_visits_each_once)
{
  struct DummyHash
  {
    size_t operator()(int a) const
    {
      return a % 3;
    }
  };

  const int count = 15;
  HashTable< int, int, DummyHash > ht(3);
  ht.set_overflow_limit(3);
  for (int i = 0; i < count; ++i)
  {
    ht.add(i, i * 10);
  }
  BOOST_REQUIRE_EQUAL(ht.overflow_size(), 3);

  // удаляем чётные ключи прямо во время обхода; каждый элемент должен быть посещён ровно один раз
  int visits[count] = {};
  HashTable< int, int, DummyHash >::iterator it = ht.begin();
  while (it != ht.end())
  {
    visits[it->first]++;
    if (it->first % 2 == 0)
    {
      it = ht.drop(it);
    }
    else
    {
      ++it;
    }
  }

  for (int i = 0; i < count; ++i)
  {
    BOOST_CHECK_EQUAL(visits[i], 1);
    BOOST_CHECK_EQUAL(ht.has(i), i % 2 != 0);
  }
  BOOST_CHECK_EQUAL(ht.size(), 7);
}

BOOST_AUTO_TEST_SUITE_END()
