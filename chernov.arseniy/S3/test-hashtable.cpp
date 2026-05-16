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

BOOST_AUTO_TEST_CASE(test_move_operator)
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

BOOST_AUTO_TEST_SUITE_END()
