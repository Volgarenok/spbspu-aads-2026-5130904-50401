#include "top_it_vector.hpp"
#include <boost/test/unit_test.hpp>

#include <string>

BOOST_AUTO_TEST_CASE(test_vector_default_constructor)
{
  ulanova::Vector<int> v;

  BOOST_TEST_CHECK(v.isEmpty());
  BOOST_TEST_CHECK(v.getsize() == 0);
  BOOST_TEST_CHECK(v.getcapacity() == 0);
}

BOOST_AUTO_TEST_CASE(test_vector_push_back)
{
  ulanova::Vector<int> v;

  v.push_back(10);
  v.push_back(20);
  v.push_back(30);

  BOOST_TEST_CHECK(!v.isEmpty());
  BOOST_TEST_CHECK(v.getsize() == 3);
  BOOST_TEST_CHECK(v[0] == 10);
  BOOST_TEST_CHECK(v[1] == 20);
  BOOST_TEST_CHECK(v[2] == 30);
}

BOOST_AUTO_TEST_CASE(test_vector_insert_and_erase)
{
  ulanova::Vector<int> v;

  v.push_back(1);
  v.push_back(3);
  v.insert(1, 2);

  BOOST_TEST_CHECK(v.getsize() == 3);
  BOOST_TEST_CHECK(v[0] == 1);
  BOOST_TEST_CHECK(v[1] == 2);
  BOOST_TEST_CHECK(v[2] == 3);

  v.erase(1);

  BOOST_TEST_CHECK(v.getsize() == 2);
  BOOST_TEST_CHECK(v[0] == 1);
  BOOST_TEST_CHECK(v[1] == 3);
}

BOOST_AUTO_TEST_CASE(test_vector_copy_constructor)
{
  ulanova::Vector<std::string> v;

  v.push_back("a");
  v.push_back("b");

  ulanova::Vector<std::string> copy(v);

  BOOST_TEST_CHECK(copy.getsize() == 2);
  BOOST_TEST_CHECK(copy[0] == "a");
  BOOST_TEST_CHECK(copy[1] == "b");

  copy[0] = "x";

  BOOST_TEST_CHECK(v[0] == "a");
  BOOST_TEST_CHECK(copy[0] == "x");
}

BOOST_AUTO_TEST_CASE(test_vector_push_back_count)
{
  ulanova::Vector<int> v;

  v.pushBackCount(3, 7);

  BOOST_TEST_CHECK(v.getsize() == 3);
  BOOST_TEST_CHECK(v[0] == 7);
  BOOST_TEST_CHECK(v[1] == 7);
  BOOST_TEST_CHECK(v[2] == 7);
}

BOOST_AUTO_TEST_CASE(test_vector_push_back_range)
{
  int values[] = {1, 2, 3};

  ulanova::Vector<int> v;
  v.pushBackRange(values, 3);

  BOOST_TEST_CHECK(v.getsize() == 3);
  BOOST_TEST_CHECK(v[0] == 1);
  BOOST_TEST_CHECK(v[1] == 2);
  BOOST_TEST_CHECK(v[2] == 3);
}
