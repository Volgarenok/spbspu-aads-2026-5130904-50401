#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include "hmac_hash.hpp"

using namespace tarasenko;

using HTable = HashTable< int, int, HmacHash< int >, std::equal_to< int > >;

BOOST_AUTO_TEST_CASE(hash_is_equal_if_key_is_equal)
{
  HmacHash< int > hash1("my_key");
  HmacHash< int > hash2("my_key");
  BOOST_TEST(hash1(67) == hash2(67));
}

BOOST_AUTO_TEST_CASE(hash_is_not_equal_if_key_is_not_equal)
{
  HmacHash< int > hash1("my_key");
  HmacHash< int > hash2("not_my_key");
  BOOST_TEST(hash1(67) != hash2(67));
}

BOOST_AUTO_TEST_CASE(hash_table)
{
  HTable table;
  table.add(1, 1);
  BOOST_TEST(table.has(1));
  BOOST_TEST(!table.has(2));
  BOOST_TEST(table.at(1) == 1);
}
