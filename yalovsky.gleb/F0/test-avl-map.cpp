#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include "avl-map.hpp"

BOOST_AUTO_TEST_CASE(avl_insertion_and_lookup)
{
  yalovsky::AvlMap< int, std::string > values;

  BOOST_TEST(values.empty());
  BOOST_TEST(values.insert(2, "two"));
  BOOST_TEST(values.insert(1, "one"));
  BOOST_TEST(values.insert(3, "three"));
  BOOST_TEST(!values.insert(2, "duplicate"));
  BOOST_TEST(values.size() == 3);
  BOOST_TEST(values.at(2) == "two");
  BOOST_CHECK_THROW(values.at(9), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(avl_order_and_balancing)
{
  yalovsky::AvlMap< int, int > values;
  const int count = 1000;

  for (int key = 0; key < count; ++key)
  {
    values.insert(key, key * key);
  }

  int expected = 0;
  values.forEach([&expected](const int& key, const int& value)
  {
    BOOST_TEST(key == expected);
    BOOST_TEST(value == expected * expected);
    ++expected;
  });

  BOOST_TEST(expected == count);

  for (int key = 0; key < count; key += 2)
  {
    BOOST_TEST(values.erase(key));
  }

  BOOST_TEST(values.size() == 500);

  for (int key = 0; key < count; ++key)
  {
    BOOST_TEST(values.contains(key) == (key % 2 != 0));
  }

  for (int key = 1; key < count; key += 2)
  {
    BOOST_TEST(values.erase(key));
  }
  BOOST_TEST(values.empty());
}

BOOST_AUTO_TEST_CASE(avl_copy_and_move)
{
  yalovsky::AvlMap< int, std::string > original;
  original.insert(1, "one");
  original.insert(2, "two");

  auto copy = original;
  copy.at(1) = "changed";
  copy.erase(2);

  BOOST_TEST(original.at(1) == "one");
  BOOST_TEST(original.size() == 2);

  auto moved = std::move(copy);
  BOOST_TEST(copy.empty());
  BOOST_TEST(moved.at(1) == "changed");

  copy = original;
  BOOST_TEST(copy.size() == 2);

  copy = std::move(moved);
  BOOST_TEST(copy.size() == 1);
  BOOST_TEST(moved.empty());
}
