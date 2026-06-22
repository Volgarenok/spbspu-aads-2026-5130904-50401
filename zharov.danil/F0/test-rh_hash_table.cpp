#include <stdexcept>
#include <string>
#include <boost/test/unit_test.hpp>
#include <rh_hash_table.hpp>

using Table =
  zharov::RHHashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > >;

BOOST_AUTO_TEST_SUITE(DefaultConstructorSuite)
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  Table table;
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_EQUAL(table.capacity(), 16);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CapacityConstructorSuite)
BOOST_AUTO_TEST_CASE(CapacityConstructor)
{
  Table table(32);
  BOOST_CHECK_EQUAL(table.capacity(), 32);
  BOOST_CHECK_EQUAL(table.size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyConstructorSuite)
BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  Table original;
  original.add(1, "one");
  original.add(2, "two");
  Table copy(original);
  BOOST_CHECK_EQUAL(copy.size(), 2);
  BOOST_CHECK_EQUAL(copy.at(1), "one");
  BOOST_CHECK_EQUAL(copy.at(2), "two");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveConstructorSuite)
BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  Table original;
  original.add(1, "one");
  Table moved(std::move(original));
  BOOST_CHECK_EQUAL(moved.size(), 1);
  BOOST_CHECK_EQUAL(moved.at(1), "one");
  BOOST_CHECK_EQUAL(original.size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyOperatorSuite)
BOOST_AUTO_TEST_CASE(CopyOperatorSelfAssign)
{
  Table table;
  table.add(1, "one");
  table = table;
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), "one");
}
BOOST_AUTO_TEST_CASE(CopyOperatorDifferentTables)
{
  Table a, b;
  a.add(10, "ten");
  b.add(20, "twenty");
  b = a;
  BOOST_CHECK_EQUAL(b.size(), 1);
  BOOST_CHECK_EQUAL(b.at(10), "ten");
  BOOST_CHECK_THROW(b.at(20), std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveOperatorSuite)
BOOST_AUTO_TEST_CASE(MoveOperator)
{
  Table a, b;
  a.add(7, "seven");
  b = std::move(a);
  BOOST_CHECK_EQUAL(b.size(), 1);
  BOOST_CHECK_EQUAL(b.at(7), "seven");
  BOOST_CHECK_EQUAL(a.size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AddSuite)
BOOST_AUTO_TEST_CASE(AddSingleElement)
{
  Table table;
  table.add(1, "value");
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), "value");
}
BOOST_AUTO_TEST_CASE(AddDuplicateThrows)
{
  Table table;
  table.add(2, "first");
  BOOST_CHECK_THROW(table.add(2, "second"), std::invalid_argument);
}
BOOST_AUTO_TEST_CASE(AddTriggersRehash)
{
  Table table(4);
  table.add(1, "a");
  table.add(2, "b");
  table.add(3, "c");
  BOOST_CHECK_EQUAL(table.size(), 3);
  BOOST_CHECK_EQUAL(table.at(1), "a");
  BOOST_CHECK_EQUAL(table.at(2), "b");
  BOOST_CHECK_EQUAL(table.at(3), "c");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RemoveSuite)
BOOST_AUTO_TEST_CASE(RemoveExisting)
{
  Table table;
  table.add(10, "ten");
  table.add(20, "twenty");
  table.remove(10);
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(!table.has(10));
  BOOST_CHECK(table.has(20));
}
BOOST_AUTO_TEST_CASE(RemoveNonExistentThrows)
{
  Table table;
  table.add(5, "five");
  BOOST_CHECK_THROW(table.remove(99), std::out_of_range);
}
BOOST_AUTO_TEST_CASE(RemoveAndReinsert)
{
  Table table;
  table.add(1, "one");
  table.remove(1);
  table.add(1, "new");
  BOOST_CHECK_EQUAL(table.at(1), "new");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HasSuite)
