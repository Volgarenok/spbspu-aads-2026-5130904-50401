#include <stdexcept>
#include <utility>
#include <boost/test/unit_test.hpp>
#include "list.hpp"


BOOST_AUTO_TEST_CASE(push_front_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
}
BOOST_AUTO_TEST_CASE(front_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  BOOST_CHECK(list.front() == 3);
}
BOOST_AUTO_TEST_CASE(front_reference_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.front() = 10;
  BOOST_CHECK(list.front() == 10);
}
BOOST_AUTO_TEST_CASE(begin_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 2);
}
BOOST_AUTO_TEST_CASE(iterator_increment_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
}
BOOST_AUTO_TEST_CASE(pop_front)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  list.pop_front();
  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 2);
}
BOOST_AUTO_TEST_CASE(push_back_basic_test)
{
  ulanova::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
}
BOOST_AUTO_TEST_CASE(insert_after_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  ulanova::LIter< int > it = list.begin();
  list.insert_after(it,5);
  ++it;
  BOOST_CHECK(*it == 5);
}
BOOST_AUTO_TEST_CASE(erase_after_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  ulanova::LIter< int > it = list.begin();
  list.erase_after(it);
  ++it;
  BOOST_CHECK(*it == 1);
}
BOOST_AUTO_TEST_CASE(iterator_traverse_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  int sum = 0;
  for (ulanova::LIter< int > it = list.begin(); it != list.end(); ++it)
  {
    sum += *it;
  }
  BOOST_CHECK(sum == 6);
}
BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  ulanova::List< int > list;
  list.push_front(1);
  list.push_front(2);
  const ulanova::List< int >& clist = list;
  ulanova::LCIter< int > it = clist.cbegin();
  BOOST_CHECK(*it == 2);
}
BOOST_AUTO_TEST_CASE(empty_list_iterators_test)
{
  ulanova::List< int > list;
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(pop_front_empty_test)
{
  ulanova::List< int > list;
  list.pop_front();
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  ulanova::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.clear();
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  ulanova::List< int > source;
  source.push_back(1);
  source.push_back(2);

  ulanova::List< int > copy(source);
  source.front() = 10;

  ulanova::LIter< int > it = copy.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  ulanova::List< int > source;
  source.push_back(1);
  source.push_back(2);

  ulanova::List< int > copy;
  copy.push_back(9);
  copy = source;

  ulanova::LIter< int > it = copy.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  ulanova::List< int > source;
  source.push_back(1);
  source.push_back(2);

  ulanova::List< int > moved(std::move(source));

  ulanova::LIter< int > it = moved.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(source.begin() == source.end());
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  ulanova::List< int > source;
  source.push_back(1);
  source.push_back(2);

  ulanova::List< int > moved;
  moved.push_back(9);
  moved = std::move(source);

  ulanova::LIter< int > it = moved.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(source.begin() == source.end());
}

BOOST_AUTO_TEST_CASE(postfix_increment_test)
{
  ulanova::List< int > list;
  list.push_back(1);
  list.push_back(2);

  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*(it++) == 1);
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  ulanova::List< int > first;
  first.push_back(1);
  first.push_back(2);

  ulanova::List< int > second;
  second.push_back(3);

  first.swap(second);

  ulanova::LIter< int > first_it = first.begin();
  BOOST_CHECK(*first_it == 3);
  ++first_it;
  BOOST_CHECK(first_it == first.end());

  ulanova::LIter< int > second_it = second.begin();
  BOOST_CHECK(*second_it == 1);
  ++second_it;
  BOOST_CHECK(*second_it == 2);
}

BOOST_AUTO_TEST_CASE(pop_front_single_element_test)
{
  ulanova::List< int > list;
  list.push_back(1);
  list.pop_front();

  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(erase_after_end_test)
{
  ulanova::List< int > list;
  list.push_back(1);

  list.erase_after(list.end());

  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 1);
}

