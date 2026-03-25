#include "list.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(list_tests)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  zubarev::List< size_t > list;

  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  zubarev::List< size_t > list;

  list.push_front(10);
  list.push_front(20);
  list.push_front(30);

  auto it = list.begin();

  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 10);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  list.pop_front();

  auto it = list.begin();
  BOOST_CHECK(*it == 2);

  list.pop_front();
  BOOST_CHECK(*list.begin() == 1);

  list.pop_front();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  zubarev::List< size_t > list;

  for (size_t i = 0; i < 10; ++i) {
    list.push_front(i);
  }

  list.clear();

  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(insert_after_test)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(3);

  auto it = list.before_begin();
  list.insert_after(it, 5);

  it = list.begin();

  BOOST_CHECK(*it == 5);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 1);
}

BOOST_AUTO_TEST_CASE(erase_after_test)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  auto it = list.before_begin();
  list.erase_after(it);

  it = list.begin();

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  zubarev::List< size_t > copy(list);

  auto it1 = list.begin();
  auto it2 = copy.begin();

  while (it1 != list.end()) {
    BOOST_CHECK(*it1 == *it2);
    ++it1;
    ++it2;
  }

  BOOST_CHECK(it2 == copy.end());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);

  zubarev::List< size_t > moved(std::move(list));

  BOOST_CHECK(list.empty());

  auto it = moved.begin();
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  zubarev::List< size_t > list1;
  list1.push_front(1);
  list1.push_front(2);

  zubarev::List< size_t > list2;
  list2 = list1;

  auto it1 = list1.begin();
  auto it2 = list2.begin();

  while (it1 != list1.end()) {
    BOOST_CHECK(*it1 == *it2);
    ++it1;
    ++it2;
  }

  BOOST_CHECK(it2 == list2.end());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  zubarev::List< size_t > list1;
  list1.push_front(10);
  list1.push_front(20);

  zubarev::List< size_t > list2;
  list2 = std::move(list1);

  BOOST_CHECK(list1.empty());

  auto it = list2.begin();
  BOOST_CHECK(*it == 20);
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  size_t expected = 3;

  for (auto it = list.begin(); it != list.end(); ++it) {
    BOOST_CHECK(*it == expected);
    expected--;
  }
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);

  const zubarev::List< size_t >& clist = list;

  auto it = clist.begin();

  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
}

BOOST_AUTO_TEST_CASE(cbegin_test)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);

  auto it = list.cbegin();

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(self_assignment)
{
  zubarev::List< size_t > list;

  list.push_front(1);
  list.push_front(2);

  list = list;

  auto it = list.begin();
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()
