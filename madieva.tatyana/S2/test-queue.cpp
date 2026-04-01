#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(queueTest)

BOOST_AUTO_TEST_CASE(default_queue_constructor)
{
  madieva::Queue< int > q;
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(add_back_a_few_items)
{
  madieva::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK(q.size() == 3);
  BOOST_CHECK(!q.empty());
}

BOOST_AUTO_TEST_CASE(deleting__items)
{
  madieva::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.pop();
  q.pop();
  q.pop();
  BOOST_CHECK(q.size() == 0);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(checking_the_first_element_without_deleting_it)
{
  madieva::Queue< int > q;
  q.push(1);
  BOOST_CHECK(q.front() == 1);
  q.push(2);
  BOOST_CHECK(q.front() == 1);
  q.push(3);
  BOOST_CHECK(q.front() == 1);
  BOOST_CHECK(q.size() == 3);
  BOOST_CHECK(!q.empty());
}

BOOST_AUTO_TEST_CASE(checking_the_first_item_from_deletion)
{
  madieva::Queue< int > q;
  q.push(1);
  BOOST_CHECK(q.front() == 1);
  q.push(2);
  BOOST_CHECK(q.front() == 1);
  q.push(3);
  BOOST_CHECK(q.front() == 1);
  BOOST_CHECK(q.size() == 3);
  BOOST_CHECK(!q.empty());
  q.pop();
  BOOST_CHECK(q.front() == 2);
  q.pop();
  BOOST_CHECK(q.front() == 3);
  q.pop();
  BOOST_CHECK(q.size() == 0);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(access_to_a_constant_element)
{
  madieva::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  const madieva::Queue< int > cq = q;

  const int & a = cq.front();
  BOOST_CHECK(a == 1);
}

BOOST_AUTO_TEST_CASE(front_when_the_queue_is_empty)
{
  madieva::Queue< int > q;
  BOOST_CHECK_THROW(q.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(pop_when_the_queue_is_empty)
{
  madieva::Queue< int > q;
  q.pop();
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear_queue)
{
  madieva::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.clear();
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear_empty_queue)
{
  madieva::Queue< int > q;
  q.clear();
  BOOST_CHECK(q.empty());
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(size_after_operations)
{
  madieva::Queue<int> q;
  BOOST_CHECK(q.size() == 0);
  q.push(10);
  BOOST_CHECK(q.size() == 1);
  q.push(20);
  BOOST_CHECK(q.size() == 2);
  q.pop();
  BOOST_CHECK(q.size() == 1);
  q.clear();
  BOOST_CHECK(q.size() == 0);
}

BOOST_AUTO_TEST_CASE(empty_on_const_queue)
{
  madieva::Queue<int> q;
  q.push(1);
  const madieva::Queue<int>& cq = q;
  BOOST_CHECK(!cq.empty());
  BOOST_CHECK(q.front() == 1);
  BOOST_CHECK(cq.front() == 1);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  madieva::Queue<int> q1;
  q1.push(1);
  q1.push(2);
  madieva::Queue<int> q2(q1);
  BOOST_CHECK(q2.size() == 2);
  BOOST_CHECK(q2.front() == 1);
  q2.pop();
  BOOST_CHECK(q2.front() == 2);
  BOOST_CHECK(q1.size() == 2);
}

BOOST_AUTO_TEST_CASE(assignment_operator)
{
  madieva::Queue<int> q1;
  q1.push(1);
  q1.push(2);
  madieva::Queue<int> q2;
  q2 = q1;
  BOOST_CHECK(q2.size() == 2);
  BOOST_CHECK(q2.front() == 1);
  BOOST_CHECK(q1.size() == 2);
  BOOST_CHECK(q1.front() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
