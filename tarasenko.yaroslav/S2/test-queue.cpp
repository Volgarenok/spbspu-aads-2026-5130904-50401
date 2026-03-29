#include "boost/test/unit_test.hpp"
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_push)
{
  tarasenko::Queue< int > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
  queue.push(5);
  BOOST_TEST(queue.size() == 1);
  BOOST_TEST(queue.front() == 5);
  BOOST_TEST(queue.back() == 5);
  const int& num1 = 3;
  queue.push(num1);
  BOOST_TEST(queue.front() == 5);
  BOOST_TEST(queue.back() == 3);
  const int& num2 = 2;
  queue.push(std::move(num2));
  BOOST_TEST(queue.front() == 5);
  BOOST_TEST(queue.back() == 2);
  queue.clear();
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(queue_pop)
{
  tarasenko::Queue< int > queue;
  for (size_t i = 0; i < 10; ++i)
  {
    queue.push(i);
  }
  for (size_t i = 0; i < 10; ++i)
  {
    BOOST_TEST(queue.pop() == i);
    BOOST_TEST(queue.size() == 10 - (i + 1));
  }
}

BOOST_AUTO_TEST_CASE(queue_copy_and_move_semantic)
{
  tarasenko::Queue< int > queue;
  for (size_t i = 0; i < 10; ++i)
  {
    queue.push(i);
  }
  tarasenko::Queue< int > copy = queue;
  BOOST_TEST(copy.front() == 0);
  BOOST_TEST(copy.back() == 9);
  tarasenko::Queue< int > copy2;
  copy2 = queue;
  BOOST_TEST(copy2.front() == 0);
  BOOST_TEST(copy2.back() == 9);
  tarasenko::Queue< int > moved = std::move(queue);
  BOOST_TEST(moved.front() == 0);
  BOOST_TEST(moved.back() == 9);
  tarasenko::Queue< int > moved2;
  moved2 = std::move(moved);
  BOOST_TEST(moved2.front() == 0);
  BOOST_TEST(moved2.back() == 9);
}
