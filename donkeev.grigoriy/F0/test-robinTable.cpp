#include <boost/test/unit_test.hpp>
#include <string>

#include "robinTable.hpp"
#include "robinTableStructs.hpp"

BOOST_AUTO_TEST_SUITE(robinTbaleTesting)

using Table = donkeev::RobinTable< std::string, size_t, donkeev::StringHash, donkeev::StringEqual >;
using Iterator = donkeev::RobinIter< std::string, size_t, donkeev::StringHash, donkeev::StringEqual >;
using ConstIterator = donkeev::RobinCIter< std::string, size_t, donkeev::StringHash, donkeev::StringEqual >;

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  Table table(16);
  table.insert("first", 1);

  Table other(table);
  BOOST_CHECK(other.size() == 1);
}

BOOST_AUTO_TEST_CASE(movementConstructor)
{
  Table table(16);
  table.insert("first", 1);

  Table other(std::move(table));
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(other.size() == 1);
}

BOOST_AUTO_TEST_CASE(equalOperator)
{
  Table table(16);
  table.insert("first", 1);
  Table other(16);
  table.insert("second", 2);

  other = table;
  BOOST_CHECK(other.at("first") == 1);
}

BOOST_AUTO_TEST_CASE(movementEqualOperator)
{
  Table table(16);
  table.insert("first", 1);
  Table other(16);
  table.insert("second", 2);

  other = std::move(table);
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(other.at("first") == 1);
}

BOOST_AUTO_TEST_CASE(subscriptOperator)
{
  Table table(16);
  table.insert("first", 1);

  BOOST_CHECK(table["first"] == 1);
}

BOOST_AUTO_TEST_CASE(beginMethod)
{
  Table table(16);
  table.insert("first", 1);
  Iterator it = table.begin();

  BOOST_CHECK(*it == 1);
}

BOOST_AUTO_TEST_CASE(constBeginMethod)
{
  Table table(16);
  table.insert("first", 1);
  const Table other(table);
  ConstIterator it = other.begin();

  BOOST_CHECK(*it == 1);
}

BOOST_AUTO_TEST_CASE(endMethod)
{
  Table table(16);
  table.insert("first", 1);
  Iterator it = table.end();

  while (it != table.end())
  {
    BOOST_CHECK(*it == 1);
  }
}

BOOST_AUTO_TEST_CASE(constEndMethod)
{
  Table table(16);
  table.insert("first", 1);
  const Table other(table);
  ConstIterator it = other.end();
  
  while (it != other.end())
  {
    BOOST_CHECK(*it == 1);
  }
}

BOOST_AUTO_TEST_CASE(constAtMethod)
{
  Table table(16);
  table.insert("first", 1);
  const Table other(table);

  BOOST_CHECK(other.at("first") == 1);
}

BOOST_AUTO_TEST_CASE(containsMethod)
{
  Table table(16);
  table.insert("first", 1);

  BOOST_CHECK(table.contains("first"));
}

BOOST_AUTO_TEST_CASE(findMethod)
{
  Table table(16);
  table.insert("first", 1);
  size_t* value = table.find("first");

  BOOST_CHECK(*value == 1);
}

BOOST_AUTO_TEST_CASE(removeMethod)
{
  Table table(16);
  table.insert("first", 1);
  BOOST_CHECK(table.size() == 1);

  table.remove("first");
  BOOST_CHECK(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(rehashMethod)
{
  Table table(2);
  table.insert("first", 1);

  table.rehash(3);
  table.insert("second", 2);
}

BOOST_AUTO_TEST_CASE(capcityMethod)
{
  Table table(16);
  BOOST_CHECK(table.capacity() == 16);
  

  table.rehash(20);
  BOOST_CHECK(table.capacity() == 20);
}

BOOST_AUTO_TEST_CASE(emptyMethod)
{
  Table table(16);
  table.insert("first", 1);
  BOOST_CHECK(table.size() == 1);

  table.remove("first");
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(clearMethod)
{
  Table table(16);
  table.insert("first", 1);
  BOOST_CHECK(table.size() == 1);

  table.clear();
  BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_SUITE_END()
