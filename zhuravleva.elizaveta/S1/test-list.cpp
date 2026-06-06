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

BOOST_AUTO_TEST_CASE(spliceAfter_one_element_test)
{
  zhuravleva::List< int > first;
  first.pushBack(1);
  first.pushBack(2);
  first.pushBack(3);
  zhuravleva::List< int > second;
  second.pushBack(10);
  second.pushBack(20);
  second.pushBack(30);
  zhuravleva::LIter< int > pos = first.begin();
  zhuravleva::LIter< int > beforeElement = second.begin();
  first.spliceAfter(pos, second, beforeElement);
  zhuravleva::LIter< int > firstIt = first.begin();

  BOOST_CHECK(*firstIt == 1);
  ++firstIt;
  BOOST_CHECK(*firstIt == 20);
  ++firstIt;
  BOOST_CHECK(*firstIt == 2);
  ++firstIt;
  BOOST_CHECK(*firstIt == 3);

  zhuravleva::LIter< int > secondIt = second.begin();
  BOOST_CHECK(*secondIt == 10);
  ++secondIt;
  BOOST_CHECK(*secondIt == 30);
}

BOOST_AUTO_TEST_CASE(spliceAfter_to_empty_list_test)
{
  zhuravleva::List< int > first;
  zhuravleva::List< int > second;
  second.pushBack(10);
  second.pushBack(20);
  zhuravleva::LIter< int > pos = first.beforeBegin();
  zhuravleva::LIter< int > beforeElement = second.beforeBegin();

  first.spliceAfter(pos, second, beforeElement);
  BOOST_CHECK(first.size() == 1);
  BOOST_CHECK(second.size() == 1);
  BOOST_CHECK(*first.begin() == 10);
  BOOST_CHECK(*second.begin() == 20);
}

BOOST_AUTO_TEST_CASE(spliceAfter_no_element_test)
{
  zhuravleva::List< int > first;
  first.pushBack(1);
  zhuravleva::List< int > second;
  second.pushBack(10);
  zhuravleva::LIter< int > pos = first.begin();
  zhuravleva::LIter< int > beforeElement = second.begin();
  first.spliceAfter(pos, second, beforeElement);
  BOOST_CHECK(first.size() == 1);
  BOOST_CHECK(second.size() == 1);
}

BOOST_AUTO_TEST_CASE(splice_after_full_list_test)
{
  zhuravleva::List< int > first;
  first.pushBack(1);
  first.pushBack(2);
  zhuravleva::List< int > second;
  second.pushBack(10);
  second.pushBack(20);
  second.pushBack(30);
  first.spliceAfter(first.begin(), second);

  BOOST_CHECK(second.empty());
  BOOST_CHECK(first.size() == 5);
  zhuravleva::LIter< int > it = first.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(*it == 2);
}
