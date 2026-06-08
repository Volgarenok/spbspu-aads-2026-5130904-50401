#include "list.hpp"
#include <functional>
#include <utility>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(empty_test)
{
  zhuravleva::List< int > a;

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(size_empty_test)
{
  zhuravleva::List< int > a;

  BOOST_CHECK(a.size() == 0);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  zhuravleva::List< int > a;

  a.pushFront(1);
  a.pushFront(2);

  zhuravleva::LIter< int > it = a.begin();

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  zhuravleva::List< int > a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::LIter< int > it = a.begin();

  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  zhuravleva::List< int > a;

  a.pushFront(1);
  a.pushFront(2);

  a.popFront();

  BOOST_CHECK(*a.begin() == 1);
}

BOOST_AUTO_TEST_CASE(pop_front_empty_test)
{
  zhuravleva::List< int > a;

  a.popFront();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
  zhuravleva::List< int > a;

  a.pushBack(1);
  a.pushBack(2);

  a.popBack();

  BOOST_CHECK(a.size() == 1);
  BOOST_CHECK(*a.begin() == 1);
}

BOOST_AUTO_TEST_CASE(pop_back_empty_test)
{
  zhuravleva::List< int > a;

  a.popBack();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(insert_after_test)
{
  zhuravleva::List< int > a;

  auto it = a.pushFront(1);

  a.insertAfter(it, 2);

  ++it;

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(erase_after_test)
{
  zhuravleva::List< int > a;

  a.pushFront(2);
  a.pushFront(1);

  auto it = a.begin();

  a.eraseAfter(it);

  BOOST_CHECK(*it == 1);
  BOOST_CHECK(a.size() == 1);
}

BOOST_AUTO_TEST_CASE(erase_after_empty_test)
{
  zhuravleva::List< int > a;

  auto it = a.beforeBegin();

  a.eraseAfter(it);

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  zhuravleva::List< int > a;

  a.pushFront(1);
  a.pushFront(2);

  a.clear();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(clear_empty_test)
{
  zhuravleva::List< int > a;

  a.clear();

  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  zhuravleva::List< int > a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List< int > b(a);

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  zhuravleva::List< int > a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List< int > b;

  b.pushBack(9);
  b = a;

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  zhuravleva::List< int > a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List< int > b(std::move(a));

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  zhuravleva::List< int > a;

  a.pushBack(1);
  a.pushBack(2);

  zhuravleva::List< int > b;

  b.pushBack(9);
  b = std::move(a);

  BOOST_CHECK(b.size() == 2);
  BOOST_CHECK(*b.begin() == 1);
  BOOST_CHECK(a.empty());
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  zhuravleva::List< int > a;

  a.pushBack(1);

  const zhuravleva::List< int >& ref = a;
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

BOOST_AUTO_TEST_CASE(merge_sorted_lists_test)
{
  zhuravleva::List< int > first;
  first.pushBack(1);
  first.pushBack(3);
  first.pushBack(5);
  zhuravleva::List< int > second;
  second.pushBack(2);
  second.pushBack(4);
  second.pushBack(6);
  first.merge(second, std::less< int >());

  BOOST_CHECK(second.empty());
  BOOST_CHECK(first.size() == 6);

  zhuravleva::LIter< int > it = first.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 4);
  ++it;
  BOOST_CHECK(*it == 5);
  ++it;
  BOOST_CHECK(*it == 6);
}

BOOST_AUTO_TEST_CASE(merge_before_first_element_test)
{
  zhuravleva::List< int > first;
  first.pushBack(4);
  first.pushBack(5);
  zhuravleva::List< int > second;
  second.pushBack(1);
  second.pushBack(2);
  second.pushBack(3);
  first.merge(second, std::less< int >());
  BOOST_CHECK(second.empty());
  BOOST_CHECK(first.size() == 5);
  zhuravleva::LIter< int > it = first.begin();

  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 4);
  ++it;
  BOOST_CHECK(*it == 5);
}

BOOST_AUTO_TEST_CASE(sort_test)
{
  zhuravleva::List< int > list;
  list.pushBack(1);
  list.pushBack(5);
  list.pushBack(3);
  list.pushBack(4);
  list.pushBack(2);
  list.sort(std::less< int >());
  zhuravleva::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 4);
  ++it;
  BOOST_CHECK(*it == 5);
}

BOOST_AUTO_TEST_CASE(sort_empty_test)
{
  zhuravleva::List< int > list;
  list.sort(std::less< int >());
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(sort_one_element_test)
{
  zhuravleva::List< int > list;
  list.pushBack(10);
  list.sort(std::less< int >());
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(*list.begin() == 10);
}

bool isLessThanFour(int value)
{
  return value < 4;
}

BOOST_AUTO_TEST_CASE(partition_test)
{
  zhuravleva::List< int > list;
  list.pushBack(1);
  list.pushBack(5);
  list.pushBack(3);
  list.pushBack(4);
  list.pushBack(2);
  list.partition(isLessThanFour);
  zhuravleva::LIter< int > it = list.begin();

  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 5);
  ++it;
  BOOST_CHECK(*it == 4);
}

BOOST_AUTO_TEST_CASE(partition_all_selected_test)
{
  zhuravleva::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.partition(isLessThanFour);
  BOOST_CHECK(list.size() == 3);
  zhuravleva::LIter< int > it = list.begin();

  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
}

BOOST_AUTO_TEST_CASE(partition_none_selected_test)
{
  zhuravleva::List< int > list;
  list.pushBack(5);
  list.pushBack(6);
  list.pushBack(7);
  list.partition(isLessThanFour);
  BOOST_CHECK(list.size() == 3);
  zhuravleva::LIter< int > it = list.begin();

  BOOST_CHECK(*it == 5);
  ++it;
  BOOST_CHECK(*it == 6);
  ++it;
  BOOST_CHECK(*it == 7);
}
