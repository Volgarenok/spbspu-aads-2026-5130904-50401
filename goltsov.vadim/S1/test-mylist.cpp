#include <boost/test/unit_test.hpp>
#include "mylist.hpp"

BOOST_AUTO_TEST_SUITE(List_suite)
BOOST_AUTO_TEST_CASE(designer_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.begin() == goltsov::LIter< int >(nullptr));
  a.push_start(1);
  BOOST_CHECK(a.begin() != goltsov::LIter< int >(nullptr));
  goltsov::List< int > b(a);
  BOOST_CHECK(b.begin() != goltsov::LIter< int >(nullptr));
  goltsov::LIter< int > temp = b.begin();
  goltsov::List< int > bc(std::move(b));
  BOOST_CHECK(temp == bc.begin());
  BOOST_CHECK(bc.begin() != goltsov::LIter< int >(nullptr));
  goltsov::List< int > bo = a;
  BOOST_CHECK(bo.begin() != goltsov::LIter< int >(nullptr));
  goltsov::LIter< int > tempo = bo.begin();
  goltsov::List< int > boc = std::move(bo);
  BOOST_CHECK(tempo == boc.begin());
  BOOST_CHECK(boc.begin() != goltsov::LIter< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.begin() == goltsov::LIter< int >(nullptr));
  BOOST_REQUIRE_THROW(* a.begin(), std::runtime_error);
  a.push_start(1);
  BOOST_CHECK(a.begin() != goltsov::LIter< int >(nullptr));
  BOOST_CHECK_NO_THROW(* a.begin());

  const goltsov::List< int > b;
  BOOST_CHECK(b.begin() == goltsov::LCIter< int >(nullptr));
  BOOST_REQUIRE_THROW(* b.begin(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.end() == goltsov::LIter< int > (nullptr));

  const goltsov::List< int > b;
  BOOST_CHECK(b.end() == goltsov::LCIter< int > (nullptr));
}

BOOST_AUTO_TEST_CASE(getLast_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.getLast() == goltsov::LIter< int > (nullptr));
  const goltsov::List< int > b;
  BOOST_CHECK(b.getLast() == goltsov::LCIter< int > (nullptr));
  a.push_start(1);
  BOOST_CHECK(a.getLast() != goltsov::LIter< int > (nullptr) && (* a.getLast()) == 1);
  const goltsov::List< int > b1 = a;
  BOOST_CHECK(b1.getLast() != goltsov::LCIter< int > (nullptr) && (* b1.getLast()) == 1);
  a.insert(a.begin(), 2);
  BOOST_CHECK(a.getLast() != goltsov::LIter< int > (nullptr) && (* a.getLast()) == 2);
  const goltsov::List< int > b2 = a;
  BOOST_CHECK(b2.getLast() != goltsov::LCIter< int > (nullptr) && (* b2.getLast()) == 2);
}

BOOST_AUTO_TEST_CASE(push_start_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.push_start(1) != goltsov::LIter< int > (nullptr) && (* a.begin()) == 1);
}

BOOST_AUTO_TEST_CASE(pop_start_test)
{
  goltsov::List< int > a;
  a.push_start(2);
  a.push_start(1);
  a.pop_start();
  BOOST_CHECK((* a.begin()) == 2);
  a.pop_start();
  a.pop_start();
  BOOST_CHECK(a.begin() == goltsov::LIter< int > (nullptr));
}

BOOST_AUTO_TEST_CASE(pop_end_test)
{
  goltsov::List< int > a;
  a.push_start(2);
  a.push_start(1);
  a.pop_end();
  BOOST_CHECK((* a.begin()) == 1);
  a.pop_end();
  a.pop_end();
  BOOST_CHECK(a.begin() == goltsov::LIter< int > (nullptr));
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  goltsov::List< int > a;
  BOOST_CHECK((* a.insert(a.begin(), 1)) == 1 && a.begin() != goltsov::LIter< int > (nullptr));
  BOOST_CHECK((* a.insert(a.begin(), 2)) == 2);
  int c1 = 1;
  int c2 = 2;
  a.clear();
  BOOST_CHECK((* a.insert(a.begin(), c1)) == 1 && a.begin() != goltsov::LIter< int > (nullptr));
  BOOST_CHECK((* a.insert(a.begin(), c2)) == 2);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  goltsov::List< int > a;
  a.clear();
  BOOST_CHECK(a.begin() == goltsov::LIter< int > (nullptr));
  a.push_start(1);
  a.push_start(2);
  a.clear();
  BOOST_CHECK(a.begin() == goltsov::LIter< int > (nullptr));
}

BOOST_AUTO_TEST_SUITE_END()
