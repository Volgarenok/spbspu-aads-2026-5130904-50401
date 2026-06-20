#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include "hash_func.hpp"
#include "hash_table_iter.hpp"
#include <string>
#include <iostream>

namespace malashenko
{
  using ht_t = mal::HashTable< std::string, int, mal::HmacHash< std::string >, mal::Equal< std::string > >;
  using htIter_t = mal::HashTableIter< std::string, int, mal::HmacHash< std::string >, mal::Equal< std::string > >;
}
namespace mal = malashenko;

BOOST_AUTO_TEST_SUITE(Hash_table_constructors)
BOOST_AUTO_TEST_CASE(common_constructors)
{
  mal::ht_t ht_empty = mal::ht_t();
  BOOST_CHECK(ht_empty.size() == 1);

  mal::ht_t ht(8);
  BOOST_CHECK(ht.size() == 8);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  mal::ht_t ht_empty = mal::ht_t();
  mal::ht_t ht_empty_copy(ht_empty);
  BOOST_CHECK(ht_empty.size() == ht_empty_copy.size());

  mal::ht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;

  mal::ht_t ht_copy(ht);
  BOOST_CHECK(ht.size() == ht_copy.size());
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}


BOOST_AUTO_TEST_CASE(move_constructor)
{
  mal::ht_t ht_empty = mal::ht_t();
  size_t ht_empty_size = ht_empty.size();
  mal::ht_t ht_empty_copy(std::move(ht_empty));
  BOOST_CHECK(ht_empty_copy.size() == ht_empty_size);

  mal::ht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;
  size_t ht_size = ht.size();

  mal::ht_t ht_copy(std::move(ht));
  BOOST_CHECK(ht_copy.size() == ht_size);
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Hash_table_operators)

BOOST_AUTO_TEST_CASE(copy_operator)
{
  mal::ht_t ht_empty = mal::ht_t();
  mal::ht_t ht_empty_copy;
  ht_empty = ht_empty_copy;
  BOOST_CHECK(ht_empty.size() == ht_empty_copy.size());

  mal::ht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;

  mal::ht_t ht_copy;
  ht_copy = ht;
  BOOST_CHECK(ht.size() == ht_copy.size());
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}

BOOST_AUTO_TEST_CASE(move_operator)
{
  mal::ht_t ht_empty = mal::ht_t();
  size_t ht_empty_size = ht_empty.size();
  mal::ht_t ht_empty_copy;
  ht_empty_copy = std::move(ht_empty);
  BOOST_CHECK(ht_empty_copy.size() == ht_empty_size);

  mal::ht_t ht(8);
  ht["a"] = 1;
  ht["b"] = 2;
  ht["c"] = 3;
  size_t ht_size = ht.size();

  mal::ht_t ht_copy;
  ht_copy = ht;
  BOOST_CHECK(ht_copy.size() == ht_size);
  BOOST_CHECK(ht_copy["a"] == 1);
  BOOST_CHECK(ht_copy["b"] == 2);
  BOOST_CHECK(ht_copy["c"] == 3);
}

BOOST_AUTO_TEST_CASE(get_operator)
{
  mal::ht_t ht(8);
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
  mal::ht_t ht(8);
  ht.insert("a", 1);
  ht.insert("b", 2);
  ht.insert("c", 3);

  BOOST_CHECK(ht["a"] == 1);
  BOOST_CHECK(ht["b"] == 2);
  BOOST_CHECK(ht["c"] == 3);
}

BOOST_AUTO_TEST_CASE(has_func)
{
  mal::ht_t ht(8);
  ht.insert("a", 1);
  ht.insert("b", 2);
  ht.insert("c", 3);

  BOOST_CHECK(ht.contains("a"));
  BOOST_CHECK(ht.contains("b"));
  BOOST_CHECK(ht.contains("c"));
  BOOST_CHECK(!ht.contains("d"));
}

BOOST_AUTO_TEST_CASE(get_func)
{
  mal::ht_t ht(8);
  ht.insert("a", 1);
  ht.insert("b", 2);
  ht.insert("c", 3);

  int a_var = ht.at("a");
  int b_var = ht.at("b");
  int c_var = ht.at("c");

  BOOST_CHECK(a_var == 1);
  BOOST_CHECK(b_var == 2);
  BOOST_CHECK(c_var == 3);

  try
  {
    int _ = ht.at("d");
    BOOST_FAIL("Should be an error");
  }
  catch(const std::exception& e)
  {
    BOOST_CHECK(!std::strcmp(e.what(), "Key not found"));
  }
}

BOOST_AUTO_TEST_CASE(drop_func)
{
  mal::ht_t ht(8);
  ht.insert("a", 1);
  ht.insert("b", 2);
  ht.insert("c", 3);

  try
  {
    ht.drop("d");
    BOOST_FAIL("Should be an error");
  }
  catch(const std::exception& e)
  {
    BOOST_CHECK(!std::strcmp(e.what(), "Key not found"));
  }

  ht.drop("a");
  try
  {
    int _ = ht.at("a");
    BOOST_FAIL("Should be an error");
  }
  catch(const std::exception& e)
  {
    BOOST_CHECK(!std::strcmp(e.what(), "Key not found"));
  }
}

BOOST_AUTO_TEST_CASE(rehash_func)
{
  mal::ht_t ht(8);
  ht.insert("a", 1);
  ht.insert("b", 2);
  ht.insert("c", 3);

  bool aFlag = false;
  bool bFlag = false;
  bool cFlag = false;
  size_t tmpSize = 0;
  ht.rehash(16);
  for (mal::htIter_t start = ht.begin(); start != ht.end(); ++start)
  {
    if (start->second == 1)
    {
      aFlag = true;
      ++tmpSize;
    } else if (start->second == 2)
    {
      bFlag = true;
      ++tmpSize;
    } else if (start->second == 3)
    {
      cFlag = true;
      ++tmpSize;
    }
  }
  BOOST_CHECK(ht.size() == 16);
  BOOST_CHECK(aFlag && bFlag && cFlag);
  BOOST_CHECK(tmpSize == 3);
}

BOOST_AUTO_TEST_SUITE_END()
