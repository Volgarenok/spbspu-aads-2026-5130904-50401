#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "cuckoo_hash_table.hpp"
#include "hash_funcs.hpp"
#include "equal_functor.hpp"
#include "cuckoo_iterator.hpp"

namespace mal = malashenko;
using cht_t = mal::CuckooHashTable< std::string, int,
              mal::HmacHash< std::string >, mal::SipHasher< std::string >,
              mal::Equal< std::string > >;

using chtIter_t = mal::Iterator< std::string, int,
              mal::HmacHash< std::string >, mal::SipHasher< std::string >,
              mal::Equal< std::string > >;

BOOST_AUTO_TEST_SUITE(Hash_table_constructors)
BOOST_AUTO_TEST_CASE(common_constructors)
{
  cht_t ht_empty = cht_t();
  BOOST_CHECK(ht_empty.capacity() == 8);
  BOOST_CHECK(ht_empty.size() == 0);


  cht_t ht(10);
  BOOST_CHECK(ht.capacity() == 10);
  BOOST_CHECK(ht.size() == 0);

}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  cht_t ht_empty = cht_t();
  cht_t ht_empty_copy(ht_empty);
  BOOST_CHECK(ht_empty.size() == ht_empty_copy.size());

  cht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;

  cht_t ht_copy(ht);
  BOOST_CHECK(ht.size() == ht_copy.size());
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}


BOOST_AUTO_TEST_CASE(move_constructor)
{
  cht_t ht_empty = cht_t();
  size_t ht_empty_size = ht_empty.size();
  cht_t ht_empty_copy(std::move(ht_empty));
  BOOST_CHECK(ht_empty_copy.size() == ht_empty_size);

  cht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;
  size_t ht_size = ht.size();

  cht_t ht_copy(std::move(ht));
  BOOST_CHECK(ht_copy.size() == ht_size);
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Hash_table_operators)

BOOST_AUTO_TEST_CASE(copy_operator)
{
  cht_t ht_empty = cht_t();
  cht_t ht_empty_copy;
  ht_empty = ht_empty_copy;
  BOOST_CHECK(ht_empty.size() == ht_empty_copy.size());

  cht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;

  cht_t ht_copy;
  ht_copy = ht;
  BOOST_CHECK(ht.size() == ht_copy.size());
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}

BOOST_AUTO_TEST_CASE(move_operator)
{
  cht_t ht_empty = cht_t();
  size_t ht_empty_size = ht_empty.size();
  cht_t ht_empty_copy;
  ht_empty_copy = std::move(ht_empty);
  BOOST_CHECK(ht_empty_copy.size() == ht_empty_size);

  cht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;
  size_t ht_size = ht.size();

  cht_t ht_copy;
  ht_copy = ht;
  BOOST_CHECK(ht_copy.size() == ht_size);
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}

BOOST_AUTO_TEST_CASE(get_operator)
{
  cht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;

  BOOST_CHECK(ht["a"] == 1);
  BOOST_CHECK(ht["b"] == 2);
  BOOST_CHECK(ht["c"] == 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Hash_table_var_funcs)
BOOST_AUTO_TEST_CASE(add_func)
{
  cht_t ht(8);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);

  BOOST_CHECK(ht["a"] == 1);
  BOOST_CHECK(ht["b"] == 2);
  BOOST_CHECK(ht["c"] == 3);
  BOOST_CHECK(ht.size() == 3);
}

BOOST_AUTO_TEST_CASE(has_func)
{
  cht_t ht(8);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);

  BOOST_CHECK(ht.has("a"));
  BOOST_CHECK(ht.has("b"));
  BOOST_CHECK(ht.has("c"));
  BOOST_CHECK(!ht.has("d"));
}

BOOST_AUTO_TEST_CASE(get_func)
{
  cht_t ht(8);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);

  int a_var = ht.get("a");
  int b_var = ht.get("b");
  int c_var = ht.get("c");

  BOOST_CHECK(a_var == 1);
  BOOST_CHECK(b_var == 2);
  BOOST_CHECK(c_var == 3);

  try
  {
    int _ = ht.get("d");
    BOOST_FAIL("Should be an error");
  }
  catch(const std::exception& e)
  {
    BOOST_CHECK(!std::strcmp(e.what(), "Unknown key"));
  }
}

BOOST_AUTO_TEST_CASE(drop_func)
{
  cht_t ht(8);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);

  try
  {
    ht.drop("d");
    BOOST_FAIL("Should be an error");
  }
  catch(const std::exception& e)
  {
    BOOST_CHECK(!std::strcmp(e.what(), "Unknown key"));
  }
  BOOST_CHECK(ht.size() == 3);

  ht.drop("a");
  try
  {
    int _ = ht.get("a");
    BOOST_FAIL("Should be an error");
  }
  catch(const std::exception& e)
  {
    BOOST_CHECK(!std::strcmp(e.what(), "Unknown key"));
  }
  BOOST_CHECK(ht.size() == 2);

}

BOOST_AUTO_TEST_CASE(rehash_func)
{
  cht_t ht(8);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);

  bool aFlag = false;
  bool bFlag = false;
  bool cFlag = false;
  size_t tmpSize = 0;
  ht.rehash(16);
  for (chtIter_t start = ht.begin(); start != ht.end(); ++start)
  {
    if (start->value == 1)
    {
      aFlag = true;
      ++tmpSize;
    } else if (start->value == 2)
    {
      bFlag = true;
      ++tmpSize;
    } else if (start->value == 3)
    {
      cFlag = true;
      ++tmpSize;
    }
  }
  BOOST_CHECK(ht.capacity() == 16);
  BOOST_CHECK(aFlag && bFlag && cFlag);
  BOOST_CHECK(tmpSize == 3);
}

BOOST_AUTO_TEST_SUITE_END()