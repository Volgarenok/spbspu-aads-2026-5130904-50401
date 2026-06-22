#include <functional>
#include <stdexcept>
#include <string>
#include <boost/test/unit_test.hpp>
#include <hash_table.hpp>
#include "hasher.hpp"

BOOST_AUTO_TEST_SUITE(DefaultConstructorSuite)
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  zharov::HashTable< size_t, std::string, zharov::Blake2Hasher< size_t >, std::equal_to< size_t > >
    table;
  BOOST_CHECK_EQUAL(table.size(), 0);
  BOOST_CHECK_EQUAL(table.capacity(), 16);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CapacityConstructorSuite)
BOOST_AUTO_TEST_CASE(CapacityConstructorNormal)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table(10);
  BOOST_CHECK_EQUAL(table.capacity(), 16);
  BOOST_CHECK_EQUAL(table.size(), 0);
}
BOOST_AUTO_TEST_CASE(CapacityConstructorPowOfTwo)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table(32);
  BOOST_CHECK_EQUAL(table.capacity(), 32);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyConstructorSuite)
BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > original;
  original.add(1, "one");
  original.add(2, "two");
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > copy(
    original);
  BOOST_CHECK_EQUAL(copy.size(), 2);
  BOOST_CHECK_EQUAL(copy.capacity(), original.capacity());
  BOOST_CHECK_EQUAL(copy.at(1), "one");
  BOOST_CHECK_EQUAL(copy.at(2), "two");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveConstructorSuite)
BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > original;
  original.add(1, "one");
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > moved(
    std::move(original));
  BOOST_CHECK_EQUAL(moved.size(), 1);
  BOOST_CHECK_EQUAL(moved.at(1), "one");
  BOOST_CHECK_EQUAL(original.size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyOperatorSuite)
BOOST_AUTO_TEST_CASE(CopyOperatorSelfAssign)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "one");
  table = table;
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), "one");
}
BOOST_AUTO_TEST_CASE(CopyOperatorDifferentTables)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > a, b;
  a.add(10, "ten");
  b.add(20, "twenty");
  b = a;
  BOOST_CHECK_EQUAL(b.size(), 1);
  BOOST_CHECK_EQUAL(b.at(10), "ten");
  BOOST_CHECK_THROW(b.at(20), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveOperatorSuite)
BOOST_AUTO_TEST_CASE(MoveOperator)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > a, b;
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
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "value");
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(1), "value");
}
BOOST_AUTO_TEST_CASE(AddDuplicateThrows)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(2, "first");
  BOOST_CHECK_THROW(table.add(2, "second"), std::logic_error);
}
BOOST_AUTO_TEST_CASE(AddWhenFullThrows)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table(2);
  table.add(1, "one");
  table.add(2, "two");
  BOOST_CHECK_THROW(table.add(3, "three"), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RemoveSuite)
BOOST_AUTO_TEST_CASE(RemoveExisting)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(10, "ten");
  table.add(20, "twenty");
  table.remove(10);
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK(!table.contains(10));
  BOOST_CHECK(table.contains(20));
}
BOOST_AUTO_TEST_CASE(RemoveNonExistentThrows)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(5, "five");
  BOOST_CHECK_THROW(table.remove(99), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ContainsSuite)
BOOST_AUTO_TEST_CASE(ContainsReturnsTrueForExistingKey)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(7, "seven");
  BOOST_CHECK(table.contains(7));
}
BOOST_AUTO_TEST_CASE(ContainsReturnsFalseForMissingKey)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(8, "eight");
  BOOST_CHECK(!table.contains(9));
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RehashSuite)
BOOST_AUTO_TEST_CASE(RehashLargerCapacity)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table(4);
  table.add(1, "a");
  table.add(2, "b");
  size_t oldCap = table.capacity();
  table.rehash(10);
  BOOST_CHECK_GT(table.capacity(), oldCap);
  BOOST_CHECK_EQUAL(table.size(), 2);
  BOOST_CHECK_EQUAL(table.at(1), "a");
  BOOST_CHECK_EQUAL(table.at(2), "b");
}
BOOST_AUTO_TEST_CASE(RehashTooSmallThrows)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "one");
  table.add(2, "two");
  BOOST_CHECK_THROW(table.rehash(1), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SwapSuite)
BOOST_AUTO_TEST_CASE(SwapTables)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > a, b;
  a.add(1, "first");
  b.add(2, "second");
  a.swap(b);
  BOOST_CHECK_EQUAL(a.size(), 1);
  BOOST_CHECK_EQUAL(a.at(2), "second");
  BOOST_CHECK_EQUAL(b.size(), 1);
  BOOST_CHECK_EQUAL(b.at(1), "first");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GetSizeSuite)
BOOST_AUTO_TEST_CASE(GetSize)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  BOOST_CHECK_EQUAL(table.size(), 0);
  table.add(1, "x");
  BOOST_CHECK_EQUAL(table.size(), 1);
  table.remove(1);
  BOOST_CHECK_EQUAL(table.size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GetCapacitySuite)
