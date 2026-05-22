#include <boost/test/unit_test.hpp>
#include <hashtable.hpp>

BOOST_AUTO_TEST_SUITE(hashtable_special_members_tests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht1(64);
  ht1.add(123, 42);
  ht1.add(321, 52);

  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht2(ht1);
  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK_EQUAL(ht2.at(123), 42);
  BOOST_CHECK_EQUAL(ht2.at(321), 52);

  ht2.add(456, 67);
  BOOST_CHECK_EQUAL(ht1.size(), 2);
  BOOST_CHECK_EQUAL(ht2.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht1(64);
  ht1.add(123, 42);
  ht1.add(321, 52);

  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht2(std::move(ht1));
  BOOST_CHECK_EQUAL(ht2.size(), 2);

  BOOST_CHECK(ht1.empty());

  ht2.add(456, 67);
  BOOST_CHECK_EQUAL(ht1.size(), 0);
  BOOST_CHECK_EQUAL(ht2.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_constructor_with_slots)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(64);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 64);

  ht.add(123, 42);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.at(123), 42);
}

BOOST_AUTO_TEST_CASE(test_constructor_with_params)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(16, 4, 8);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 72);

  ht.add(123, 42);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.at(123), 42);
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  auto * ht = new chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > >(64);
  ht->add(123, 42);
  ht->add(321, 52);
  ht->add(456, 67);

  BOOST_CHECK_EQUAL(ht->size(), 3);
  delete ht;
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht1(64);
  ht1.add(123, 42);
  ht1.add(321, 52);

  ht1 = ht1;
  BOOST_CHECK_EQUAL(ht1.size(), 2);
  BOOST_CHECK_EQUAL(ht1.at(123), 42);

  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht2;
  ht2 = ht1;

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK_EQUAL(ht2.at(123), 42);
  BOOST_CHECK_EQUAL(ht2.at(321), 52);

  ht2.add(456, 67);
  BOOST_CHECK_EQUAL(ht1.size(), 2);
  BOOST_CHECK_EQUAL(ht2.size(), 3);

  ht1.clear();
  BOOST_CHECK_EQUAL(ht1.size(), 0);
  ht2 = ht1;
  BOOST_CHECK_EQUAL(ht2.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_operator)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht1(64);
  ht1.add(123, 42);
  ht1.add(321, 52);

  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht2;
  ht2 = std::move(ht1);

  BOOST_CHECK(ht1.empty());

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK_EQUAL(ht2.at(123), 42);
  BOOST_CHECK_EQUAL(ht2.at(321), 52);

  ht2.add(456, 67);
  BOOST_CHECK_EQUAL(ht1.size(), 0);
  BOOST_CHECK_EQUAL(ht2.size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(hashtable_iterator_tests)

BOOST_AUTO_TEST_CASE(test_begin_iterator)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(64);
  BOOST_CHECK(ht.begin() == ht.end());

  ht.add(123, 42);
  ht.add(321, 52);

  auto iter = ht.begin();
  BOOST_CHECK(iter != ht.end());
  if ((iter++)->first == 123) {
    BOOST_CHECK_EQUAL(iter->first, 321);
  } else {
    BOOST_CHECK_EQUAL(iter->first, 123);
  }
  BOOST_CHECK(++iter == ht.end());
}

BOOST_AUTO_TEST_CASE(test_сbegin_iterator)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht1(64);
  BOOST_CHECK(ht1.cbegin() == ht1.cend());

  ht1.add(123, 42);
  ht1.add(321, 52);

  const chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > & ht2 = ht1;

  auto iter = ht2.cbegin();
  BOOST_CHECK(iter != ht2.cend());
  if ((iter++)->first == 123) {
    BOOST_CHECK_EQUAL(iter->first, 321);
  } else {
    BOOST_CHECK_EQUAL(iter->first, 123);
  }
  BOOST_CHECK(++iter == ht2.end());
}

BOOST_AUTO_TEST_CASE(test_end_iterator)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(64);
  BOOST_CHECK(ht.begin() == ht.end());

  ht.add(123, 42);
  ht.add(321, 52);

  size_t count = 0;
  for (auto iter = ht.begin(); iter != ht.end(); ++iter) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_cend_iterator)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht1(64);
  BOOST_CHECK(ht1.cbegin() == ht1.cend());

  ht1.add(123, 42);
  ht1.add(321, 52);

  const chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > & ht2 = ht1;

  size_t count = 0;
  for (auto iter = ht2.cbegin(); iter != ht2.cend(); ++iter) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(hashtable_capacity_tests)

