#include <boost/test/unit_test.hpp>
#include "my_hash_table.hpp"

using ht_ci4 = goltsov::HashTable< char, int, goltsov::Sha1Hasher< char >, std::equal_to< char > >;
using ht_ci4_it = goltsov::HashTableIterator< char, int, goltsov::Sha1Hasher< char >, std::equal_to< char > >;
using ht_ci4_cit = goltsov::HashTableConstIterator< char, int, goltsov::Sha1Hasher< char >, std::equal_to< char > >;

BOOST_AUTO_TEST_SUITE(HashTable_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  ht_ci4 t;
  BOOST_CHECK(t.size() == 1 && t.count() == 0 && t.countBuckets() == 1);
  t['a'] = 1;
  t['b'] = 2;
  ht_ci4 t1 (t);
  BOOST_CHECK(t1.size() == 1 && t1.count() == 2 && t1.countBuckets() == 1);
  ht_ci4 t2 (std::move(t));
  BOOST_CHECK(t2.size() == 1 && t2.count() == 2 && t2.countBuckets() == 1);
  ht_ci4 t3;
  t3['a'] = 1;
  t3['b'] = 2;
  ht_ci4 t4 = t3;
  BOOST_CHECK(t4.size() == 1 && t4.count() == 2 && t4.countBuckets() == 1);
  ht_ci4 t5 = std::move(t3);
  BOOST_CHECK(t5.size() == 1 && t5.count() == 2 && t5.countBuckets() == 1);
  ht_ci4 t6 (5, 4);
  BOOST_CHECK(t6.size() == 20 && t6.count() == 0 && t6.countBuckets() == 5);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  ht_ci4 t;
  t['a'] = 1;
  t['b'] = 2;
  ht_ci4 t1 (2, 2);
  t1['a'] = 1;
  t.swap(t1);
  BOOST_CHECK(t1.size() == 1 && t1.count() == 2 && t1.countBuckets() == 1
    && t.size() == 4 && t.count() == 1 && t.countBuckets() == 2);
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  ht_ci4 t;
  t.insert(std::pair< char, int >{'a', 1});
  BOOST_CHECK(t['a'] == 1);
  BOOST_CHECK((t.insert(std::pair< char, int >{'a', 1})) == (std::pair< ht_ci4_it, bool >{t.begin(), false}));
  int q = 2;
  t.insert(std::pair< char, int >{'b', q});
  BOOST_CHECK(t['b'] == q);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  ht_ci4 t;
  t.insert(std::pair< char, int >{'a', 1});
  BOOST_CHECK(t.erase('a') && !t.contains('a'));
  BOOST_CHECK(t.erase('a') == 0);
}

BOOST_AUTO_TEST_CASE(recontainsh_test)
{
  ht_ci4 t;
  t['a'] = 1;
  t.rehash(4, 10);
  BOOST_CHECK(t.size() == 40 && t.countBuckets() == 4 && t['a'] == 1);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  ht_ci4 t (4, 4);
  t['a'] = 1;
  t['b'] = 2;
  t.clear();
  BOOST_CHECK(t.countBuckets() == 4 && t.count() == 0);
}

BOOST_AUTO_TEST_CASE(size_countBuckets_count_capacity_test)
{
  ht_ci4 t (4, 4);
  t['a'] = 1;
  t['b'] = 2;
  BOOST_CHECK(t.size() == 16 && t.countBuckets() == 4 && t.count() == 2 && t.capacity() == 4);
}

BOOST_AUTO_TEST_CASE(contains_test)
{
  ht_ci4 t;
  t['a'] = 1;
  BOOST_CHECK(t.contains('a') && !t.contains('b'));
}

BOOST_AUTO_TEST_CASE(at_test)
{
  ht_ci4 t;
  t['a'] = 1;
  BOOST_CHECK(t.at('a') == 1);
  BOOST_CHECK_THROW(t.at('b'), std::logic_error);
}

BOOST_AUTO_TEST_CASE(opertor_square_brackets_test)
{
  ht_ci4 t;
  t['a'] = 1;
  char q = 'a';
  char q1 = 'b';
  BOOST_CHECK(t['a'] == 1 && t[q] == 1);
  BOOST_CHECK_NO_THROW(t['b'] && t[q1]);
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4 t1;
  const ht_ci4 tc = t;
  const ht_ci4 tc1 = t1;
  BOOST_CHECK(t.begin()->second == 1 && t.begin()->first == 'a' && t.begin() != t.end()
    && t1.begin() == t1.end());
  BOOST_CHECK(tc.cbegin()->second == 1 && tc.cbegin()->first == 'a' && tc.cbegin() != tc.cend()
    && tc1.cbegin() == tc1.cend());
}

BOOST_AUTO_TEST_CASE(end_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4 t1;
  const ht_ci4 tc = t;
  const ht_ci4 tc1 = t1;
  BOOST_CHECK(t.begin() != t.end() && t1.begin() == t1.end());
  BOOST_CHECK(tc.cbegin() != t.cend() && tc1.cbegin() == tc1.cend());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Iterators_suite)

BOOST_AUTO_TEST_CASE(constructor_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.cbegin();
  BOOST_CHECK(i->second == 1 && ci->second == 1);
}

BOOST_AUTO_TEST_CASE(key_value_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.cbegin();
  BOOST_CHECK(i->second == 1 && ci->second == 1 && i->first == 'a' && ci->first == 'a');
}

BOOST_AUTO_TEST_CASE(operator_plus_plus_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.cbegin();
  BOOST_CHECK(++i == t.end() && ++ci == t1.cend());
}

BOOST_AUTO_TEST_CASE(dereference_operators_test)
{
  ht_ci4 t;
  t['a'] = 1;
  ht_ci4_it i = t.begin();
  const ht_ci4 t1 = t;
  ht_ci4_cit ci = t1.cbegin();
  BOOST_CHECK((* i).second == 1 && i->second == 1 && (* ci).second == 1 && ci->second == 1);
}

BOOST_AUTO_TEST_SUITE_END()
