#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <stdexcept>

#include "double-hash-table.hpp"

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
      return static_cast< size_t >((value * 37) + 17);
    }
  };

  using Table = lukashevich::HashTable< int, int, FirstHash, SecondHash >;
}

BOOST_AUTO_TEST_SUITE(DoubleHashTableTests)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  const Table table;

  BOOST_CHECK(table.isEmpty());
  BOOST_CHECK(table.getSize() == 0);
  BOOST_CHECK(table.cbegin() == table.cend());
}

BOOST_AUTO_TEST_CASE(InsertAndAt)
{
  Table table;

  table.insert(1, 10);
  table.insert(2, 20);
  table.insert(3, 30);

  BOOST_CHECK(!table.isEmpty());
  BOOST_CHECK(table.getSize() == 3);

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

  BOOST_CHECK(table.getSize() == 1);
  BOOST_CHECK(table.contains(1));
  BOOST_CHECK(table.at(1) == 42);
}

BOOST_AUTO_TEST_CASE(Erase)
{
  Table table;

  table.insert(1, 10);
  table.insert(2, 20);

  table.erase(1);

  BOOST_CHECK(table.getSize() == 1);
  BOOST_CHECK(!table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK(table.at(2) == 20);
}

BOOST_AUTO_TEST_CASE(EraseAndInsertAgain)
{
  Table table;

  table.insert(1, 10);
  table.insert(2, 20);
  table.erase(1);
  table.insert(3, 30);

  BOOST_CHECK(table.getSize() == 2);
  BOOST_CHECK(!table.contains(1));
  BOOST_CHECK(table.contains(2));
  BOOST_CHECK(table.contains(3));

  BOOST_CHECK(table.at(2) == 20);
  BOOST_CHECK(table.at(3) == 30);
}

BOOST_AUTO_TEST_CASE(Exceptions)
{
  Table table;

  BOOST_CHECK_THROW(table.at(1), std::out_of_range);
  BOOST_CHECK_THROW(table.erase(1), std::out_of_range);

  table.insert(1, 10);

  BOOST_CHECK_NO_THROW(table.at(1));
  BOOST_CHECK_NO_THROW(table.erase(1));
  BOOST_CHECK_THROW(table.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Iterators)
{
  Table table;

  table.insert(1, 10);
  table.insert(2, 20);
  table.insert(3, 30);

  size_t count = 0;
  int keySum = 0;
  int valueSum = 0;

  for (Table::Iterator it = table.begin(); it != table.end(); ++it) {
    keySum += it.getKey();
    valueSum += it.getValue();
    ++count;
  }

  BOOST_CHECK(count == table.getSize());
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
  int keySum = 0;
  int valueSum = 0;

  for (Table::ConstIterator it = constTable.cbegin();
      it != constTable.cend();
      ++it) {
    keySum += it.getKey();
    valueSum += it.getValue();
    ++count;
  }

  BOOST_CHECK(count == constTable.getSize());
  BOOST_CHECK(keySum == 3);
  BOOST_CHECK(valueSum == 30);
}

BOOST_AUTO_TEST_CASE(RehashByManyInsertions)
{
  Table table;

  for (int i = 0; i < 100; ++i) {
    table.insert(i, i * 10);
  }

  BOOST_CHECK(table.getSize() == 100);

  for (int i = 0; i < 100; ++i) {
    BOOST_CHECK(table.contains(i));
    BOOST_CHECK(table.at(i) == i * 10);
  }
}

BOOST_AUTO_TEST_CASE(CopyConstruction)
{
  Table source;

  source.insert(1, 10);
  source.insert(2, 20);

  Table copy(source);

  source.insert(1, 100);

  BOOST_CHECK(copy.getSize() == 2);
  BOOST_CHECK(copy.contains(1));
  BOOST_CHECK(copy.contains(2));
  BOOST_CHECK(copy.at(1) == 10);
  BOOST_CHECK(copy.at(2) == 20);

  BOOST_CHECK(source.at(1) == 100);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  Table source;

  source.insert(1, 10);
  source.insert(2, 20);

  Table assigned;

  assigned.insert(3, 30);
  assigned = source;

  source.insert(1, 100);

  BOOST_CHECK(assigned.getSize() == 2);
  BOOST_CHECK(assigned.contains(1));
  BOOST_CHECK(assigned.contains(2));
  BOOST_CHECK(!assigned.contains(3));

  BOOST_CHECK(assigned.at(1) == 10);
  BOOST_CHECK(assigned.at(2) == 20);
}

BOOST_AUTO_TEST_SUITE_END()