BOOST_AUTO_TEST_CASE(test_empty)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(64);
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK(ht.empty());

  ht.add(123, 42);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK(!ht.empty());

  ht.clear();
  BOOST_CHECK_EQUAL(ht.size(), 0);
  BOOST_CHECK(ht.empty());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(64);
  BOOST_CHECK_EQUAL(ht.size(), 0);

  ht.add(123, 42);
  BOOST_CHECK_EQUAL(ht.size(), 1);

  ht.add(321, 52);
  BOOST_CHECK_EQUAL(ht.size(), 2);

  ht.remove(123);
  BOOST_CHECK_EQUAL(ht.size(), 1);

  ht.clear();
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_max_capacity)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 0);

  ht.rehash(64);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 64);

  ht.rehash(100);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 100);

  ht.rehash(16, 4, 8);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 72);
}

BOOST_AUTO_TEST_CASE(test_has)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(64);

  BOOST_CHECK(!ht.has(123));
  ht.add(123, 42);
  ht.add(321, 52);
  BOOST_CHECK(ht.has(123));

  ht.remove(123);
  BOOST_CHECK(!ht.has(123));
  BOOST_CHECK(ht.has(321));
}

BOOST_AUTO_TEST_CASE(test_at)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht(64);
  BOOST_CHECK_THROW(ht.at(123), std::out_of_range);

  ht.add(123, 42);
  BOOST_CHECK_EQUAL(ht.at(123), 42);

  ht.add(321, 52);
  BOOST_CHECK_EQUAL(ht.at(321), 52);

  ht.rehash(100);
  BOOST_CHECK_EQUAL(ht.at(321), 52);

  ht.clear();
  BOOST_CHECK_THROW(ht.at(321), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(hashtable_modifiers_tests)

BOOST_AUTO_TEST_CASE(test_swap)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht1(64);
  ht1.add(123, 42);
  ht1.add(321, 52);

  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht2;
  ht1.swap(ht2);

  BOOST_CHECK(ht1.empty());
  BOOST_CHECK_EQUAL(ht1.maxCapacity(), 0);
  BOOST_CHECK_THROW(ht1.at(123), std::out_of_range);

  BOOST_CHECK_EQUAL(ht2.size(), 2);
  BOOST_CHECK_EQUAL(ht2.maxCapacity(), 64);
  BOOST_CHECK_EQUAL(ht2.at(123), 42);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht;
  ht.clear();
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 0);

  ht.rehash(64);
  ht.add(123, 42);
  ht.add(321, 52);
  ht.clear();
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_THROW(ht.at(123), std::out_of_range);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 64);
}

BOOST_AUTO_TEST_CASE(test_add)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK_THROW(ht.add(123, 42), std::length_error);

  ht.rehash(1, 1, 1);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 2);

  ht.add(123, 42);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.at(123), 42);

  ht.add(321, 52);
  BOOST_CHECK_EQUAL(ht.size(), 2);
  BOOST_CHECK_EQUAL(ht.at(321), 52);

  BOOST_CHECK_THROW(ht.add(456, 67), std::length_error);

  ht.add(123, 78);
  BOOST_CHECK_EQUAL(ht.size(), 2);
  BOOST_CHECK_EQUAL(ht.at(123), 78);
}

BOOST_AUTO_TEST_CASE(test_remove)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK_THROW(ht.remove(123), std::out_of_range);

  ht.rehash(1, 2, 1);
  ht.add(123, 42);
  ht.remove(123);
  BOOST_CHECK(ht.empty());

  ht.add(123, 42);
  ht.add(321, 52);
  ht.add(456, 67);

  ht.remove(456);
  ht.remove(123);
  BOOST_CHECK_EQUAL(ht.size(), 1);
  BOOST_CHECK_EQUAL(ht.at(321), 52);
  ht.remove(321);
  BOOST_CHECK(ht.empty());
  BOOST_CHECK_THROW(ht.remove(321), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_rehash_with_slots)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 0);

  ht.rehash(64);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 64);
  for (int i = 0; i < 8; ++i) {
    ht.add(i, i * i);
  }

  ht.rehash(1024);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 1024);
  for (int i = 0; i < 8; ++i) {
    BOOST_CHECK_EQUAL(ht.at(i), i * i);
  }
}

BOOST_AUTO_TEST_CASE(test_rehash_with_params)
{
  chernov::HashTable< int, int, std::hash< int >, std::equal_to< int > > ht;
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 0);

  ht.rehash(1, 2, 1);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 3);
  ht.add(123, 42);
  ht.add(321, 52);
  ht.add(456, 67);

  ht.rehash(16, 4, 0);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 64);
  BOOST_CHECK_EQUAL(ht.at(123), 42);
  BOOST_CHECK_EQUAL(ht.at(321), 52);
  BOOST_CHECK_EQUAL(ht.at(456), 67);

  ht.rehash(1, 2, 1);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 3);
  BOOST_CHECK_EQUAL(ht.at(123), 42);
  BOOST_CHECK_EQUAL(ht.at(321), 52);
  BOOST_CHECK_EQUAL(ht.at(456), 67);

  BOOST_CHECK_THROW(ht.rehash(0, 0, 3), std::logic_error);
  BOOST_CHECK_THROW(ht.rehash(1, 2, 0), std::length_error);
  BOOST_CHECK_EQUAL(ht.maxCapacity(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