BOOST_AUTO_TEST_CASE(GetCapacity)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table(8);
  BOOST_CHECK_EQUAL(table.capacity(), 8);
  table.rehash(20);
  BOOST_CHECK_EQUAL(table.capacity(), 32);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AtSuite)
BOOST_AUTO_TEST_CASE(AtNonConstModifies)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(100, "initial");
  table.at(100) = "modified";
  BOOST_CHECK_EQUAL(table.at(100), "modified");
}
BOOST_AUTO_TEST_CASE(AtConstReturnsValue)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(200, "constant");
  const zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > >&
    constTable = table;
  BOOST_CHECK_EQUAL(constTable.at(200), "constant");
}
BOOST_AUTO_TEST_CASE(AtThrowsWhenNotFound)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  BOOST_CHECK_THROW(table.at(999), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IteratorSuite)
BOOST_AUTO_TEST_CASE(BeginEndNonEmpty)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");
  std::vector< std::string > values;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    values.push_back(it->second);
  }
  std::sort(values.begin(), values.end());
  std::vector< std::string > expected = {"one", "three", "two"};
  BOOST_CHECK_EQUAL_COLLECTIONS(values.begin(), values.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(EmptyTableBeginEqualsEnd)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  BOOST_CHECK(table.begin() == table.end());
  BOOST_CHECK(table.cbegin() == table.cend());
}

BOOST_AUTO_TEST_CASE(DereferenceOperator)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "one");
  auto it = table.begin();
  BOOST_CHECK_EQUAL((*it).first, 1);
  BOOST_CHECK_EQUAL(it->second, "one");
}

BOOST_AUTO_TEST_CASE(PreIncrement)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "a");
  table.add(2, "b");
  auto it = table.begin();
  auto prev = it;
  ++it;
  BOOST_CHECK(prev != it);
  BOOST_CHECK(prev->first != it->first);
  ++it;
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(PostIncrement)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(10, "x");
  table.add(20, "y");
  auto it = table.begin();
  auto old = it++;
  BOOST_CHECK(old != it);
  BOOST_CHECK(old->first != it->first);
}

BOOST_AUTO_TEST_CASE(PreDecrement)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(5, "five");
  table.add(6, "six");
  auto it = table.begin();
  ++it;
  --it;
  BOOST_CHECK(it == table.begin());
}

BOOST_AUTO_TEST_CASE(PostDecrement)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(7, "seven");
  table.add(8, "eight");
  auto it = table.begin();
  ++it;
  auto old = it--;
  BOOST_CHECK(old != it);
  BOOST_CHECK(it == table.begin());
}

BOOST_AUTO_TEST_CASE(EqualityComparators)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(100, "hundred");
  auto it1 = table.begin();
  auto it2 = table.begin();
  auto it3 = table.end();
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
}

BOOST_AUTO_TEST_CASE(ConstIteratorNonModifiable)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(99, "const");
  const zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > >&
    constTable = table;
  auto cit = constTable.cbegin();
  BOOST_CHECK_EQUAL(cit->second, "const");
}

BOOST_AUTO_TEST_CASE(RangeForLoop)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "first");
  table.add(2, "second");
  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, table.size());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(OperatorBracketSuite)
BOOST_AUTO_TEST_CASE(OperatorBracketExistingKey)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "one");
  BOOST_CHECK_EQUAL(table[1], "one");
}
BOOST_AUTO_TEST_CASE(OperatorBracketInsertsDefault)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table[42] = "inserted";
  BOOST_CHECK_EQUAL(table.size(), 1);
  BOOST_CHECK_EQUAL(table.at(42), "inserted");
}
BOOST_AUTO_TEST_CASE(OperatorBracketModifies)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(5, "old");
  table[5] = "new";
  BOOST_CHECK_EQUAL(table.at(5), "new");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(FindSuite)
BOOST_AUTO_TEST_CASE(FindExistingKey)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(10, "ten");
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > >::iterator
    it = table.find(10);
  BOOST_CHECK(it != table.end());
  BOOST_CHECK_EQUAL(it->second, "ten");
}
BOOST_AUTO_TEST_CASE(FindMissingKey)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(1, "one");
  BOOST_CHECK(table.find(99) == table.end());
}
BOOST_AUTO_TEST_CASE(FindConstExistingKey)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  table.add(7, "seven");
  const zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > >& ct =
    table;
  zharov::HashTable< size_t, std::string, std::hash< size_t >,
    std::equal_to< size_t > >::const_iterator cit = ct.find(7);
  BOOST_CHECK(cit != ct.end());
  BOOST_CHECK_EQUAL(cit->second, "seven");
}
BOOST_AUTO_TEST_CASE(FindConstMissingKey)
{
  zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > > table;
  const zharov::HashTable< size_t, std::string, std::hash< size_t >, std::equal_to< size_t > >& ct =
    table;
  BOOST_CHECK(ct.find(0) == ct.end());
}
BOOST_AUTO_TEST_SUITE_END()
