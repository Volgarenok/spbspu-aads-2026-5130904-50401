#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(rizatdinov_list_tests)

BOOST_AUTO_TEST_CASE(empty_list)
{
  rizatdinov::List<int> list;

  BOOST_CHECK(list.is_empty());
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(initializer_list_constructor)
{
  rizatdinov::List<int> list{1, 2, 3, 4};

  auto it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;

  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(push_front_order)
{
  rizatdinov::List<int> list;

  list.push_front(10);
  list.push_front(20);
  list.push_front(30);

  auto it = list.begin();

  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  rizatdinov::List<int> list;

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  list.pop_front();

  BOOST_CHECK_EQUAL(*list.begin(), 2);

  list.pop_front();
  BOOST_CHECK_EQUAL(*list.begin(), 1);

  list.pop_front();

  BOOST_CHECK(list.is_empty());
}

BOOST_AUTO_TEST_CASE(insert_after_beginning)
{
  rizatdinov::List<int> list;

  list.push_front(5);

  auto before = list.before_begin();

  auto inserted = list.insert_after(before, 10);

  BOOST_CHECK_EQUAL(*inserted, 10);
  BOOST_CHECK_EQUAL(*list.begin(), 10);

  ++inserted;
  BOOST_CHECK_EQUAL(*inserted, 5);
}

BOOST_AUTO_TEST_CASE(insert_after_middle)
{
  rizatdinov::List<int> list{1, 3};

  auto it = list.begin();

  list.insert_after(it, 2);

  BOOST_CHECK_EQUAL(*it, 1);

  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(erase_after_test)
{
  rizatdinov::List<int> list{1, 2, 3};

  auto it = list.before_begin();

  list.erase_after(it);

  auto cur = list.begin();

  BOOST_CHECK_EQUAL(*cur, 2);
  ++cur;
  BOOST_CHECK_EQUAL(*cur, 3);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  rizatdinov::List<int> list;

  for (int i = 0; i < 20; ++i) {
    list.push_front(i);
  }

  list.clear();

  BOOST_CHECK(list.is_empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  rizatdinov::List<int> original{1, 2, 3};
  rizatdinov::List<int> copy(original);

  auto a = original.begin();
  auto b = copy.begin();

  while (a != original.end()) {
    BOOST_CHECK_EQUAL(*a, *b);
    ++a;
    ++b;
  }

  BOOST_CHECK(b == copy.end());
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  rizatdinov::List<int> first{4, 5, 6};
  rizatdinov::List<int> second;

  second = first;

  auto a = first.begin();
  auto b = second.begin();

  while (a != first.end()) {
    BOOST_CHECK_EQUAL(*a, *b);
    ++a;
    ++b;
  }

  BOOST_CHECK(b == second.end());
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  rizatdinov::List<int> source{7, 8, 9};

  rizatdinov::List<int> moved(std::move(source));

  BOOST_CHECK(source.is_empty());

  auto it = moved.begin();

  BOOST_CHECK_EQUAL(*it, 7);
  ++it;
  BOOST_CHECK_EQUAL(*it, 8);
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  rizatdinov::List<int> first{100, 200};
  rizatdinov::List<int> second;

  second = std::move(first);

  BOOST_CHECK(first.is_empty());

  auto it = second.begin();

  BOOST_CHECK_EQUAL(*it, 100);
  ++it;
  BOOST_CHECK_EQUAL(*it, 200);
}

BOOST_AUTO_TEST_CASE(iterator_modification_test)
{
  rizatdinov::List<int> list{1, 2, 3};

  auto it = list.begin();

  *it = 10;

  BOOST_CHECK_EQUAL(*list.begin(), 10);
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  rizatdinov::List<int> list{5, 6, 7};

  const rizatdinov::List<int>& const_list = list;

  auto it = const_list.begin();

  BOOST_CHECK_EQUAL(*it, 5);

  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
}

BOOST_AUTO_TEST_CASE(self_assignment_test)
{
  rizatdinov::List<int> list{1, 2, 3};

  list = list;

  auto it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_SUITE_END()


