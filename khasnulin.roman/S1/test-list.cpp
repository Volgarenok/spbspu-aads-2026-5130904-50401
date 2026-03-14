#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <stdexcept>

#include "List.hpp"

BOOST_AUTO_TEST_SUITE(list_suite)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_create_empty_list)
{
  BiList< int > list;

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_add_element)
{
  BiList< int > list;
  list.push_back(1);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 1);
}

BOOST_AUTO_TEST_CASE(test_front_and_begin_iter_value_equal)
{
  BiList< int > list;
  list.push_back(1);

  LIter< int > it = list.begin();

  BOOST_CHECK_EQUAL(list.front(), *it);
}

BOOST_AUTO_TEST_CASE(test_iters_moving)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);

  LIter< int > it = list.begin();

  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  --it;
  BOOST_CHECK_EQUAL(*it, 2);
  --it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_list_copy_constructor)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  BiList< int > listCP(list);

  BOOST_CHECK_EQUAL(listCP.front(), 1);
  BOOST_CHECK_EQUAL(listCP.back(), 2);
}

BOOST_AUTO_TEST_CASE(test_list_move_constructor)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);

  BiList< int > listCP(std::move(list));

  BOOST_CHECK_EQUAL(listCP.front(), 1);
  BOOST_CHECK_EQUAL(listCP.back(), 2);

  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_list_self_assignment)
{
  BiList< int > list;
  list.push_back(1);

  list = list;

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 1);
}

BOOST_AUTO_TEST_CASE(test_item_erase)
{
  BiList< int > list;
  list.push_back(1);

  list.push_back(3);

  auto it = list.begin();
  it++;
  it = list.insert(it, 2);

  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);

  it = list.erase(it);
  BOOST_CHECK_EQUAL(*it, 3);
  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);
}

BOOST_AUTO_TEST_CASE(test_list_clear)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 3);

  list.clear();

  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK_THROW(list.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_erase_all_from_end)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto it = list.end();
  --it;
  it = list.erase(it);
  BOOST_CHECK_EQUAL(it, list.end());
  --it;
  it = list.erase(it);
  BOOST_CHECK_EQUAL(it, list.end());
  --it;
  it = list.erase(it);
  BOOST_CHECK_EQUAL(it, list.end());

  BOOST_CHECK_THROW(list.erase(it), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_erase_single_element)
{
  BiList< int > list;
  list.push_back(10);
  list.erase(list.begin());

  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(test_erase_range_middle)
{
  BiList< int > list;
  for (int i = 1; i <= 5; ++i)
  {
    list.push_back(i);
  }

  auto it1 = list.begin();
  ++it1;
  ++it1;

  auto it2 = it1;
  ++it2;
  ++it2;

  auto next_it = list.erase(it1, it2);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(*next_it, 5);
  BOOST_CHECK_EQUAL(list.front(), 1);
  BOOST_CHECK_EQUAL(list.back(), 5);
}

BOOST_AUTO_TEST_CASE(test_erase_range_full)
{
  BiList< int > list;
  list.push_back(10);
  list.push_back(20);

  auto it = list.erase(list.begin(), list.end());

  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(test_erase_range_at_start)
{
  BiList< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto it_end = list.begin();
  ++it_end;
  ++it_end;

  auto it = list.erase(list.begin(), it_end);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 3);
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(test_erase_range_empty)
{
  BiList< int > list;
  list.push_back(1);

  auto it = list.erase(list.begin(), list.begin());

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK(it == list.begin());
}

BOOST_AUTO_TEST_CASE(push_front_empty)
{
  BiList< int > list;
  list.push_front(10);

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(list.front(), 10);
  BOOST_CHECK_EQUAL(list.back(), 10);
}

BOOST_AUTO_TEST_CASE(push_front_multiple)
{
  BiList< int > list;
  list.push_front(10);
  list.push_front(20);

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK_EQUAL(list.front(), 20);
  BOOST_CHECK_EQUAL(list.back(), 10);

  list.push_front(30);

  BOOST_CHECK_EQUAL(list.size(), 3);
  BOOST_CHECK_EQUAL(list.front(), 30);
  BOOST_CHECK_EQUAL(list.back(), 10);

  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(push_front_rvalue_move)
{
  khasnulin::BiList< std::string > list;
  std::string s = "hello";
  list.push_front(std::move(s));

  BOOST_CHECK_EQUAL(list.front(), "hello");
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_SUITE_END()
