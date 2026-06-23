#define BOOST_TEST_MODULE F0
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <stdexcept>
#include "model.hpp"
#include "map.hpp"
#include "unorderedMap.hpp"
#include "optimize.hpp"

BOOST_AUTO_TEST_SUITE(MapTests)

BOOST_AUTO_TEST_CASE(DefaulEmptyMap)
{
  kuznetsov::map< std::string, int > m;
  BOOST_CHECK(m.empty());
  BOOST_CHECK_EQUAL(m.size(), 0);
  BOOST_CHECK(!m.contains("x"));
}

BOOST_AUTO_TEST_CASE(InsertNew)
{
  kuznetsov::map< std::string, int > m;
  m.insert("b", 2);
  m.insert("a", 1);
  m.insert("c", 3);
  BOOST_CHECK_EQUAL(m.size(), 3);
  BOOST_CHECK(m.contains("a"));
  BOOST_CHECK_EQUAL(m.at("b"), 2);

  std::string keys;
  for (auto it = m.cbegin(); it != m.cend(); ++it) {
    keys += it->first;
  }
  BOOST_CHECK_EQUAL(keys, "abc");
}

BOOST_AUTO_TEST_CASE(UpdatesByInsert)
{
  kuznetsov::map< std::string, int > m;
  m.insert("a", 1);
  m.insert("a", 42);
  BOOST_CHECK_EQUAL(m.size(), 1);
  BOOST_CHECK_EQUAL(m.at("a"), 42);
}

BOOST_AUTO_TEST_CASE(Erase)
{
  kuznetsov::map< std::string, int > m;
  m.insert("a", 1);
  m.insert("b", 2);
  m.erase("a");
  BOOST_CHECK_EQUAL(m.size(), 1);
  BOOST_CHECK(!m.contains("a"));
  BOOST_CHECK(m.contains("b"));
}

