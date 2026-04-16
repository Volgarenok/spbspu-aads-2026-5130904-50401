#define BOOST_TEST_MODULE ListDeepTest
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTestForLab)

BOOST_AUTO_TEST_CASE(test_iter)
{
  petrov::List<int> l;
  l.push_back(1);

  petrov::LIter<int> i = l.begin();
  BOOST_CHECK_EQUAL(*i, 1);
}

BOOST_AUTO_TEST_CASE(test_isempty)
{
  petrov::List<int> c;

  BOOST_CHECK_EQUAL(1, c.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_empty_plus_elems)
{
  petrov::List<int> c;
  c.push_back(5);

  BOOST_CHECK(!c.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  petrov::List<int> c;
  c.push_front(73);
  c.push_front(37);

  petrov::LIter<int> i = c.begin();
  BOOST_CHECK_EQUAL(*i, 37);
  BOOST_CHECK_EQUAL(*(++i), 73);
}

BOOST_AUTO_TEST_CASE(test_iter_equal)
{
  petrov::List<int> c;
  c.push_back(11);

  petrov::LIter<int> i = c.begin();
  petrov::LIter<int> i2 = c.begin();
  BOOST_CHECK(i == i2);
}

BOOST_AUTO_TEST_CASE(test_iter_not_eq)
{
  petrov::List<int> c;
  c.push_back(11);
  c.push_back(73);

  petrov::LIter<int> i1 = c.begin();
  petrov::LIter<int> i2 = c.begin();
  ++i2;
  BOOST_CHECK(i1 != i2);
}

BOOST_AUTO_TEST_CASE(test_minus_plus)
{
  petrov::List<int> c;
  c.push_back(1);
  c.push_back(2);

  petrov::LIter<int> i = c.begin();
  petrov::LIter<int> i2 = c.begin();
  ++i2;
  BOOST_CHECK(i == --i2);
}

BOOST_AUTO_TEST_CASE(test_nullptr)
{
  petrov::List<int> c;
  c.push_back(1);

  BOOST_CHECK(c.end() == nullptr);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  petrov::List<int> i1;
  i1.push_back(10);
  i1.push_back(20);

  petrov::List<int> i2(i1);
  BOOST_CHECK_EQUAL(i1.IsEmpty(), i2.IsEmpty());

  petrov::LIter<int> l1 = i1.begin();
  petrov::LIter<int> l2 = i2.begin();
  BOOST_CHECK_EQUAL(*l1, *l2);;
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  petrov::List<int> l1;
  l1.push_back(1);
  petrov::List<int> l2;
  l2.push_back(2);

  l2 = l1;
  BOOST_CHECK_EQUAL(*l2.begin(), 1);

  l1.clear();
  BOOST_CHECK(!l2.IsEmpty());
  BOOST_CHECK_EQUAL(*l2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(test_move_semantics)
{
  petrov::List<int> l1;
  l1.push_back(100);
  l1.push_back(200);

  petrov::List<int> l2(std::move(l1));
  BOOST_CHECK(l1.IsEmpty());
  BOOST_CHECK_EQUAL(*l2.begin(), 100);

  petrov::LIter<int> i = l2.begin();
  BOOST_CHECK_EQUAL(*(++i), 200);
}

BOOST_AUTO_TEST_CASE(test_string_list)
{
  petrov::List<std::string> l;
  l.push_back("ab");
  l.push_back("ba");

  petrov::LIter<std::string> i = l.begin();
  BOOST_CHECK_EQUAL(*i, "ab");
  BOOST_CHECK_EQUAL(*(++i), "ba");
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  petrov::List<int> l;
  l.push_back(1);
  l.push_back(2);
  l.pop_back();

  BOOST_CHECK_EQUAL(*l.begin(), 1);
  petrov::LIter<int> it = l.begin();
  BOOST_CHECK(++it == l.end());
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  petrov::List<int> l;
  l.push_back(10);
  l.push_back(20);
  l.pop_front();

  BOOST_CHECK_EQUAL(*l.begin(), 20);
  BOOST_CHECK(!l.IsEmpty());
  l.pop_front();
  BOOST_CHECK(l.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_clear_logic)
{
  petrov::List<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.clear();

  BOOST_CHECK(l.IsEmpty());
  BOOST_CHECK(l.begin() == l.end());
}

BOOST_AUTO_TEST_CASE(test_arrow_operator)
{
  struct Point { int x, y; };
  petrov::List<Point> l;
  Point p = {10, 20};
  l.push_back(p);

  petrov::LIter<Point> it = l.begin();
  BOOST_CHECK_EQUAL(it->x, 10);
  BOOST_CHECK_EQUAL(it->y, 20);
}

BOOST_AUTO_TEST_CASE(test_self_assignment)
{
  petrov::List<int> l;
  l.push_back(42);
  l = l;

  BOOST_CHECK_EQUAL(*l.begin(), 42);
  BOOST_CHECK(!l.IsEmpty());
}

BOOST_AUTO_TEST_CASE(test_post_increment)
{
  petrov::List<int> l;
  l.push_back(1);
  l.push_back(2);

  petrov::LIter<int> i = l.begin();
  petrov::LIter<int> j = i++;

  BOOST_CHECK_EQUAL(*j, 1);
  BOOST_CHECK_EQUAL(*i, 2);
}

BOOST_AUTO_TEST_SUITE_END()
