#include "list.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(push_front_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  LIter<int> it = list.begin();
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
}
BOOST_AUTO_TEST_CASE(front_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  BOOST_CHECK(list.front() == 3);
}
BOOST_AUTO_TEST_CASE(front_reference_test)
{
  List<int> list;
  list.push_front(1);
  list.front() = 10;
  BOOST_CHECK(list.front() == 10);
}
BOOST_AUTO_TEST_CASE(begin_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  LIter<int> it = list.begin();
  BOOST_CHECK(*it == 2);
}
BOOST_AUTO_TEST_CASE(iterator_increment_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  LIter<int> it = list.begin();
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
}
BOOST_AUTO_TEST_CASE(pop_front)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  list.pop_front();
  LIter<int> it = list.begin();
  BOOST_CHECK(*it == 2);
}
BOOST_AUTO_TEST_CASE(insert_after_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  LIter<int> it = list.begin();
  list.insert_after(it,5);
  ++it;
  BOOST_CHECK(*it == 5);
}
BOOST_AUTO_TEST_CASE(erase_after_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  LIter<int> it = list.begin();
  list.erase_after(it);
  ++it;
  BOOST_CHECK(*it == 1);
}
BOOST_AUTO_TEST_CASE(iterator_traverse_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  int sum = 0;
  for(LIter<int> it = list.begin(); it != list.end(); ++it)
  {
    sum += *it;
  }
  BOOST_CHECK(sum == 6);
}
BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  List<int> list;
  list.push_front(1);
  list.push_front(2);
  const List<int>& clist = list;
  LCIter<int> it = clist.cbegin();
  BOOST_CHECK(*it == 2);
}