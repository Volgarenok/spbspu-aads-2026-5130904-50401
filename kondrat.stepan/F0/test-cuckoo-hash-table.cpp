#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <stdexcept>
#include "cuckoo-hash-table.hpp"

namespace
{
  struct FirstHash
  {
    size_t operator()(int value) const
    {
      return static_cast< size_t >(value);
    }
  };

  struct SecondHash
  {
    size_t operator()(int value) const
    {
      return static_cast< size_t >(value * 7 + 3);
    }
  };

  using Table = kondrat::CuckooHashTable< int, int, FirstHash, SecondHash >;
}

BOOST_AUTO_TEST_SUITE(CuckooHashTableTests)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  const Table table;

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() >= 16);
  BOOST_CHECK(table.cbegin() == table.cend());
}

BOOST_AUTO_TEST_CASE(AddAndGet)
{
  Table table;
  table.insert(1, 10);
  table.insert(2, 20);
  table.insert(3, 30);

  BOOST_CHECK(!table.empty());
  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK(table.contains(3));
  BOOST_CHECK(table.at(1) == 10);
  BOOST_CHECK(table.at(2) == 20);
  BOOST_CHECK(table.at(3) == 30);
}

BOOST_AUTO_TEST_CASE(ReplaceValue)
{
  Table table;
  table.insert(1, 10);
  table.insert(1, 42);

  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(table.at(1) == 42);
}

BOOST_AUTO_TEST_CASE(Drop)
{
  Table table;
  table.insert(1, 10);
  table.insert(2, 20);

  BOOST_CHECK(table.erase(1) == 1);
  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(!table.contains(1));
  BOOST_CHECK(table.contains(2));
}

BOOST_AUTO_TEST_CASE(Clear)
{
  Table table;
  table.insert(1, 10);
  table.insert(2, 20);
  table.clear();

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(Exceptions)
{
  Table table;

  BOOST_CHECK_THROW(table.at(1), std::logic_error);
  BOOST_CHECK(table.erase(1) == 0);
  BOOST_CHECK_THROW(table.rehash(0), std::logic_error);
}

BOOST_AUTO_TEST_CASE(Iterators)
{
  Table table;
  table.insert(1, 10);
  table.insert(2, 20);
  table.insert(3, 30);

  size_t count = 0;
  int valueSum = 0;
  int keySum = 0;
  for (Table::Iterator it = table.begin(); it != table.end(); ++it)
  {
    keySum += it->first;
    valueSum += it->second;
    ++count;
  }

  BOOST_CHECK(count == table.size());
  BOOST_CHECK(keySum == 6);
  BOOST_CHECK(valueSum == 60);
}

BOOST_AUTO_TEST_CASE(ConstIterators)
{
  Table table;
  table.insert(1, 10);
  table.insert(2, 20);
  const Table & constTable = table;

  size_t count = 0;
  int valueSum = 0;
  for (Table::ConstIterator it = constTable.cbegin(); it != constTable.cend(); ++it)
  {
    valueSum += it->second;
    ++count;
  }

  BOOST_CHECK(count == constTable.size());
  BOOST_CHECK(valueSum == 30);
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  Table table;
  for (int i = 0; i < 50; ++i)
  {
    table.insert(i, i * 10);
  }

  const size_t oldCapacity = table.capacity();
  table.rehash(oldCapacity * 2);

  BOOST_CHECK(table.size() == 50);
  BOOST_CHECK(table.capacity() >= oldCapacity * 2);
  for (int i = 0; i < 50; ++i)
  {
    BOOST_CHECK(table.at(i) == i * 10);
  }
}

BOOST_AUTO_TEST_CASE(CopyConstructionAndAssignment)
{
  Table source;
  source.insert(1, 10);
  source.insert(2, 20);

  Table copy(source);
  Table assigned;
  assigned.insert(3, 30);
  assigned = source;

  source.insert(1, 100);

  BOOST_CHECK(copy.size() == 2);
  BOOST_CHECK(copy.at(1) == 10);
  BOOST_CHECK(copy.at(2) == 20);
  BOOST_CHECK(assigned.size() == 2);
  BOOST_CHECK(assigned.at(1) == 10);
  BOOST_CHECK(assigned.at(2) == 20);
  BOOST_CHECK(!assigned.contains(3));
}

BOOST_AUTO_TEST_CASE(Swap)
{
  Table lhs;
  lhs.insert(1, 10);
  Table rhs;
  rhs.insert(2, 20);
  rhs.insert(3, 30);

  lhs.swap(rhs);

  BOOST_CHECK(lhs.size() == 2);
  BOOST_CHECK(lhs.contains(2));
  BOOST_CHECK(lhs.contains(3));
  BOOST_CHECK(rhs.size() == 1);
  BOOST_CHECK(rhs.contains(1));
}

BOOST_AUTO_TEST_SUITE_END()