BOOST_AUTO_TEST_CASE(BalancedTree)
{
  kuznetsov::map< int, int > m;
  const int n = 1023;
  for (int i = 0; i < n; ++i) {
    m.insert(i, i);
  }
  BOOST_CHECK_EQUAL(m.size(), static_cast< size_t >(n));
  BOOST_CHECK_LT(m.height(), 20);
  int prev = -1;
  bool ordered = true;
  for (auto it = m.cbegin(); it != m.cend(); ++it) {
    if (it->first <= prev) {
      ordered = false;
    }
    prev = it->first;
  }
  BOOST_CHECK(ordered);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(UnorderedMapTests)

using Hash = kuznetsov::SipHasher< std::string >;
using Eq = kuznetsov::detail::StrEqual;
using UMap = kuznetsov::unorderedMap< std::string, int, Hash, Eq >;

BOOST_AUTO_TEST_CASE(InsertAndErase)
{
  UMap m;
  BOOST_CHECK(m.empty());
  m.insert("a", 1);
  m.insert("b", 2);
  BOOST_CHECK_EQUAL(m.size(), 2);
  BOOST_CHECK(m.contains("a"));
  BOOST_CHECK_EQUAL(m.at("b"), 2);
  m.erase("a");
  BOOST_CHECK(!m.contains("a"));
  BOOST_CHECK_EQUAL(m.size(), 1);
}

BOOST_AUTO_TEST_CASE(UnMapUpdateByInsert)
{
  UMap m;
  m.insert("a", 1);
  m.insert("a", 7);
  BOOST_CHECK_EQUAL(m.size(), 1);
  BOOST_CHECK_EQUAL(m.at("a"), 7);
}

BOOST_AUTO_TEST_CASE(AutoRehash)
{
  UMap m;
  const int n = 200;
  for (int i = 0; i < n; ++i) {
    m.insert("k" + std::to_string(i), i);
  }
  BOOST_CHECK_EQUAL(m.size(), n);
  for (int i = 0; i < n; ++i) {
    BOOST_CHECK(m.contains("k" + std::to_string(i)));
    BOOST_CHECK_EQUAL(m.at("k" + std::to_string(i)), i);
  }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MapModelTests)

BOOST_AUTO_TEST_CASE(AddCityAndDublicate)
{
  kuznetsov::Map m;
  m.addCity("A");
  BOOST_CHECK(m.hasCity("A"));
  BOOST_CHECK_THROW(m.addCity("A"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(AddRoad)
{
  kuznetsov::Map m;
  m.addCity("A");
  m.addCity("B");
  BOOST_CHECK_THROW(m.addRoad("auto", "A", "B", 5), std::logic_error);
  m.addTransport("auto");
  m.setRoadType("auto", 1.0, 0.0, 1.0);
  BOOST_CHECK_THROW(m.addRoad("auto", "A", "Z", 5), std::logic_error);
  m.addRoad("auto", "A", "B", 5);
  BOOST_CHECK(m.hasRoad("auto", "A", "B"));
  BOOST_CHECK(m.hasRoad("auto", "B", "A"));
}

BOOST_AUTO_TEST_CASE(AddOrder)
{
  kuznetsov::Map m;
  m.addCity("A");
  m.addCity("B");
  BOOST_CHECK_THROW(m.addOrder(kuznetsov::Order{"o0", "A", "B", 0}), std::logic_error);
  BOOST_CHECK_THROW(m.addOrder(kuznetsov::Order{"o0", "A", "B", 11}), std::logic_error);
  BOOST_CHECK_THROW(m.addOrder(kuznetsov::Order{"o0", "A", "Z", 5}), std::logic_error);
  m.addOrder(kuznetsov::Order{"o1", "A", "B", 5});
  BOOST_CHECK(m.hasOrder("o1"));
  BOOST_CHECK_THROW(m.addOrder(kuznetsov::Order{"o1", "A", "B", 5}), std::logic_error);
}

BOOST_AUTO_TEST_CASE(RemoveCityAndRoadsConnectedWithIt)
{
  kuznetsov::Map m;
  m.addCity("A");
  m.addCity("B");
  m.addTransport("auto");
  m.setRoadType("auto", 1.0, 0.0, 1.0);
  m.addRoad("auto", "A", "B", 5);
  m.addOrder(kuznetsov::Order{"o1", "A", "B", 5});

  m.removeCity("B");
  BOOST_CHECK(!m.hasCity("B"));
  BOOST_CHECK(!m.hasRoad("auto", "A", "B"));
  BOOST_CHECK_EQUAL(m.orders().getSize(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

namespace kuznetsov {
  Map oneRoad()
  {
    Map m;
    m.addCity("A");
    m.addCity("B");
    m.addTransport("auto");
    m.setRoadType("auto", 1.0, 0.0, 1.0);
    m.addRoad("auto", "A", "B", 10);
    return m;
  }
}

BOOST_AUTO_TEST_SUITE(OptimizeTests)

BOOST_AUTO_TEST_CASE(SingleOrderRoad)
{
  kuznetsov::Map m = kuznetsov::oneRoad();
  m.addOrder(kuznetsov::Order{"o1", "A", "B", 4});
  kuznetsov::optimize(m, "A");
  BOOST_CHECK(m.hasRoute());
  BOOST_CHECK_CLOSE(m.routeCost(), 14.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(NoOrders)
{
  kuznetsov::Map m = kuznetsov::oneRoad();
  BOOST_CHECK_THROW(::kuznetsov::optimize(m, "A"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(UnknownBase)
{
  kuznetsov::Map m = kuznetsov::oneRoad();
  m.addOrder(::kuznetsov::Order{"o1", "A", "B", 4});
  BOOST_CHECK_THROW(::kuznetsov::optimize(m, "Z"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(Unreacheble)
{
  kuznetsov::Map m = kuznetsov::oneRoad();
  m.addCity("C");
  m.addOrder(::kuznetsov::Order{"o1", "A", "C", 4});
  BOOST_CHECK_THROW(optimize(m, "A"), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MapsControllerTests)

BOOST_AUTO_TEST_CASE(DefaultAndSwitch)
{
  kuznetsov::MapsController mc;
  BOOST_CHECK_EQUAL(mc.activeName(), "default");
  mc.newMap("eu");
  BOOST_CHECK_EQUAL(mc.activeName(), "eu");
  BOOST_CHECK_THROW(mc.newMap("eu"), std::logic_error);
  mc.switchMap("default");
  BOOST_CHECK_EQUAL(mc.activeName(), "default");
  BOOST_CHECK_THROW(mc.switchMap("none"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(RemoveMap)
{
  kuznetsov::MapsController mc;
  mc.newMap("eu");
  mc.removeMap("eu");
  BOOST_CHECK_EQUAL(mc.activeName(), "default");
  BOOST_CHECK_THROW(mc.removeMap("none"), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
