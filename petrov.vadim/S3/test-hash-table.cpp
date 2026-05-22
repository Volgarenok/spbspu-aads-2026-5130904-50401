#include <boost/test/unit_test.hpp>
#include <string>
#include <utility>
#include <algorithm>
#include "HashTable.hpp"
#include "Grath.hpp"
#include "HashFunc.hpp"
#include "../common/vector/top-it-vector.hpp"

namespace petrov
{
  using TestHT = HashTable< std::string, int, sha1, Equal< std::string > >;
  using EdgeKey = std::pair< std::string, std::string >;
  using Weight = topit::Vector< size_t >;
  using EdgeVec = std::pair< std::string, Weight >;
}

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault)
{
  petrov::TestHT table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 8);
}

BOOST_AUTO_TEST_CASE(ConstructorWithCapacity)
{
  petrov::TestHT table(16);
  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 16);
}

BOOST_AUTO_TEST_CASE(AddHasGet)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);

  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.has("a"));
  BOOST_CHECK(table.has("b"));
  BOOST_CHECK(table.has("c"));
  BOOST_CHECK(!table.has("d"));

  BOOST_CHECK(table.get("a") == 1);
  BOOST_CHECK(table.get("b") == 2);
  BOOST_CHECK(table.get("c") == 3);
}

BOOST_AUTO_TEST_CASE(AddUpdatesExistingKey)
{
  petrov::TestHT table(8);
  table.add("key", 10);
  table.add("key", 99);

  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(table.get("key") == 99);
}

BOOST_AUTO_TEST_CASE(GetThrowsIfKeyNotFound)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  BOOST_CHECK_THROW(table.get("b"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(DropRemovesAndReturnsValue)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);

  int val = table.drop("a");
  BOOST_CHECK(val == 1);
  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(!table.has("a"));
  BOOST_CHECK(table.has("b"));
}

BOOST_AUTO_TEST_CASE(DropThrowsIfKeyNotFound)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  BOOST_CHECK_THROW(table.drop("b"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.clear();

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 8);
  BOOST_CHECK(!table.has("a"));
  BOOST_CHECK(!table.has("b"));
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);

  petrov::TestHT copy(table);
  BOOST_CHECK(copy.size() == 2);
  BOOST_CHECK(copy.get("a") == 1);

  copy.add("a", 100);
  BOOST_CHECK(table.get("a") == 1);
  BOOST_CHECK(copy.get("a") == 100);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);

  petrov::TestHT moved(std::move(table));
  BOOST_CHECK(moved.size() == 2);
  BOOST_CHECK(moved.get("a") == 1);
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.drop("b");

  table.rehash(32);
  BOOST_CHECK(table.capacity() == 32);
  BOOST_CHECK(table.size() == 2);
  BOOST_CHECK(table.get("a") == 1);
  BOOST_CHECK(table.get("c") == 3);
}

BOOST_AUTO_TEST_CASE(IteratorSkipsTombstones)
{
  petrov::TestHT table(8);
  table.add("a", 10);
  table.add("b", 20);
  table.add("c", 30);
  table.drop("b");

  size_t count = 0;
  int sum = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
    sum += it->value_;
  }

  BOOST_CHECK(count == 2);
  BOOST_CHECK(sum == 40);
}

BOOST_AUTO_TEST_SUITE_END()
