#include <memory>
#include <string>
#include <utility>
#include <boost/test/unit_test.hpp>
#include "hash-func.hpp"
#include "hash-table.hpp"

namespace
{
  using TestHashTable = kondrat::HashTable< std::string, int, kondrat::blake2 >;

  struct ConstantHash
  {
    size_t operator()(const std::string &) const noexcept
    {
      return 0;
    }
  };
}

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault)
{
  TestHashTable table;
  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 17);
}

BOOST_AUTO_TEST_CASE(ConstructorWithCapacity)
{
  TestHashTable table(8);
  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 8);
}

BOOST_AUTO_TEST_CASE(AddContainsAt)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);

  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.contains("a"));
  BOOST_CHECK(table.contains("b"));
  BOOST_CHECK(table.contains("c"));
  BOOST_CHECK(!table.contains("d"));
  BOOST_CHECK(table.at("a") == 1);
  BOOST_CHECK(table.at("b") == 2);
  BOOST_CHECK(table.at("c") == 3);
}

BOOST_AUTO_TEST_CASE(AddDoesNotReplaceMatchingKey)
{
  kondrat::HashTable< std::string, int, ConstantHash > table(8);
  table.add("first", 1);
  table.add("second", 2);
  table.add("first", 100);

  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.at("first") == 1);
  BOOST_CHECK(table.at("second") == 2);

  int sum = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    sum += it->second;
  }
  BOOST_CHECK(sum == 103);
}

BOOST_AUTO_TEST_CASE(AddRvalue)
{
  kondrat::HashTable< std::string, std::unique_ptr< int >, ConstantHash > table(8);
  std::unique_ptr< int > value(new int(42));
  table.add("answer", std::move(value));

  BOOST_CHECK(value == nullptr);
  BOOST_CHECK(*table.at("answer") == 42);
}

BOOST_AUTO_TEST_CASE(AtThrowsIfKeyNotFound)
{
  TestHashTable table(8);
  table.add("a", 1);
  BOOST_CHECK_THROW(table.at("b"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(EraseRemovesElement)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.erase("a");

  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(!table.contains("a"));
  BOOST_CHECK(table.contains("b"));
}

BOOST_AUTO_TEST_CASE(EraseThrowsIfKeyNotFound)
{
  TestHashTable table(8);
  table.add("a", 1);
  BOOST_CHECK_THROW(table.erase("b"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.clear();

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 8);
  BOOST_CHECK(!table.contains("a"));
  BOOST_CHECK(!table.contains("b"));
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  TestHashTable copy(table);
  copy.add("a", 100);

  BOOST_CHECK(copy.size() == 3);
  BOOST_CHECK(table.at("a") == 1);
  BOOST_CHECK(copy.at("a") == 1);
  BOOST_CHECK(copy.at("b") == 2);

  int sum = 0;
  for (auto it = copy.begin(); it != copy.end(); ++it)
  {
    sum += it->second;
  }
  BOOST_CHECK(sum == 103);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  TestHashTable copy;
  copy = table;
  copy.erase("a");

  BOOST_CHECK(copy.size() == 1);
  BOOST_CHECK(table.contains("a"));
  BOOST_CHECK(!copy.contains("a"));
  BOOST_CHECK(copy.at("b") == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  TestHashTable moved(std::move(table));

  BOOST_CHECK(moved.size() == 2);
  BOOST_CHECK(moved.at("a") == 1);
  BOOST_CHECK(moved.at("b") == 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  TestHashTable moved;
  moved = std::move(table);

  BOOST_CHECK(moved.size() == 2);
  BOOST_CHECK(moved.at("a") == 1);
  BOOST_CHECK(moved.at("b") == 2);
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.rehash(32);

  BOOST_CHECK(table.capacity() == 32);
  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.at("a") == 1);
  BOOST_CHECK(table.at("b") == 2);
  BOOST_CHECK(table.at("c") == 3);
}

BOOST_AUTO_TEST_CASE(IteratorReturnsValueType)
{
  TestHashTable table(8);
  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.erase("b");

  size_t count = 0;
  int sum = 0;
  for (kondrat::HTIter< std::string, int > it = table.begin(); it != table.end(); ++it)
  {
    ++count;
    sum += it->second;
    it->second += 1;
  }

  BOOST_CHECK(count == 2);
  BOOST_CHECK(sum == 4);
  BOOST_CHECK(table.at("a") == 2);
  BOOST_CHECK(table.at("c") == 4);
}

BOOST_AUTO_TEST_SUITE_END()
