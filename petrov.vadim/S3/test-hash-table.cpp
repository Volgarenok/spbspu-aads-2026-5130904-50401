#include <boost/test/unit_test.hpp>
#include <string>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include "HashTable.hpp"
#include "Grath.hpp"
#include "HashFunc.hpp"
#include "../common/vector/top-it-vector.hpp"

namespace petrov
{
  using TestHT = HashTable< std::string, int, sha1, std::equal_to< std::string > >;
  using EdgeKey = std::pair< std::string, std::string >;
  using Weight = Vector< size_t >;
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

BOOST_AUTO_TEST_CASE(AddHasAt)
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

  BOOST_CHECK(table.at("a") == 1);
  BOOST_CHECK(table.at("b") == 2);
  BOOST_CHECK(table.at("c") == 3);
}

BOOST_AUTO_TEST_CASE(AddUpdatesExistingKey)
{
  petrov::TestHT table(8);
  table.add("key", 10);
  table.add("key", 99);

  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(table.at("key") == 99);
}

BOOST_AUTO_TEST_CASE(AtThrowsIfKeyNotFound)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  BOOST_CHECK_THROW(table.at("b"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(OperatorBracketCreatesDefault)
{
  petrov::TestHT table(8);
  table["new_key"] = 42;
  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(table.at("new_key") == 42);

  BOOST_CHECK(table["new_key"] == 42);
}

BOOST_AUTO_TEST_CASE(DropRemovesKeyAndReturnsTrue)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);

  bool success = table.drop("a");
  BOOST_CHECK(success);
  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(!table.has("a"));
  BOOST_CHECK(table.has("b"));
}

BOOST_AUTO_TEST_CASE(DropReturnsFalseIfKeyNotFound)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  bool success = table.drop("b");
  BOOST_CHECK(!success);
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
  BOOST_CHECK(copy.at("a") == 1);

  copy.add("a", 100);
  BOOST_CHECK(table.at("a") == 1);
  BOOST_CHECK(copy.at("a") == 100);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  petrov::TestHT table(8);
  table.add("a", 1);
  table.add("b", 2);

  petrov::TestHT moved(std::move(table));
  BOOST_CHECK(moved.size() == 2);
  BOOST_CHECK(moved.at("a") == 1);
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
  BOOST_CHECK(table.at("a") == 1);
  BOOST_CHECK(table.at("c") == 3);
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
    sum += it->second;
  }

  BOOST_CHECK(count == 2);
  BOOST_CHECK(sum == 40);
}

BOOST_AUTO_TEST_SUITE_END()
