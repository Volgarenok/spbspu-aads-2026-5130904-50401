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
	BOOST_CHECK(a.size() == 2 && a.front() == 5 && a.back() == 4);
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  goltsov::Queue< int > a;
  BOOST_CHECK_THROW(a.drop(), std::runtime_error);
	a.push(5);
	a.push(4);
  a.drop();
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

BOOST_AUTO_TEST_CASE(back_test)
{
  goltsov::Queue< int > a;
  BOOST_CHECK_THROW(a.back(), std::runtime_error);
	a.push(5);
	a.push(4);
  BOOST_CHECK(a.back() == 4);
  const goltsov::Queue< int > ac = a;
  BOOST_CHECK(ac.back() == 4);
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
  BOOST_CHECK_THROW(a.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
