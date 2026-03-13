#include "list.hpp"
#include <boost/test/unit_test.hpp>

using namespace levkin;
BOOST_AUTO_TEST_CASE(constructor_test)
{
  List< int > l(3);
  BOOST_CHECK(l.begin() != l.end());
  BOOST_CHECK(*(l.begin()) == 3);
}

BOOST_AUTO_TEST_CASE(iter_test)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  auto it2 = l.begin();
  BOOST_CHECK_EQUAL(*it2, 1);
  BOOST_CHECK_EQUAL(*(++it2), 2);
  BOOST_CHECK_EQUAL(*(++it2), 3);
  BOOST_CHECK(++it2 == l.end());
  BOOST_CHECK_EQUAL(*(--it2), 3);
  BOOST_CHECK_EQUAL(*(--it2), 2);
  BOOST_CHECK_EQUAL(*(--it2), 1);
}

BOOST_AUTO_TEST_CASE(comp_iters)
{
  List< int > l;
  l.pushBack(10);

  auto it1 = l.begin();
  auto it2 = l.cbegin();

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK_EQUAL(*it1, *it2);
}

BOOST_AUTO_TEST_CASE(citer_test)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  auto it2 = l.cbegin();
  BOOST_CHECK_EQUAL(*it2, 1);
  BOOST_CHECK_EQUAL(*(++it2), 2);
  BOOST_CHECK_EQUAL(*(++it2), 3);
  BOOST_CHECK(++it2 == l.end());
  BOOST_CHECK_EQUAL(*(--it2), 3);
  BOOST_CHECK_EQUAL(*(--it2), 2);
  BOOST_CHECK_EQUAL(*(--it2), 1);
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  l.erase(++l.begin());

  auto it = l.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(*(++it), 3);

  l.clear();
}
BOOST_AUTO_TEST_CASE(push_and_insert_test)
{
  List< int > l;

  l.pushBack(2);
  l.pushFront(1);
  l.pushBack(3);
  l.pushBack(3);
  l.pushFront(3);

  l.popFront();
  l.popBack();

  auto it = l.begin();

  auto inserted_it = l.insertAfter(it, 10);
  BOOST_CHECK_EQUAL(*inserted_it, 10);

  auto check_it = l.begin();
  BOOST_CHECK_EQUAL(*check_it, 1);
  BOOST_CHECK_EQUAL(*(++check_it), 10);
  BOOST_CHECK_EQUAL(*(++check_it), 2);
  BOOST_CHECK_EQUAL(*(++check_it), 3);

  BOOST_CHECK(++check_it == l.end());
}

BOOST_AUTO_TEST_CASE(copying_constructor)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  List< int > r = l;

  auto itl = l.begin();
  auto itr = r.begin();

  for (size_t i = 0; i < 3; ++i) {
    BOOST_CHECK_EQUAL(*(itl++), *(itr++));
  }

  BOOST_CHECK(itl == l.end());
  BOOST_CHECK(itr == r.end());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  List< int > r = std::move(l);

  auto itr = r.begin();
  BOOST_CHECK_EQUAL(*(itr++), 1);
  BOOST_CHECK_EQUAL(*(itr++), 2);
  BOOST_CHECK_EQUAL(*(itr++), 3);
  BOOST_CHECK(itr == r.end());
}
BOOST_AUTO_TEST_CASE(copy_assignment)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  List< int > r;
  r.pushBack(100);
  r = l;

  auto itl = l.begin();
  BOOST_CHECK_EQUAL(*(itl++), 1);
  BOOST_CHECK_EQUAL(*(itl++), 2);
  BOOST_CHECK_EQUAL(*(itl++), 3);

  auto itr = r.begin();
  BOOST_CHECK_EQUAL(*(itr++), 1);
  BOOST_CHECK_EQUAL(*(itr++), 2);
  BOOST_CHECK_EQUAL(*(itr++), 3);
  BOOST_CHECK(itr == r.end());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  List< int > r;
  r.pushBack(100);

  r = std::move(l);

  auto itr = r.begin();
  BOOST_CHECK_EQUAL(*(itr++), 1);
  BOOST_CHECK_EQUAL(*(itr++), 2);
  BOOST_CHECK_EQUAL(*(itr++), 3);
  BOOST_CHECK(itr == r.end());
}

BOOST_AUTO_TEST_CASE(clear_and_init_test)
{
  List< int > l;
  l.pushBack(1);
  l.pushBack(2);
  l.pushBack(3);

  l.clearAndInit(5, 42);

  size_t count = 0;
  for (auto it = l.begin(); it != l.end(); ++it) {
    BOOST_CHECK_EQUAL(*it, 42);
    count++;
  }

  BOOST_CHECK_EQUAL(count, 5);

  l.clearAndInit(0, 100);
  BOOST_CHECK(l.begin() == l.end());
}
