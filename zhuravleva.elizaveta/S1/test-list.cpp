#include "list.hpp"
#include <boost/test/unit_test.hpp>
#include <utility>

BOOST_AUTO_TEST_CASE(empty_test)
{
  zhuravleva::List<int> a;

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(size_empty_test)
{
  zhuravleva::List<int> a;

  BOOST_CHECK(a.size() == 0);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  zhuravleva::List<int> a;

  a.pushFront(1);
  a.pushFront(2);

  zhuravleva::LIter<int> it = a.begin();

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  zhuravleva::List<int> a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::LIter<int> it = a.begin();

  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  zhuravleva::List<int> a;

  a.pushFront(1);
  a.pushFront(2);

  a.popFront();

  BOOST_CHECK(*a.begin() == 1);
}

BOOST_AUTO_TEST_CASE(pop_front_empty_test)
{
  zhuravleva::List<int> a;

  a.popFront();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
  zhuravleva::List<int> a;

  a.pushBack(1);
  a.pushBack(2);

  a.popBack();

  BOOST_CHECK(a.size() == 1);
  BOOST_CHECK(*a.begin() == 1);
}

BOOST_AUTO_TEST_CASE(pop_back_empty_test)
{
  zhuravleva::List<int> a;

  a.popBack();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(insert_after_test)
{
  zhuravleva::List<int> a;

  auto it = a.pushFront(1);

  a.insertAfter(it, 2);

  ++it;

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(erase_after_test)
{
  zhuravleva::List<int> a;

  a.pushFront(2);
  a.pushFront(1);

  auto it = a.begin();

  a.eraseAfter(it);

  BOOST_CHECK(*it == 1);
  BOOST_CHECK(a.size() == 1);
}

BOOST_AUTO_TEST_CASE(erase_after_empty_test)
{
  zhuravleva::List<int> a;

  auto it = a.beforeBegin();

  a.eraseAfter(it);

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  zhuravleva::List<int> a;

  a.pushFront(1);
  a.pushFront(2);

  a.clear();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(clear_empty_test)
{
  zhuravleva::List<int> a;

  a.clear();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  zhuravleva::List<int> a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List<int> b(a);

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  zhuravleva::List<int> a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List<int> b;

  b.pushBack(9);
  b = a;

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  zhuravleva::List<int> a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List<int> b(std::move(a));

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  zhuravleva::List<int> a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List<int> b;

  b.pushBack(9);
  b = std::move(a);

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  zhuravleva::List<int> a;

  a.pushBack(1);

  const zhuravleva::List<int>& ref = a;
  auto it = ref.cbegin();

  BOOST_CHECK(*it == 1);
}