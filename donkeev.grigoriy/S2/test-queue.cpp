#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(queueTesting)
BOOST_AUTO_TEST_CASE(defaultConstruct)
{
  donkeev::Queue< int > queue;
  BOOST_TEST(queue.size() == 0);
  BOOST_TEST(queue.isEmpty());
}
BOOST_AUTO_TEST_CASE(initializerListConstruct)
{
  donkeev::Queue< int > queue{1, 2};
  BOOST_TEST(queue.size() == 2);
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(pushTesting)
{
  donkeev::Queue< int > queue;
  BOOST_TEST(queue.size() == 0);
  queue.push(1);
  BOOST_TEST(queue.size() == 1);
  BOOST_TEST(queue.front() == 1);
}
BOOST_AUTO_TEST_CASE(popTesting)
{
  donkeev::Queue< int > queue{1, 2};
  BOOST_TEST(queue.size() == 2);
  BOOST_TEST(queue.front() == 1);
  queue.pop();
  BOOST_TEST(queue.size() == 1);
  BOOST_TEST(queue.front() == 2);
}

BOOST_AUTO_TEST_CASE(frontValue)
{
  donkeev::Queue< int > queue{1, 2};
  BOOST_TEST(queue.front() == 1);
  queue.pop();
  BOOST_TEST(queue.front() == 2);
}
BOOST_AUTO_TEST_CASE(constFrontValue)
{
  const donkeev::Queue< int > queue{1, 2};
  BOOST_TEST(queue.front() == 1);
}

BOOST_AUTO_TEST_CASE(sizeTest)
{
  donkeev::Queue< int > queue{1, 2};
  BOOST_TEST(queue.size() == 2);
  queue.pop();
  BOOST_TEST(queue.size() == 1);
}
BOOST_AUTO_TEST_CASE(isEmptyTest)
{
  donkeev::Queue< int > queue{1};
  BOOST_TEST(!queue.isEmpty());
  queue.pop();
  BOOST_TEST(queue.isEmpty());
}
BOOST_AUTO_TEST_CASE(clearTest)
{
  donkeev::Queue< int > queue{1, 2};
  BOOST_TEST(queue.size() == 2);
  queue.clear();
  BOOST_TEST(queue.size() == 0);
  BOOST_TEST(queue.isEmpty());
}

BOOST_AUTO_TEST_CASE(destructTest)
{
  donkeev::Queue< int > queue{1, 2};
  queue.~Queue();
  BOOST_TEST(queue.size() == 0);
  BOOST_TEST(queue.isEmpty());
}
BOOST_AUTO_TEST_SUITE_END()