BOOST_AUTO_TEST_CASE(HasExistingKey)
{
  Table table;
  table.add(7, "seven");
  BOOST_CHECK(table.has(7));
}
BOOST_AUTO_TEST_CASE(HasMissingKey)
{
  Table table;
  table.add(8, "eight");
  BOOST_CHECK(!table.has(9));
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RehashSuite)
BOOST_AUTO_TEST_CASE(RehashLargerCapacity)
{
  Table table(4);
  table.add(1, "a");
  table.add(2, "b");
  size_t oldCap = table.capacity();
  table.rehash(32);
  BOOST_CHECK_GT(table.capacity(), oldCap);
  BOOST_CHECK_EQUAL(table.size(), 2);
  BOOST_CHECK_EQUAL(table.at(1), "a");
  BOOST_CHECK_EQUAL(table.at(2), "b");
}
BOOST_AUTO_TEST_CASE(RehashTooSmallThrows)
{
  Table table;
  table.add(1, "one");
  table.add(2, "two");
  BOOST_CHECK_THROW(table.rehash(1), std::invalid_argument);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SwapSuite)
BOOST_AUTO_TEST_CASE(SwapTables)
{
  Table a, b;
  a.add(1, "first");
  b.add(2, "second");
  a.swap(b);
  BOOST_CHECK_EQUAL(a.at(2), "second");
  BOOST_CHECK_EQUAL(b.at(1), "first");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AtSuite)
BOOST_AUTO_TEST_CASE(AtNonConstModifies)
{
  Table table;
  table.add(100, "initial");
  table.at(100) = "modified";
  BOOST_CHECK_EQUAL(table.at(100), "modified");
}
BOOST_AUTO_TEST_CASE(AtConstReturnsValue)
{
  Table table;
  table.add(200, "constant");
  const Table& constTable = table;
  BOOST_CHECK_EQUAL(constTable.at(200), "constant");
}
BOOST_AUTO_TEST_CASE(AtThrowsWhenNotFound)
{
  Table table;
  BOOST_CHECK_THROW(table.at(999), std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IteratorSuite)
BOOST_AUTO_TEST_CASE(BeginEndNonEmpty)
{
  Table table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 3);
}
BOOST_AUTO_TEST_CASE(EmptyTableBeginEqualsEnd)
{
  Table table;
  BOOST_CHECK(table.begin() == table.end());
  BOOST_CHECK(table.cbegin() == table.cend());
}
BOOST_AUTO_TEST_CASE(DereferenceOperator)
{
  Table table;
  table.add(1, "one");
  auto it = table.begin();
  BOOST_CHECK_EQUAL((*it).first, 1);
  BOOST_CHECK_EQUAL(it->second, "one");
}
BOOST_AUTO_TEST_CASE(PreIncrement)
{
  Table table;
  table.add(1, "a");
  table.add(2, "b");
  auto it = table.begin();
  auto prev = it;
  ++it;
  BOOST_CHECK(prev != it);
  ++it;
  BOOST_CHECK(it == table.end());
}
BOOST_AUTO_TEST_CASE(PostIncrement)
{
  Table table;
  table.add(10, "x");
  table.add(20, "y");
  auto it = table.begin();
  auto old = it++;
  BOOST_CHECK(old != it);
}
BOOST_AUTO_TEST_CASE(EqualityComparators)
{
  Table table;
  table.add(100, "hundred");
  auto it1 = table.begin();
  auto it2 = table.begin();
  auto it3 = table.end();
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
}
BOOST_AUTO_TEST_CASE(ConstIterator)
{
  Table table;
  table.add(99, "const");
  const Table& constTable = table;
  auto cit = constTable.cbegin();
  BOOST_CHECK_EQUAL(cit->second, "const");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RobinHoodSuite)
BOOST_AUTO_TEST_CASE(ManyCollisions)
{
  Table table(8);
  for (size_t i = 0; i < 5; ++i)
  {
    table.add(i * 8, std::to_string(i));
  }
  for (size_t i = 0; i < 5; ++i)
  {
    BOOST_CHECK_EQUAL(table.at(i * 8), std::to_string(i));
  }
}
BOOST_AUTO_TEST_CASE(RemoveWithBackwardShift)
{
  Table table(8);
  table.add(0, "a");
  table.add(8, "b");
  table.add(16, "c");
  table.remove(0);
  BOOST_CHECK(!table.has(0));
  BOOST_CHECK(table.has(8));
  BOOST_CHECK(table.has(16));
}
BOOST_AUTO_TEST_SUITE_END()
