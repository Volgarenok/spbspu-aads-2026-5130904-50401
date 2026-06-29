#include <boost/test/unit_test.hpp>
#include "myqueue.hpp"

BOOST_AUTO_TEST_SUITE(Queue_suite)

BOOST_AUTO_TEST_CASE(designer_test)
{
  goltsov::Queue< int > a;
  BOOST_CHECK(a.empty());
  goltsov::Queue< int > a01(a);
  BOOST_CHECK(a01.empty());
  goltsov::Queue< int > a02(std::move(a));
  BOOST_CHECK(a02.empty());
  goltsov::Queue< int > a1 = a;
  BOOST_CHECK(a1.empty());
  goltsov::Queue< int > a2 = std::move(a);
  BOOST_CHECK(a2.empty());

  const goltsov::Queue< int > ac;
  BOOST_CHECK(ac.empty());
  const goltsov::Queue< int > a01c(ac);
  BOOST_CHECK(a01c.empty());
  const goltsov::Queue< int > a02c(std::move(ac));
  BOOST_CHECK(a02c.empty());
}

BOOST_AUTO_TEST_CASE(push_test)
{
  goltsov::Queue< int > a;
  int temp = 4;
  a.push(5);
  a.push(temp);
  BOOST_CHECK(a.size() == 2 && a.front() == 5);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  goltsov::Queue< int > a;
  BOOST_CHECK_THROW(a.pop(), std::runtime_error);
  a.push(5);
  a.push(4);
  a.pop();
  BOOST_CHECK(a.size() == 1 && a.front() == 4);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  goltsov::Queue< int > a;
  BOOST_CHECK_THROW(a.front(), std::runtime_error);
  a.push(5);
  a.push(4);
  BOOST_CHECK(a.front() == 5);
  const goltsov::Queue< int > ac = a;
  BOOST_CHECK(ac.front() == 5);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  goltsov::Queue< int > a;
  goltsov::Queue< int > b;
  b.push(1);
  BOOST_CHECK(a.empty() && !b.empty());
  const goltsov::Queue< int > ac = a;
  const goltsov::Queue< int > bc = b;
  BOOST_CHECK(ac.empty() && !bc.empty());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  goltsov::Queue< int > a;
  a.push(5);
  a.push(4);
  goltsov::Queue< int > b;
  BOOST_CHECK(a.size() == 2 && b.size() == 0);
  const goltsov::Queue< int > ac = a;
  const goltsov::Queue< int > bc = b;
  BOOST_CHECK(ac.size() == 2 && bc.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  goltsov::Queue< int > a;
  a.push(5);
  a.push(4);
  a.clear();
  BOOST_CHECK_THROW(a.pop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(emplace_test)
{
  goltsov::Queue< std::pair< int, std::string > > q;
  q.emplace(1, "one");
  BOOST_CHECK(q.size() == 1);
  BOOST_CHECK(q.front().first == 1);
  BOOST_CHECK(q.front().second == "one");
  q.emplace(2, "two");
  BOOST_CHECK(q.size() == 2);
  BOOST_CHECK(q.front().first == 1);
  BOOST_CHECK(q.front().second == "one");
  q.pop();
  BOOST_CHECK(q.front().first == 2);
  BOOST_CHECK(q.front().second == "two");
  goltsov::Queue< std::tuple< int, double, char > > t;
  t.emplace(42, 3.14, 'a');
  BOOST_CHECK(t.size() == 1);
  BOOST_CHECK(std::get<0>(t.front()) == 42);
  BOOST_CHECK(std::get<1>(t.front()) == 3.14);
  BOOST_CHECK(std::get<2>(t.front()) == 'a');
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
  goltsov::Queue< NonCopyable > q;
  BOOST_CHECK_NO_THROW(q.emplace(10, 20));
  BOOST_CHECK(q.size() == 1);
  BOOST_CHECK(q.front().value == 10);
  BOOST_CHECK(*q.front().ptr == 20);
  BOOST_CHECK_NO_THROW(q.emplace(30, 40));
  BOOST_CHECK(q.size() == 2);
  BOOST_CHECK(q.front().value == 10);
  BOOST_CHECK(*q.front().ptr == 20);
  q.pop();
  BOOST_CHECK(q.front().value == 30);
  BOOST_CHECK(*q.front().ptr == 40);
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
  goltsov::Queue< ThreeArgs > q;
  q.emplace(42, 3.14, "hello");
  BOOST_CHECK(q.size() == 1);
  BOOST_CHECK(q.front().a == 42);
  BOOST_CHECK(q.front().b == 3.14);
  BOOST_CHECK(q.front().c == "hello");
}

BOOST_AUTO_TEST_CASE(emplace_order_test)
{
  goltsov::Queue< int > q;
  q.emplace(1);
  q.emplace(2);
  q.emplace(3);
  BOOST_CHECK(q.front() == 1);
  q.pop();
  BOOST_CHECK(q.front() == 2);
  q.pop();
  BOOST_CHECK(q.front() == 3);
  q.pop();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_SUITE_END()
