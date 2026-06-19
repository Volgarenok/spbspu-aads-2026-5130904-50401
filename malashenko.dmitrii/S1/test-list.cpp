#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(List_creation)
BOOST_AUTO_TEST_CASE(common_constructor)
{
  malashenko::List< int > l;
  BOOST_CHECK(l.empty());
  BOOST_CHECK(l.front() == l.back());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  malashenko::List< int > lCopy(l);
  malashenko::LIter< int > itL = l.begin();
  malashenko::LIter< int > itLCopy = lCopy.begin();
  for (;itL != l.end(); ++itL, ++itLCopy)
  {
    BOOST_CHECK(*itL == *itLCopy);
  }
  BOOST_CHECK(itLCopy == lCopy.end());
  BOOST_CHECK(l.size() == lCopy.size());
}


BOOST_AUTO_TEST_CASE(move_constructor)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  malashenko::List< int > tmpCopy(l);

  malashenko::List< int > lCopy(std::move(l));
  BOOST_CHECK(l.empty());

  malashenko::LIter< int > itL = tmpCopy.begin();
  malashenko::LIter< int > itLCopy = lCopy.begin();
  for (;itL != tmpCopy.end(); ++itL, ++itLCopy)
  {
    BOOST_CHECK(*itL == *itLCopy);
  }
  BOOST_CHECK(itLCopy == lCopy.end());
  BOOST_CHECK(tmpCopy.size() == lCopy.size());
}

BOOST_AUTO_TEST_CASE(copy_operator)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  malashenko::List< int > lCopy;
  lCopy = l;
  malashenko::LIter< int > itL = l.begin();
  malashenko::LIter< int > itLCopy = lCopy.begin();
  for (;itL != l.end(); ++itL, ++itLCopy)
  {
    BOOST_CHECK(*itL == *itLCopy);
  }
  BOOST_CHECK(itLCopy == lCopy.end());
  BOOST_CHECK(l.size() == lCopy.size());
}


BOOST_AUTO_TEST_CASE(move_operator)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  malashenko::List< int > tmpCopy(l);

  malashenko::List< int > lCopy;
  lCopy = std::move(l);
  BOOST_CHECK(l.empty());

  malashenko::LIter< int > itL = tmpCopy.begin();
  malashenko::LIter< int > itLCopy = lCopy.begin();
  for (;itL != tmpCopy.end(); ++itL, ++itLCopy)
  {
    BOOST_CHECK(*itL == *itLCopy);
  }
  BOOST_CHECK(itLCopy == lCopy.end());
  BOOST_CHECK(tmpCopy.size() == lCopy.size());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(List_iterators)
BOOST_AUTO_TEST_CASE(begin_and_end_with_empty_list)
{
  malashenko::List< int > l;
  BOOST_CHECK(l.begin() == l.end());
}

BOOST_AUTO_TEST_CASE(begin_and_end_with_non_empty_list)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  BOOST_CHECK(*(l.begin()) == 1);
  BOOST_CHECK(*(--l.end()) == 3);
}

BOOST_AUTO_TEST_CASE(traverse_forward)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  std::vector< int > res{1, 2, 3};
  std::vector< int > check;
  for (malashenko::LIter< int > it = l.begin(); it != l.end(); ++it)
  {
    check.push_back(*it);
  }

  BOOST_CHECK(res == check);
}


BOOST_AUTO_TEST_CASE(traverse_backward)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  std::vector< int > res{3, 2, 1};
  std::vector< int > check;
  for (malashenko::LIter< int > it = --l.end(); it != --l.begin(); --it)
  {
    check.push_back(*it);
  }

  BOOST_CHECK(res == check);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(List_getters)
BOOST_AUTO_TEST_CASE(front_and_end_with_empty_list)
{
  malashenko::List< int > l;
  BOOST_CHECK(l.front() == l.back());
}

BOOST_AUTO_TEST_CASE(front_and_end_with_non_empty_list)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 3);
}

BOOST_AUTO_TEST_CASE(size_of_list)
{
  malashenko::List< int > l;
  BOOST_CHECK(l.empty());
  l.push_back(1);
  BOOST_CHECK(l.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(List_adding_elements)

BOOST_AUTO_TEST_CASE(push_back_with_lvalue)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 3);
}

BOOST_AUTO_TEST_CASE(push_back_with_rvalue)
{
  malashenko::List< int > l;
  l.push_back(std::move(1));
  l.push_back(std::move(2));
  l.push_back(std::move(3));

  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 3);
}


BOOST_AUTO_TEST_CASE(push_front_with_lvalue)
{
  malashenko::List< int > l;
  l.push_front(1);
  l.push_front(2);
  l.push_front(3);

  BOOST_CHECK(l.front() == 3);
  BOOST_CHECK(l.back() == 1);
}


BOOST_AUTO_TEST_CASE(push_front_with_rvalue)
{
  malashenko::List< int > l;
  l.push_front(std::move(1));
  l.push_front(std::move(2));
  l.push_front(std::move(3));

  BOOST_CHECK(l.front() == 3);
  BOOST_CHECK(l.back() == 1);
}

BOOST_AUTO_TEST_CASE(insert_with_empty_list)
{
  malashenko::List< int > l;
  l.insert(l.begin(), 1);
  BOOST_CHECK(l.front() == 1);
}

BOOST_AUTO_TEST_CASE(insert_with_non_empty_list)
{
  malashenko::List< int > l;
  l.push_front(1);
  l.push_front(3);
  malashenko::LIter< int > it = l.insert(++l.begin(), 2);

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(insert_rvalue_with_empty_list)
{
  malashenko::List< int > l;
  l.insert(l.begin(), std::move(1));
  BOOST_CHECK(l.front() == 1);
}

BOOST_AUTO_TEST_CASE(insert_rvalue_with_non_empty_list)
{
  malashenko::List< int > l;
  l.push_front(1);
  l.push_front(3);
  malashenko::LIter< int > it = l.insert(++l.begin(), std::move(2));

  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(List_deleting_elements)
BOOST_AUTO_TEST_CASE(pop_list)
{
  malashenko::List< int > l;
  l.push_front(1);
  l.push_front(2);
  l.push_front(3);

  l.pop_back();
  l.pop_front();

  BOOST_CHECK(l.front() == 2);
  BOOST_CHECK(l.size() == 1);
}

BOOST_AUTO_TEST_CASE(cut_element_of_list)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  l.cut(l.begin());
  BOOST_CHECK(l.front() == 2);
  BOOST_CHECK(l.size() == 2);

  l.cut(l.begin());
  BOOST_CHECK_EQUAL(l.front(), 3);
  BOOST_CHECK(l.size() == 1);

  l.cut(l.begin());
  BOOST_CHECK(l.empty());

  l.cut(l.begin());
  BOOST_CHECK(l.empty());
}

BOOST_AUTO_TEST_CASE(erase_element_of_list)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  l.erase(l.begin());
  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(*(++l.begin()) == 3);
  BOOST_CHECK(l.size() == 2);

  l.erase(l.begin());
  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.size() == 1);

  l.erase(l.begin());
  BOOST_CHECK(l.empty());

  l.erase(l.begin());
  BOOST_CHECK(l.empty());
}

BOOST_AUTO_TEST_CASE(clear_list)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  l.clear();
  BOOST_CHECK(l.empty());
}


BOOST_AUTO_TEST_SUITE_END()
