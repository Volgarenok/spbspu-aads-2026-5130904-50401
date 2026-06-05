#include "Hash-table.hpp"
#include "hash-structs.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(hashTable_tests)

using testTable = donkeev::HashTable< std::string, size_t, donkeev::TestStructHash, donkeev::TestStructEqual >;

BOOST_AUTO_TEST_CASE(sizesConstructor)
{
  testTable table(16, 4);
  BOOST_CHECK(table.size() == 0);

  table.add("first", 1);
  BOOST_CHECK(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  testTable table(16, 4);
  table.add("first", 1);
  BOOST_CHECK(table.size() == 1);

  testTable copyTable(table);
  BOOST_CHECK(copyTable.size() == 1);
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  testTable table(16, 4);
  table.add("first", 1);
  BOOST_CHECK(table.size() == 1);

  testTable copyTable(std::move(table));
  BOOST_CHECK(copyTable.size() == 1);
  BOOST_CHECK(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(copyOperator)
{
  testTable table(16, 4);
  table.add("first", 1);
  BOOST_CHECK(table.size() == 1);

  testTable copyTable(16, 4);
  BOOST_CHECK(copyTable.size() == 0);
  copyTable = table;
  BOOST_CHECK(copyTable.size() == 1);
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  testTable table(16, 4);
  table.add("first", 1);
  BOOST_CHECK(table.size() == 1);

  testTable copyTable(16, 4);
  BOOST_CHECK(copyTable.size() == 0);
  copyTable = std::move(table);
  BOOST_CHECK(copyTable.size() == 1);
  BOOST_CHECK(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(beginMethod)
{
  testTable table(16, 4);
  table.add("first", 1);

  testTable::iterator it = table.begin();
  BOOST_CHECK((*it).first == "first" && (*it).second == 1);
}

BOOST_AUTO_TEST_CASE(constBeginMethod)
{
  testTable table(16, 4);
  table.add("first", 1);
  const testTable copyTable(table);

  testTable::constIterator it = copyTable.begin();
  BOOST_CHECK((*it).first == "first" && (*it).second == 1);
}

BOOST_AUTO_TEST_CASE(endMethod)
{
  testTable table(16, 4);
  table.add("first", 1);

  testTable::iterator it = table.end();
  --it;
  BOOST_CHECK((*it).first == "first" && (*it).second == 1);
}

BOOST_AUTO_TEST_CASE(constEndMethod)
{
  testTable table(16, 4);
  table.add("first", 1);
  const testTable copyTable(table);

  testTable::constIterator it = copyTable.end();
  --it;
  BOOST_CHECK((*it).first == "first" && (*it).second == 1);
}

BOOST_AUTO_TEST_CASE(sizeMethod)
{
  testTable table(16, 4);
  table.add("first", 1);
  BOOST_CHECK(table.size() == 1);

  table.add("first", 1);
  BOOST_CHECK(table.size() == 2);
}

BOOST_AUTO_TEST_CASE(movementAddMethod)
{
  testTable table(16, 4);
  size_t value = 1;
  table.add("first", std::move(value));
  BOOST_CHECK(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(getValueMethod)
{
  testTable table(16, 4);
  table.add("first", 1);
  size_t value = table.get("first");
  BOOST_CHECK(value == 1);
}

BOOST_AUTO_TEST_CASE(findValueMethod)
{
  testTable table(16, 4);
  table.add("first", 1);
  size_t* value_ptr = table.find("first");
  BOOST_CHECK(*value_ptr == 1);
}

BOOST_AUTO_TEST_CASE(dropValueMethod)
{
  testTable table(16, 4);
  table.add("first", 1);
  size_t value = table.drop("first");
  BOOST_CHECK(value == 1);
  BOOST_CHECK(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(hasValueMethod)
{
  testTable table(16, 4);
  table.add("first", 1);
  BOOST_CHECK(table.has("first"));
}

BOOST_AUTO_TEST_CASE(rehashMethod)
{
  testTable table(1, 1);
  table.add("first", 1);
  table.add("second", 2);

  table.rehash(3);
  table.add("third", 3);
}
BOOST_AUTO_TEST_SUITE_END()
