#include <boost/test/unit_test.hpp>

#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTest)

BOOST_AUTO_TEST_CASE(constructAndOperators)
{
  yalovsky::Queue< int > queue1;
  BOOST_TEST(queue1.empty());

  queue1.push(1);
  yalovsky::Queue< int > queue2(queue1);
  BOOST_TEST(queue2.size() == 1);
  BOOST_TEST(queue2.front() == 1);
  BOOST_TEST(queue2.back() == 1);

  queue2.push(2);
  yalovsky::Queue< int > queue3(std::move(queue2));
  BOOST_TEST(queue3.size() == 2);
  BOOST_TEST(queue3.front() == 1);
  BOOST_TEST(queue3.back() == 2);

  queue3.push(3);
  yalovsky::Queue< int > queue4 = queue3;
  BOOST_TEST(queue4.size() == 3);
  BOOST_TEST(queue4.front() == 1);
  BOOST_TEST(queue4.back() == 3);

  yalovsky::Queue< int > queue5 = std::move(queue4);
  BOOST_TEST(queue5.size() == 3);
  BOOST_TEST(queue5.front() == 1);
  BOOST_TEST(queue5.back() == 3);
}

BOOST_AUTO_TEST_CASE(emptyAndSize)
{
  yalovsky::Queue< int > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);

  queue.push(1);
  queue.push(2);
  BOOST_TEST(!queue.empty());
  BOOST_TEST(queue.size() == 2);
}

BOOST_AUTO_TEST_CASE(pushPopFrontBackAndDrop)
{
  yalovsky::Queue< int > queue;
  queue.push(1);
  queue.push(2);
  queue.push(3);

  BOOST_TEST(queue.front() == 1);
  BOOST_TEST(queue.back() == 3);
  BOOST_TEST(queue.drop() == 1);
  BOOST_TEST(queue.front() == 2);

  queue.pop();
  BOOST_TEST(queue.front() == 3);
}

BOOST_AUTO_TEST_CASE(swapAndClear)
{
  yalovsky::Queue< int > queue1;
  queue1.push(1);

  yalovsky::Queue< int > queue2;
  queue2.push(2);
  queue2.push(3);

  queue1.swap(queue2);
  BOOST_TEST(queue1.front() == 2);
  BOOST_TEST(queue1.back() == 3);
  BOOST_TEST(queue1.size() == 2);
  BOOST_TEST(queue2.front() == 1);
  BOOST_TEST(queue2.size() == 1);

  queue1.clear();
  BOOST_TEST(queue1.empty());
}

BOOST_AUTO_TEST_SUITE_END()
