#include <boost/test/unit_test.hpp>
#include "mystack.hpp"

BOOST_AUTO_TEST_SUITE(Stack_suite)

BOOST_AUTO_TEST_CASE(designer_test)
{
  goltsov::Stack< int > a;
  BOOST_CHECK(a.empty());
  goltsov::Stack< int > a01(a);
  BOOST_CHECK(a01.empty());
  goltsov::Stack< int > a02(std::move(a));
  BOOST_CHECK(a02.empty());
  goltsov::Stack< int > a1 = a;
  BOOST_CHECK(a1.empty());
  goltsov::Stack< int > a2 = std::move(a);
  BOOST_CHECK(a2.empty());

  const goltsov::Stack< int > ac;
  BOOST_CHECK(ac.empty());
  const goltsov::Stack< int > a01c(ac);
  BOOST_CHECK(a01c.empty());
  const goltsov::Stack< int > a02c(std::move(ac));
  BOOST_CHECK(a02c.empty());
}

BOOST_AUTO_TEST_CASE(push_test)
{
  goltsov::Stack< int > a;
  int temp = 4;
  a.push(5);
  a.push(temp);
  BOOST_CHECK(a.size() == 2 && a.top() == 4);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  goltsov::Stack< int > a;
  BOOST_CHECK_THROW(a.pop(), std::runtime_error);
  a.push(5);
  a.push(4);
  a.pop();
  BOOST_CHECK(a.size() == 1 && a.top() == 5);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  goltsov::Stack< int > a;
  BOOST_CHECK_THROW(a.top(), std::runtime_error);
  a.push(5);
  a.push(4);
  BOOST_CHECK(a.top() == 4);
  const goltsov::Stack< int > ac = a;
  BOOST_CHECK(ac.top() == 4);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  goltsov::Stack< int > a;
  goltsov::Stack< int > b;
  b.push(1);
  BOOST_CHECK(a.empty() && !b.empty());
  const goltsov::Stack< int > ac = a;
  const goltsov::Stack< int > bc = b;
  BOOST_CHECK(ac.empty() && !bc.empty());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  goltsov::Stack< int > a;
  a.push(5);
  a.push(4);
  goltsov::Stack< int > b;
  BOOST_CHECK(a.size() == 2 && b.size() == 0);
  const goltsov::Stack< int > ac = a;
  const goltsov::Stack< int > bc = b;
  BOOST_CHECK(ac.size() == 2 && bc.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  goltsov::Stack< int > a;
  a.push(5);
  a.push(4);
  a.clear();
  BOOST_CHECK_THROW(a.pop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(emplace_test)
{
  goltsov::Stack< std::pair< int, std::string > > s;
  s.emplace(1, "one");
  BOOST_CHECK(s.size() == 1);
  BOOST_CHECK(s.top().first == 1);
  BOOST_CHECK(s.top().second == "one");
  s.emplace(2, "two");
  BOOST_CHECK(s.size() == 2);
  BOOST_CHECK(s.top().first == 2);
  BOOST_CHECK(s.top().second == "two");
  goltsov::Stack< std::tuple< int, double, char > > t;
  t.emplace(42, 3.14, 'a');
  BOOST_CHECK(t.size() == 1);
  BOOST_CHECK(std::get< 0 >(t.top()) == 42);
  BOOST_CHECK(std::get< 1 >(t.top()) == 3.14);
  BOOST_CHECK(std::get< 2 >(t.top()) == 'a');
}

BOOST_AUTO_TEST_CASE(emplace_with_complex_object_test)
{
  struct NonCopyable {
    int value;
    std::unique_ptr< int > ptr;
    NonCopyable():
      value(0),
      ptr(nullptr)
    {}
    NonCopyable(int x, int y):
      value(x),
      ptr(std::make_unique< int >(y))
    {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = default;
  };
  goltsov::Stack< NonCopyable > s;
  BOOST_CHECK_NO_THROW(s.emplace(10, 20));
  BOOST_CHECK(s.size() == 1);
  BOOST_CHECK(s.top().value == 10);
  BOOST_CHECK(*s.top().ptr == 20);
  BOOST_CHECK_NO_THROW(s.emplace(30, 40));
  BOOST_CHECK(s.size() == 2);
  BOOST_CHECK(s.top().value == 30);
  BOOST_CHECK(*s.top().ptr == 40);
}

BOOST_AUTO_TEST_CASE(emplace_with_three_arguments_test)
{
  struct ThreeArgs {
    int a;
    double b;
    std::string c;
    ThreeArgs():
      a(0),
      b(0.0),
      c("")
    {}
    ThreeArgs(int x, double y, std::string z):
      a(x),
      b(y),
      c(z)
    {}
  };
  goltsov::Stack< ThreeArgs > s;
  s.emplace(42, 3.14, "hello");
  BOOST_CHECK(s.size() == 1);
  BOOST_CHECK(s.top().a == 42);
  BOOST_CHECK(s.top().b == 3.14);
  BOOST_CHECK(s.top().c == "hello");
}

BOOST_AUTO_TEST_SUITE_END()
