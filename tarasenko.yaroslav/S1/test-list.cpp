#include <boost/test/unit_test.hpp>
#include "bidir_list.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  tarasenko::BidirList< int > list;
  BOOST_CHECK(list.size() == 0);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(adding_elems_to_the_end)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(--list.end()) == 5);
  list.push_back(2);
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(--list.end()) == 2);
}

BOOST_AUTO_TEST_CASE(dereference)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(list.cbegin()) == 5);
}

BOOST_AUTO_TEST_CASE(accessing_fields)
{
  struct Int
  {
    int a;
  };
  tarasenko::BidirList< Int > list;
  list.push_back(Int{5});
  BOOST_CHECK(list.begin()->a == 5);
  BOOST_CHECK(list.cbegin()->a == 5);
}

BOOST_AUTO_TEST_CASE(increment)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.push_back(2);
  list.push_back(3);
  tarasenko::ListIter< int > it = list.begin();
  tarasenko::ListConstIter< int > cit = list.cbegin();
  BOOST_CHECK(*(it++) == 5);
  BOOST_CHECK(*it == 2);
  BOOST_CHECK(*(++it) == 3);
  BOOST_CHECK(++it == list.end());
  BOOST_CHECK(*(cit++) == 5);
  BOOST_CHECK(*cit == 2);
  BOOST_CHECK(*(++cit) == 3);
  BOOST_CHECK(++cit == list.cend());
}

BOOST_AUTO_TEST_CASE(decrement)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.push_back(2);
  list.push_back(3);
  tarasenko::ListIter< int > it = list.end();
  tarasenko::ListConstIter< int > cit = list.cend();
  BOOST_CHECK(*(--it) == 3);
  BOOST_CHECK(*(it--) == 3);
  BOOST_CHECK(*(it--) == 2);
  BOOST_CHECK(it == list.begin());
  BOOST_CHECK(*(--cit) == 3);
  BOOST_CHECK(*(cit--) == 3);
  BOOST_CHECK(*(cit--) == 2);
  BOOST_CHECK(cit == list.cbegin());
}

BOOST_AUTO_TEST_CASE(adding_elems_to_the_begin)
{
  tarasenko::BidirList< int > list;
  list.push_front(5);
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 5);
  BOOST_CHECK(*(--list.end()) == 5);
  list.push_front(2);
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK(list.cbegin() != list.cend());
  BOOST_CHECK(*(list.begin()) == 2);
  BOOST_CHECK(*(--list.end()) == 5);
}

BOOST_AUTO_TEST_CASE(empty)
{
  tarasenko::BidirList< int > list;
  BOOST_CHECK(list.empty() == true);
  list.push_front(5);
  BOOST_CHECK(list.empty() == false);
}

BOOST_AUTO_TEST_CASE(accessint_edge_elems)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  BOOST_CHECK(list.front() == 5);
  BOOST_CHECK(list.back() == 5);
  list.push_back(2);
  BOOST_CHECK(list.front() == 5);
  BOOST_CHECK(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(erase_single_elem)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.erase(list.begin());
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(erase_one_elem_from_head)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.push_back(2);
  tarasenko::ListIter< int > it = list.begin();
  it = list.erase(it);
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(*(list.begin()) == 2);
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(erase_one_elem_from_tail)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.push_back(2);
  tarasenko::ListIter< int > it = --(list.end());
  it = list.erase(it);
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(*(--(list.end())) == 5);
  BOOST_CHECK(*(--it) == 5);
}

BOOST_AUTO_TEST_CASE(erase_one_elem_from_middle)
{
  tarasenko::BidirList< int > list;
  list.push_back(5);
  list.push_back(2);
  list.push_back(3);
  tarasenko::ListIter< int > it = ++(list.begin());
  it = list.erase(it);
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(*it == 3);
  BOOST_CHECK(*(--it) == 5);
}

BOOST_AUTO_TEST_CASE(erase_elems)
{
  tarasenko::BidirList< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  tarasenko::ListIter< int > first = list.begin();
  tarasenko::ListIter< int > last = list.end();
  for (size_t i = 0; i < 3; ++i)
  {
    ++first;
    --last;
  }
  first = list.erase(first, last);
  BOOST_CHECK(list.size() == 6);
  BOOST_CHECK(*first == 7);
  BOOST_CHECK(*(--first) == 2);
}

BOOST_AUTO_TEST_CASE(erase_from_edges)
{
  tarasenko::BidirList< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  for (size_t i = 0; i < 3; ++i)
  {
    list.pop_back();
    list.pop_front();
  }
  BOOST_CHECK(list.size() == 4);
  BOOST_CHECK(*(list.begin()) == 3);
  BOOST_CHECK(*(--(list.end())) == 6);
}

BOOST_AUTO_TEST_CASE(insert)
{
  tarasenko::BidirList< int > list;
  list.insert(list.begin(), 5);
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(*(list.begin()) == 5);
  list.insert(list.end(), 2);
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(*(++(list.begin())) == 2);
  list.insert(--(list.end()), 3);
  BOOST_CHECK(list.size() == 3);
  BOOST_CHECK(*(++(list.begin())) == 3);
}

BOOST_AUTO_TEST_CASE(clear)
{
  tarasenko::BidirList< int > list;
  list.clear();
  BOOST_CHECK(list.empty() == true);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  list.clear();
  BOOST_CHECK(list.empty() == true);
  BOOST_CHECK(list.begin() == list.end());
  BOOST_CHECK(list.cbegin() == list.cend());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  tarasenko::BidirList< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  tarasenko::BidirList< int > copy = list;
  auto lit = list.begin();
  BOOST_CHECK(copy.size() == 10);
  for (auto it = copy.begin(); it != copy.end(); ++it, ++lit)
  {
    BOOST_CHECK(*it == *lit);
  }
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  tarasenko::BidirList< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  tarasenko::BidirList< int > copy = std::move(list);
  BOOST_CHECK(copy.size() == 10);
  int i = 0;
  for (auto it = copy.begin(); it != copy.end(); ++it, ++i)
  {
    BOOST_CHECK(*it == i);
  }
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  tarasenko::BidirList< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  tarasenko::BidirList< int > copy;
  copy = list;
  auto lit = list.begin();
  BOOST_CHECK(copy.size() == 10);
  for (auto it = copy.begin(); it != copy.end(); ++it, ++lit)
  {
    BOOST_CHECK(*it == *lit);
  }
}

BOOST_AUTO_TEST_CASE(move_assignment_operator)
{
  tarasenko::BidirList< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.push_back(i);
  }
  tarasenko::BidirList< int > copy;
  copy = std::move(list);
  BOOST_CHECK(copy.size() == 10);
  int i = 0;
  for (auto it = copy.begin(); it != copy.end(); ++it, ++i)
  {
    BOOST_CHECK(*it == i);
  }
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}
