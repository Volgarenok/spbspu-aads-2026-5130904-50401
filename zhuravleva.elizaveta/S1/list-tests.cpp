#include "list.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(empty_test)
{
  zhuravleva::List<int> a;

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(add_start_test)
{
  zhuravleva::List<int> a;

  a.AddStart(1);
  a.AddStart(2);

  zhuravleva::LIter<int> it = a.begin();

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(delete_start_test)
{
  zhuravleva::List<int> a;

  a.AddStart(1);
  a.AddStart(2);

  a.deleteStart();

  BOOST_CHECK(*a.begin() == 1);
}

BOOST_AUTO_TEST_CASE(add_after_test)
{
  zhuravleva::List<int> a;

  auto it = a.AddStart(1);

  a.addAfter(it, 2);

  it = it.next();

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(delete_after_test)
{
  zhuravleva::List<int> a;

  auto it = a.AddStart(1);
  a.addAfter(it, 2);

  a.deleteAfter(it);

  BOOST_CHECK(it.next() == a.end());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  zhuravleva::List<int> a;

  a.AddStart(1);
  a.AddStart(2);

  a.clear();

  BOOST_CHECK(a.empty());
}

