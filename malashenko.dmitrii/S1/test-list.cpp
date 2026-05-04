#include <boost/test/unit_test.hpp>
#include <list/list.hpp>

BOOST_AUTO_TEST_SUITE(List_test_suite)
BOOST_AUTO_TEST_CASE(creation_of_list)
{
  malashenko::List< int > l;
  BOOST_CHECK(l.begin() == l.getFake());
  BOOST_CHECK(l.end() == l.getFake());

  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  BOOST_CHECK(l.front() == 1);

  BOOST_CHECK(l.end() == l.getFake());

  malashenko::List< int > k(l);
  k.push_back(4);
  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(l.back() == 3);
  BOOST_CHECK(k.back() == 4);

  malashenko::List< int > g = k;
  g.pop_front();
  BOOST_CHECK(g.front() == 2);
  BOOST_CHECK(l.front() == 1);
  BOOST_CHECK(k.front() == 1);
}

BOOST_AUTO_TEST_CASE(begin_and_end)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  BOOST_CHECK(*(l.begin()) == 1);
  BOOST_CHECK(*(l.begin() + 1) == 2);
  BOOST_CHECK(*(l.begin() + 2) == 3);
  BOOST_CHECK(l.end() == l.getFake());
}

BOOST_AUTO_TEST_CASE(cbegin_and_cend)
{
  malashenko::List< const int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  BOOST_CHECK(*(l.cbegin()) == 1);
  BOOST_CHECK(*(l.cbegin() + 1) == 2);
  BOOST_CHECK(*(l.cbegin() + 2) == 3);
}

BOOST_AUTO_TEST_CASE(insert_and_push)
{
  malashenko::List< int > l;
  malashenko::LIter< int > it = l.begin();
  l.insert(it, 1);
  BOOST_CHECK(l.front() == 1);
  l.push_back(2);
  BOOST_CHECK(l.back() == 2);
  l.push_front(0);
  BOOST_CHECK(l.front() == 0);
}

BOOST_AUTO_TEST_CASE(cut_and_pop)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  malashenko::LIter< int > it = l.begin();
  l.cut(it);
  BOOST_CHECK(l.front() == 2);
  it = it + 1;
  l.push_front(1);
  l.push_front(0);
  l.push_back(4);
  l.pop_back();
  BOOST_CHECK(l.back() == 3);
  l.pop_front();
  BOOST_CHECK(l.front() == 1);
}

BOOST_AUTO_TEST_CASE(clear_size_empty)
{
  malashenko::List< int > l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  BOOST_CHECK(l.size() == 3);
  BOOST_CHECK(l.empty() != true);
  l.clear();
  BOOST_CHECK(l.size() == 0);
  BOOST_CHECK(l.empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()
