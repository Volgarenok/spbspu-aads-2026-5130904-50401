#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "Stack.hpp"

BOOST_AUTO_TEST_SUITE(stack_test_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_empty_creation)
{
  Stack< int > s;

  BOOST_REQUIRE(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push)
{
  Stack< int > s;

  s.push(1);
  s.push(2);

  BOOST_REQUIRE(s.empty() == false);
  BOOST_CHECK_EQUAL(s.size(), 2);
  BOOST_CHECK_EQUAL(s.top(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Stack< int > s;

  s.push(1);
  s.push(2);

  s.pop();

  BOOST_REQUIRE(s.empty() == false);
  BOOST_CHECK_EQUAL(s.top(), 2);

  s.pop();

  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_empty_pop)
{
  Stack< int > s;

  BOOST_CHECK_THROW(s.pop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_empty_top)
{
  Stack< int > s;

  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}

struct Person
{
  size_t age;
  std::string name;
  Person(size_t a):
      age(a),
      name("unknown")
  {
  }
  Person(const std::string &n):
      age(0),
      name(n)
  {
  }
  Person(size_t a, const std::string &n):
      age(a),
      name(n)
  {
  }
  bool operator==(const Person &rhs) const
  {
    return age == rhs.age && name == rhs.name;
  }
  bool operator!=(const Person &rhs) const
  {
    return !(*this == rhs);
  }
};

std::ostream &operator<<(std::ostream &out, const Person &rhs)
{
  return out << rhs.age << ", " << rhs.name << "; ";
}

BOOST_AUTO_TEST_CASE(test_emplacing_element)
{
  Stack< Person > s;
  s.push(Person{1, "Oleg"});

  s.emplace(2, "Kristina");
  s.emplace(3);
  s.emplace("Andrey");

  BOOST_REQUIRE(s.empty() == false);
  BOOST_CHECK_EQUAL(s.size(), 4);
  Person top{0, "Andrey"};
  BOOST_CHECK_EQUAL(s.top(), top);
}

BOOST_AUTO_TEST_SUITE_END()
