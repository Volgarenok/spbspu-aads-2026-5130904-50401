#include <boost/test/unit_test.hpp>
#include "STRUCTS.hpp"

BOOST_AUTO_TEST_SUITE(List_suite)

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  petrov::List< int > a;
  BOOST_CHECK(a.begin() == petrov::LIter< int >(nullptr));
  BOOST_CHECK(a.end() == petrov::LIter< int >(nullptr));
  BOOST_CHECK(a.getLast() == petrov::LIter< int >(nullptr));
  BOOST_CHECK(a.size() == 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);
  a.addStart(3);

  petrov::List< int > b(a);
  BOOST_CHECK(b.size() == 3);
  BOOST_CHECK(*b.begin() == 3);

  petrov::LIter< int > it = b.begin();
  BOOST_CHECK(*it == 3);
  it = it.next();
  BOOST_CHECK(*it == 2);
  it = it.next();
  BOOST_CHECK(*it == 1);

  a.addStart(100);
  BOOST_CHECK(a.size() == 4);
  BOOST_CHECK(b.size() == 3);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);

  petrov::LIter< int > temp = a.begin();
  petrov::List< int > b(std::move(a));

  BOOST_CHECK(temp == b.begin());
  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(a.size() == 0);
  BOOST_CHECK(a.begin() == petrov::LIter< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);

  petrov::List< int > b;
  b = a;

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 2);

  a = a;
  BOOST_CHECK(a.size() == 2);
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);

  petrov::List< int > b;
  b.addStart(100);

  petrov::LIter< int > temp = a.begin();
  b = std::move(a);

  BOOST_CHECK(temp == b.begin());
  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(a.size() == 0);
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  petrov::List< int > a;
  BOOST_CHECK(a.begin() == petrov::LIter< int >(nullptr));
  BOOST_REQUIRE_THROW(*a.begin(), std::runtime_error);

  a.addStart(1);
  BOOST_CHECK(a.begin() != petrov::LIter< int >(nullptr));
  BOOST_CHECK_NO_THROW(*a.begin());
  BOOST_CHECK(*a.begin() == 1);

  const petrov::List< int > b;
  BOOST_CHECK(b.begin() == petrov::LCIter< int >(nullptr));
  BOOST_REQUIRE_THROW(*b.begin(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  petrov::List< int > a;
  BOOST_CHECK(a.end() == petrov::LIter< int >(nullptr));

  a.addStart(1);
  BOOST_CHECK(a.end() == petrov::LIter< int >(nullptr));

  const petrov::List< int > b;
  BOOST_CHECK(b.end() == petrov::LCIter< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(getLast_test)
{
  petrov::List< int > a;
  BOOST_CHECK(a.getLast() == petrov::LIter< int >(nullptr));

  const petrov::List< int > b;
  BOOST_CHECK(b.getLast() == petrov::LCIter< int >(nullptr));

  a.addStart(1);
  BOOST_CHECK(a.getLast() != petrov::LIter< int >(nullptr));
  BOOST_CHECK(*a.getLast() == 1);

  const petrov::List< int > b1 = a;
  BOOST_CHECK(b1.getLast() != petrov::LCIter< int >(nullptr));
  BOOST_CHECK(*b1.getLast() == 1);

  petrov::LIter< int > last = a.getLast();
  a.insert(last, 2);
  BOOST_CHECK(*a.getLast() == 2);

  const petrov::List< int > b2 = a;
  BOOST_CHECK(*b2.getLast() == 2);
}

BOOST_AUTO_TEST_CASE(addStart_test)
{
  petrov::List< int > a;
  petrov::LIter< int > it = a.addStart(1);

  BOOST_CHECK(it != petrov::LIter< int >(nullptr));
  BOOST_CHECK(*it == 1);
  BOOST_CHECK(*a.begin() == 1);
  BOOST_CHECK(a.size() == 1);

  a.addStart(2);
  BOOST_CHECK(*a.begin() == 2);
  BOOST_CHECK(a.size() == 2);

  BOOST_CHECK(*a.getLast() == 1);
}

BOOST_AUTO_TEST_CASE(popStart_test)
{
  petrov::List< int > a;
  a.addStart(2);
  a.addStart(1);

  BOOST_CHECK(a.size() == 2);

  a.popStart();
  BOOST_CHECK(*a.begin() == 2);
  BOOST_CHECK(a.size() == 1);

  a.popStart();
  BOOST_CHECK(a.size() == 0);
  BOOST_CHECK(a.begin() == petrov::LIter< int >(nullptr));
  BOOST_CHECK(a.getLast() == petrov::LIter< int >(nullptr));

  BOOST_CHECK_NO_THROW(a.popStart());
}

BOOST_AUTO_TEST_CASE(insert_lvalue_test)
{
  petrov::List< int > a;
  int val = 1;
  petrov::LIter< int > it = a.insert(a.begin(), val);

  BOOST_CHECK(it != petrov::LIter< int >(nullptr));
  BOOST_CHECK(*it == 1);
  BOOST_CHECK(a.size() == 1);

  it = a.insert(a.begin(), val);
  BOOST_CHECK(*it == 1);
  BOOST_CHECK(*a.begin() == 1);
  BOOST_CHECK(a.size() == 2);
}

BOOST_AUTO_TEST_CASE(insert_rvalue_test)
{
  petrov::List< int > a;
  petrov::LIter< int > it = a.insert(a.begin(), 1);

  BOOST_CHECK(it != petrov::LIter< int >(nullptr));
  BOOST_CHECK(*it == 1);
  BOOST_CHECK(a.size() == 1);
}

BOOST_AUTO_TEST_CASE(insert_after_position_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);
  a.addStart(3);

  petrov::LIter< int > it = a.begin();
  it = a.insert(it, 100);

  BOOST_CHECK(*a.begin() == 3);
  BOOST_CHECK(*it == 100);
  BOOST_CHECK(a.size() == 4);

  petrov::LIter< int > last = a.getLast();
  a.insert(last, 200);
  BOOST_CHECK(*a.getLast() == 200);
  BOOST_CHECK(a.size() == 5);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  petrov::List< int > a;
  a.clear();
  BOOST_CHECK(a.begin() == petrov::LIter< int >(nullptr));
  BOOST_CHECK(a.size() == 0);

  a.addStart(1);
  a.addStart(2);
  a.clear();
  BOOST_CHECK(a.begin() == petrov::LIter< int >(nullptr));
  BOOST_CHECK(a.getLast() == petrov::LIter< int >(nullptr));
  BOOST_CHECK(a.size() == 0);

  BOOST_CHECK_NO_THROW(a.clear());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  petrov::List< int > a;
  BOOST_CHECK(a.size() == 0);

  a.addStart(1);
  BOOST_CHECK(a.size() == 1);

  a.addStart(2);
  BOOST_CHECK(a.size() == 2);

  a.popStart();
  BOOST_CHECK(a.size() == 1);

  a.clear();
  BOOST_CHECK(a.size() == 0);
}

BOOST_AUTO_TEST_CASE(iterator_comparison_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);

  petrov::LIter< int > it1 = a.begin();
  petrov::LIter< int > it2 = a.begin();
  petrov::LIter< int > it3 = a.end();

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(it1 != it3);
  BOOST_CHECK(!(it1 == it3));
}

BOOST_AUTO_TEST_CASE(iterator_dereference_test)
{
  petrov::List< int > a;
  a.addStart(42);

  petrov::LIter< int > it = a.begin();
  BOOST_CHECK(*it == 42);

  *it = 100;
  BOOST_CHECK(*a.begin() == 100);

  const petrov::List< int > b;
  BOOST_REQUIRE_THROW(*b.begin(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(iterator_next_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);
  a.addStart(3);

  petrov::LIter< int > it = a.begin();
  BOOST_CHECK(*it == 3);

  it = it.next();
  BOOST_CHECK(*it == 2);

  it = it.next();
  BOOST_CHECK(*it == 1);

  it = it.next();
  BOOST_CHECK(it == a.end());

  BOOST_REQUIRE_THROW(it.next(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  petrov::List< int > a;
  a.addStart(1);
  a.addStart(2);

  const petrov::List< int > b = a;
  petrov::LCIter< int > it = b.begin();

  BOOST_CHECK(*it == 2);
  it = it.next();
  BOOST_CHECK(*it == 1);
  it = it.next();
  BOOST_CHECK(it == b.end());
}

BOOST_AUTO_TEST_SUITE_END()
