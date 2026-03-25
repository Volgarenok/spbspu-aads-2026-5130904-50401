#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <string>

#include "Queue.hpp"

BOOST_AUTO_TEST_SUITE(queue_test_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_empty_creation)
{
  Queue< int > q;

  BOOST_REQUIRE(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_push)
{
  Queue< int > q;

  q.push(1);
  q.push(2);

  BOOST_REQUIRE(q.empty() == false);
  BOOST_CHECK_EQUAL(q.size(), 2);
  BOOST_CHECK_EQUAL(q.front(), 2);
  BOOST_CHECK_EQUAL(q.back(), 1);
}

BOOST_AUTO_TEST_CASE(test_pop)
{
  Queue< int > q;

  q.push(1);
  q.push(2);

  q.pop();

  BOOST_REQUIRE(q.empty() == false);
  BOOST_CHECK_EQUAL(q.front(), 1);
  BOOST_CHECK_EQUAL(q.front(), q.back());

  q.pop();

  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_empty_pop)
{
  Queue< int > q;

  BOOST_CHECK_THROW(q.pop(), std::runtime_error);
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
  Queue< Person > q;
  q.push(Person{1, "Oleg"});

  q.emplace(2, "Kristina");
  q.emplace(3);
  q.emplace("Andrey");

  BOOST_REQUIRE(q.empty() == false);
  BOOST_CHECK_EQUAL(q.size(), 4);
  Person frst{1, "Oleg"};
  Person last{0, "Andrey"};
  BOOST_CHECK_EQUAL(q.back(), frst);
  BOOST_CHECK_EQUAL(q.front(), last);
  BOOST_CHECK_EQUAL(q.front(), q.back());
}

BOOST_AUTO_TEST_CASE(test_push_rvalue)
{
  Queue< Person > q;
  Person test_person{1, "Oleg"};
  q.push(std::move(test_person));

  BOOST_REQUIRE(q.empty() == false);
  BOOST_CHECK(q.front() == (Person{1, "Oleg"}));
  BOOST_CHECK(test_person.name.empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()
