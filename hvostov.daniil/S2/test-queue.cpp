#include <boost/test/unit_test.hpp>
#include <queue.hpp>

BOOST_AUTO_TEST_SUITE(queue_suite)

BOOST_AUTO_TEST_CASE(test_create_queue)
{
  hvostov::Queue< int > queue;
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_push_element)
{
  hvostov::Queue< int > queue;
  queue.push(6);
  BOOST_CHECK(!queue.empty());
  BOOST_CHECK(queue.front() == 6);
  queue.push(7);
  BOOST_CHECK(queue.front() == 6);
  queue.push(8);
  BOOST_CHECK(queue.front() == 6);
}

BOOST_AUTO_TEST_CASE(test_pop_element)
{
  hvostov::Queue< int > queue;
  queue.push(6);
  queue.push(7);
  queue.push(8);
  BOOST_CHECK(queue.front() == 6);
  queue.pop();
  BOOST_CHECK(queue.front() == 7);
  queue.pop();
  BOOST_CHECK(queue.front() == 8);
  queue.pop();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::Queue< int > queue;
  queue.push(6);
  queue.push(7);
  queue.push(8);
  BOOST_CHECK(!queue.empty());
  queue.clear();
  BOOST_CHECK(queue.empty());
  queue.push(6);
  queue.push(7);
  BOOST_CHECK(!queue.empty());
  queue.clear();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  hvostov::Queue< int > queue;
  BOOST_CHECK_EQUAL(queue.size(), 0);

  queue.push(1);
  BOOST_CHECK_EQUAL(queue.size(), 1);

  queue.push(2);
  BOOST_CHECK_EQUAL(queue.size(), 2);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.size(), 1);

  queue.clear();
  BOOST_CHECK_EQUAL(queue.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_push)
{
  hvostov::Queue< std::string > queue;
  std::string str = "test";
  queue.push(std::move(str));
  BOOST_CHECK_EQUAL(queue.front(), "test");
}

BOOST_AUTO_TEST_CASE(test_copy_and_move_queue)
{
  hvostov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);

  hvostov::Queue< int > queue2 = queue1;
  BOOST_CHECK_EQUAL(queue2.front(), 1);

  hvostov::Queue< int > queue3 = std::move(queue1);
  BOOST_CHECK_EQUAL(queue3.front(), 1);
  BOOST_CHECK(queue1.empty());
}

BOOST_AUTO_TEST_CASE(test_large_queue)
{
  hvostov::Queue< int > queue;
  const int N = 1000;

  for (int i = 0; i < N; ++i) {
    queue.push(i);
  }

  BOOST_CHECK_EQUAL(queue.size(), N);

  for (int i = 0; i < N; ++i) {
    BOOST_CHECK_EQUAL(queue.front(), i);
    queue.pop();
  }

  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(test_front_after_operations)
{
  hvostov::Queue< int > queue;
  queue.push(10);
  queue.push(20);
  queue.push(30);

  BOOST_CHECK_EQUAL(queue.front(), 10);

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 20);

  queue.push(40);
  BOOST_CHECK_EQUAL(queue.front(), 20);

  queue.pop();
  queue.pop();
  BOOST_CHECK_EQUAL(queue.front(), 40);
}

BOOST_AUTO_TEST_CASE(test_queue_with_different_types)
{
  hvostov::Queue< double > dqueue;
  dqueue.push(3.14);
  dqueue.push(2.718);
  BOOST_CHECK_EQUAL(dqueue.front(), 3.14);

  hvostov::Queue< char > cqueue;
  cqueue.push('A');
  cqueue.push('B');
  BOOST_CHECK_EQUAL(cqueue.front(), 'A');
}

BOOST_AUTO_TEST_CASE(test_queue_emplace)
{
  hvostov::Queue< std::pair< int, std::string > > queue;

  queue.emplace(1, "one");
  queue.emplace(2, "two");
  queue.emplace(3, "three");

  BOOST_CHECK_EQUAL(queue.size(), 3);
  BOOST_CHECK_EQUAL(queue.front().first, 1);
  BOOST_CHECK_EQUAL(queue.front().second, "one");

  queue.pop();
  BOOST_CHECK_EQUAL(queue.front().first, 2);

  queue.pop();
  queue.pop();
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_SUITE_END()