BOOST_AUTO_TEST_CASE(erase_after_wraps_to_head_test)
{
  ulanova::List< int > list;
  list.push_back(1);
  list.push_back(2);

  ulanova::LIter< int > it = list.begin();
  ++it;

  list.erase_after(it);

  BOOST_CHECK(list.front() == 2);

  ulanova::LIter< int > check_it = list.begin();
  BOOST_CHECK(*check_it == 2);
  ++check_it;
  BOOST_CHECK(check_it == list.end());
}

BOOST_AUTO_TEST_CASE(insert_after_end_test)
{
  ulanova::List< int > list;
  list.push_back(1);

  ulanova::LIter< int > result = list.insert_after(list.end(), 2);

  BOOST_CHECK(result == list.end());
  BOOST_CHECK(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(increment_end_iterator_test)
{
  ulanova::List< int > list;
  ulanova::LIter< int > it = list.end();

  ++it;

  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(front_empty_throws_test)
{
  ulanova::List< int > list;

  BOOST_CHECK_THROW(list.front(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(splice_after_all_test)
{
  ulanova::List< int > first;
  first.push_back(1);
  first.push_back(4);

  ulanova::List< int > second;
  second.push_back(2);
  second.push_back(3);

  first.splice_after(first.begin(), second);

  ulanova::LIter< int > it = first.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 4);
  ++it;
  BOOST_CHECK(it == first.end());

  BOOST_CHECK(second.begin() == second.end());
}

BOOST_AUTO_TEST_CASE(splice_after_one_test)
{
  ulanova::List< int > first;
  first.push_back(1);
  first.push_back(4);

  ulanova::List< int > second;
  second.push_back(2);
  second.push_back(3);

  ulanova::LIter< int > before = second.begin();

  first.splice_after(first.begin(), second, before);

  ulanova::LIter< int > first_it = first.begin();
  BOOST_CHECK(*first_it == 1);
  ++first_it;
  BOOST_CHECK(*first_it == 3);
  ++first_it;
  BOOST_CHECK(*first_it == 4);

  ulanova::LIter< int > second_it = second.begin();
  BOOST_CHECK(*second_it == 2);
  ++second_it;
  BOOST_CHECK(second_it == second.end());
}

BOOST_AUTO_TEST_CASE(splice_after_range_test)
{
  ulanova::List< int > first;
  first.push_back(1);
  first.push_back(5);

  ulanova::List< int > second;
  second.push_back(2);
  second.push_back(3);
  second.push_back(4);
  second.push_back(6);

  ulanova::LIter< int > before_first = second.begin();
  ulanova::LIter< int > before_last = second.begin();
  ++before_last;
  ++before_last;
  ++before_last;

  first.splice_after(first.begin(), second, before_first, before_last);

  ulanova::LIter< int > first_it = first.begin();
  BOOST_CHECK(*first_it == 1);
  ++first_it;
  BOOST_CHECK(*first_it == 3);
  ++first_it;
  BOOST_CHECK(*first_it == 4);
  ++first_it;
  BOOST_CHECK(*first_it == 5);
  ++first_it;
  BOOST_CHECK(first_it == first.end());

  ulanova::LIter< int > second_it = second.begin();
  BOOST_CHECK(*second_it == 2);
  ++second_it;
  BOOST_CHECK(*second_it == 6);
  ++second_it;
  BOOST_CHECK(second_it == second.end());
}

BOOST_AUTO_TEST_CASE(sort_test)
{
  ulanova::List< int > list;
  list.push_back(3);
  list.push_back(1);
  list.push_back(2);

  list.sort();

  ulanova::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(sort_empty_test)
{
  ulanova::List< int > list;

  list.sort();

  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(sort_single_element_test)
{
  ulanova::List< int > list;
  list.push_back(1);

  list.sort();

  BOOST_CHECK(list.front() == 1);
}
