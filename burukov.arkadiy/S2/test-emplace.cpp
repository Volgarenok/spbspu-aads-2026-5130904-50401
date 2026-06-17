#include <boost/test/unit_test.hpp>
#include <memory>
#include <list.hpp>
#include "stack.hpp"
#include "queue.hpp"

struct TestStruct
{
  int a_;
  double b_;
  std::string c_;

  TestStruct(int a, double b, const std::string &c):
    a_(a),
    b_(b),
    c_(c)
  {}

  bool operator==(const TestStruct &other) const
  {
    return a_ == other.a_ && b_ == other.b_ && c_ == other.c_;
  }
};

BOOST_AUTO_TEST_SUITE(ListEmplaceTest)

BOOST_AUTO_TEST_CASE(emplaceFront)
{
  burukov::List< TestStruct > list;
  list.emplaceFront(1, 2.5, "hello");
  BOOST_CHECK(list.front().a_ == 1);
  BOOST_CHECK(list.front().b_ == 2.5);
  BOOST_CHECK(list.front().c_ == "hello");
  BOOST_CHECK(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(emplaceBack)
{
  burukov::List< TestStruct > list;
  list.emplaceBack(1, 2.5, "hello");
  list.emplaceBack(2, 3.5, "world");
  BOOST_CHECK(list.front().a_ == 1);
  BOOST_CHECK(list.front().b_ == 2.5);
  BOOST_CHECK(list.front().c_ == "hello");
  BOOST_CHECK(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(emplaceAfter)
{
  burukov::List< TestStruct > list;
  list.emplaceBack(1, 2.5, "first");
  burukov::LIter< TestStruct > it = list.begin();
  list.emplaceAfter(it, 2, 3.5, "second");
  BOOST_CHECK(list.size() == 2);
  ++it;
  BOOST_CHECK(it->a_ == 2);
  BOOST_CHECK(it->b_ == 3.5);
  BOOST_CHECK(it->c_ == "second");
}

BOOST_AUTO_TEST_CASE(emplaceFrontEmptyList)
{
  burukov::List< TestStruct > list;
  list.emplaceFront(10, 20.5, "test");
  BOOST_CHECK(!list.empty());
  BOOST_CHECK(list.front().a_ == 10);
}

BOOST_AUTO_TEST_CASE(emplaceBackEmptyList)
{
  burukov::List< TestStruct > list;
  list.emplaceBack(10, 20.5, "test");
  BOOST_CHECK(!list.empty());
  BOOST_CHECK(list.front().a_ == 10);
}

BOOST_AUTO_TEST_CASE(emplaceAfterEnd)
{
  burukov::List< TestStruct > list;
  list.emplaceBack(1, 1.0, "one");
  burukov::LIter< TestStruct > it = list.end();
  burukov::LIter< TestStruct > result = list.emplaceAfter(it, 2, 2.0, "two");
  BOOST_CHECK(result == list.end());
  BOOST_CHECK(list.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(StackEmplaceTest)

BOOST_AUTO_TEST_CASE(emplaceInt)
{
  burukov::Stack< TestStruct > stack;
  stack.emplace(1, 2.5, "hello");
  BOOST_CHECK(stack.top().a_ == 1);
  BOOST_CHECK(stack.top().b_ == 2.5);
  BOOST_CHECK(stack.top().c_ == "hello");
  BOOST_CHECK(stack.size() == 1);
}

BOOST_AUTO_TEST_CASE(emplaceMultiple)
{
  burukov::Stack< TestStruct > stack;
  stack.emplace(1, 1.0, "first");
  stack.emplace(2, 2.0, "second");
  stack.emplace(3, 3.0, "third");
  BOOST_CHECK(stack.size() == 3);
  BOOST_CHECK(stack.top().a_ == 3);
  stack.pop();
  BOOST_CHECK(stack.top().a_ == 2);
  stack.pop();
  BOOST_CHECK(stack.top().a_ == 1);
}

BOOST_AUTO_TEST_CASE(emplaceVsPush)
{
  burukov::Stack< TestStruct > stack;
  stack.emplace(1, 1.0, "emplace");
  TestStruct ts(2, 2.0, "push");
  stack.push(ts);
  BOOST_CHECK(stack.top().a_ == 2);
  stack.pop();
  BOOST_CHECK(stack.top().a_ == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(QueueEmplaceTest)

BOOST_AUTO_TEST_CASE(emplaceInt)
{
  burukov::Queue< TestStruct > queue;
  queue.emplace(1, 2.5, "hello");
  BOOST_CHECK(queue.front().a_ == 1);
  BOOST_CHECK(queue.front().b_ == 2.5);
  BOOST_CHECK(queue.front().c_ == "hello");
  BOOST_CHECK(queue.size() == 1);
}

BOOST_AUTO_TEST_CASE(emplaceMultiple)
{
  burukov::Queue< TestStruct > queue;
  queue.emplace(1, 1.0, "first");
  queue.emplace(2, 2.0, "second");
  queue.emplace(3, 3.0, "third");
  BOOST_CHECK(queue.size() == 3);
  BOOST_CHECK(queue.front().a_ == 1);
  queue.pop();
  BOOST_CHECK(queue.front().a_ == 2);
  queue.pop();
  BOOST_CHECK(queue.front().a_ == 3);
}

BOOST_AUTO_TEST_CASE(emplaceVsPush)
{
  burukov::Queue< TestStruct > queue;
  queue.emplace(1, 1.0, "emplace");
  TestStruct ts(2, 2.0, "push");
  queue.push(ts);
  BOOST_CHECK(queue.front().a_ == 1);
  queue.pop();
  BOOST_CHECK(queue.front().a_ == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(EmplaceWithNonCopyable)

struct NonCopyable
{
  int value_;
  std::unique_ptr< int > ptr_;

  NonCopyable(int v, int p):
    value_(v),
    ptr_(std::make_unique< int >(p))
  {}

  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;

  NonCopyable(NonCopyable &&) = default;
  NonCopyable &operator=(NonCopyable &&) = default;

  bool operator==(const NonCopyable &other) const
  {
    return value_ == other.value_ && *ptr_ == *other.ptr_;
  }
};

BOOST_AUTO_TEST_CASE(emplaceNonCopyableList)
{
  burukov::List< NonCopyable > list;
  list.emplaceFront(10, 20);
  list.emplaceBack(30, 40);
  BOOST_CHECK(list.front().value_ == 10);
  BOOST_CHECK(*list.front().ptr_ == 20);
  BOOST_CHECK(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(emplaceNonCopyableStack)
{
  burukov::Stack< NonCopyable > stack;
  stack.emplace(10, 20);
  stack.emplace(30, 40);
  BOOST_CHECK(stack.top().value_ == 30);
  BOOST_CHECK(*stack.top().ptr_ == 40);
}

BOOST_AUTO_TEST_CASE(emplaceNonCopyableQueue)
{
  burukov::Queue< NonCopyable > queue;
  queue.emplace(10, 20);
  queue.emplace(30, 40);
  BOOST_CHECK(queue.front().value_ == 10);
  BOOST_CHECK(*queue.front().ptr_ == 20);
}

BOOST_AUTO_TEST_SUITE_END()
