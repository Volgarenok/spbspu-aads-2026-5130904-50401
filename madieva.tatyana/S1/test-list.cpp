#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(default_constructor_and_empty_state)
{
  madieva::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.empty());

  BOOST_CHECK(list.begin() == list.end());

  int count = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 0);
}

BOOST_AUTO_TEST_CASE(pushFront)
{
  madieva::List< int > list;
  list.pushFront(10);
  list.pushFront(20);
  list.pushFront(30);
  list.pushFront(40);
  BOOST_CHECK(list.size() == 4);

  auto it = list.begin();
  BOOST_CHECK(*it == 40);
  ++it;
  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 10);
}

BOOST_AUTO_TEST_CASE(pushBack)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);
  list.pushBack(40);
  BOOST_CHECK(list.size() == 4);

  auto it = list.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(*it == 40);
}

BOOST_AUTO_TEST_CASE(popFront)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);

  list.popFront();
  BOOST_CHECK(list.size() == 2);
  auto it = list.begin();
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);
}

BOOST_AUTO_TEST_CASE(popBack)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);

  list.popBack();
  BOOST_CHECK(list.size() == 2);
  auto it = list.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
}

BOOST_AUTO_TEST_CASE(popBack_one)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.popBack();
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(popFront_one)
{
  madieva::List< int > list;
  list.pushFront(10);
  list.popFront();
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(clear)
{
  madieva::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.clear();
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(move_semantics)
{
  madieva::List< std::string > list;
  std::string val = "move_me";

  list.pushBack(std::move(val));

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK(*list.begin() == "move_me");
  BOOST_CHECK(val.empty());

  val = "front_val";
  list.pushFront(std::move(val));

  BOOST_CHECK_EQUAL(list.size(), 2);
  BOOST_CHECK(*list.begin() == "front_val");
  BOOST_CHECK(val.empty());
}

BOOST_AUTO_TEST_CASE(swap_method)
{
  madieva::List< int > list_a;
  list_a.pushBack(10);
  list_a.pushBack(20);

  madieva::List< int > list_b;
  list_b.pushBack(100);
  list_b.pushBack(200);
  list_b.pushBack(300);

  list_a.swap(list_b);

  BOOST_CHECK_EQUAL(list_a.size(), 3);
  auto it_a = list_a.begin();
  BOOST_CHECK(*it_a == 100);
  ++it_a;
  BOOST_CHECK(*it_a == 200);
  ++it_a;
  BOOST_CHECK(*it_a == 300);

  BOOST_CHECK_EQUAL(list_b.size(), 2);
  auto it_b = list_b.begin();
  BOOST_CHECK(*it_b == 10);
  ++it_b;
  BOOST_CHECK(*it_b == 20);
}

BOOST_AUTO_TEST_CASE(arrow_operator)
{
  madieva::List< std::pair< int, char > > list;
  list.pushBack({1, 'a'});
  list.pushBack({2, 'b'});

  auto it = list.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second, 'a');

  ++it;
  BOOST_CHECK_EQUAL(it->first, 2);
  BOOST_CHECK_EQUAL(it->second, 'b');
}

BOOST_AUTO_TEST_CASE(push_front_and_back)
{
  madieva::List< int > list;

  list.pushFront(10);
  list.pushBack(20);

  BOOST_CHECK_EQUAL(list.size(), 2);
  auto it = list.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(it == list.end());

  list.pushFront(5);
  list.pushBack(25);

  BOOST_CHECK_EQUAL(list.size(), 4);
  it = list.begin();
  BOOST_CHECK(*it == 5);
  ++it;
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 25);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(iterator_begin_end_empty)
{
  madieva::List< int > empty_list;
  BOOST_CHECK(empty_list.begin() == empty_list.end());
  BOOST_CHECK(empty_list.cbegin() == empty_list.cend());
}

BOOST_AUTO_TEST_CASE(iterator_forward_traversal)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);

  auto it = list.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(iterator_backward_traversal)
{
  madieva::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  auto it = list.end();
  --it;
  BOOST_CHECK(*it == 3);
  --it;
  BOOST_CHECK(*it == 2);
  --it;
  BOOST_CHECK(*it == 1);
  BOOST_CHECK(it == list.begin());
}

BOOST_AUTO_TEST_CASE(iterator_comparison)
{
  madieva::List< int > list;
  list.pushBack(42);

  auto it1 = list.begin();
  auto it2 = list.begin();
  BOOST_CHECK(it1 == it2);
  BOOST_CHECK(!(it1 != it2));

  ++it2;
  BOOST_CHECK(it1 != it2);
  BOOST_CHECK(!(it1 == it2));
}

BOOST_AUTO_TEST_CASE(const_iterator_access)
{
  madieva::List< std::string > list;
  list.pushBack("hello");
  list.pushBack("world");

  const madieva::List< std::string > & const_ref = list;
  auto cit = const_ref.cbegin();

  BOOST_CHECK(*cit == "hello");
  ++cit;
  BOOST_CHECK(*cit == "world");
  ++cit;
  BOOST_CHECK(cit == const_ref.cend());
}

BOOST_AUTO_TEST_CASE(const_iterator_read_only)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.pushBack(20);

  madieva::List< int > & const_ref = list;
  auto cit = const_ref.cbegin();

  BOOST_CHECK(*cit == 10);
  ++cit;
  BOOST_CHECK(*cit == 20);
  ++cit;
  BOOST_CHECK(cit == const_ref.cend());
}

BOOST_AUTO_TEST_CASE(mutable_iterator_modification)
{
  madieva::List< int > list;
  list.pushBack(100);
  list.pushBack(200);

  auto it = list.begin();
  *it = 150;
  BOOST_CHECK(*list.begin() == 150);

  ++it;
  *it = 250;
  ++it;
  BOOST_CHECK(it == list.end());

  --it;
  BOOST_CHECK(*it == 250);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  madieva::List< int > original;
  original.pushBack(10);
  original.pushBack(20);
  original.pushBack(30);

  madieva::List< int > copy(original);

  BOOST_CHECK_EQUAL(copy.size(), 3);
  auto it = copy.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);

  original.popBack();
  BOOST_CHECK_EQUAL(original.size(), 2);
  BOOST_CHECK_EQUAL(copy.size(), 3);
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator)
{
  madieva::List< int > source;
  source.pushFront(5);
  source.pushBack(15);

  madieva::List< int > destination;
  destination.pushBack(100);
  destination = source;

  BOOST_CHECK_EQUAL(destination.size(), 2);
  auto it = destination.begin();
  BOOST_CHECK(*it == 5);
  ++it;
  BOOST_CHECK(*it == 15);

  source.pushBack(25);
  BOOST_CHECK_EQUAL(source.size(), 3);
  BOOST_CHECK_EQUAL(destination.size(), 2);
}

BOOST_AUTO_TEST_CASE(copy_empty_list)
{
  madieva::List< int > empty_original;
  madieva::List< int > empty_copy(empty_original);

  BOOST_CHECK(empty_copy.empty());
  BOOST_CHECK(empty_copy.begin() == empty_copy.end());
}

BOOST_AUTO_TEST_CASE(clear_empty_list)
{
  madieva::List< int > list;
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(postfixIncrement)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.pushBack(20);

  auto it = list.begin();
  auto old_it = it++;

  BOOST_CHECK(*old_it == 10);
  BOOST_CHECK(*it == 20);
  BOOST_CHECK(old_it != it);
}

BOOST_AUTO_TEST_CASE(postfixIncrementBoundary)
{
  madieva::List< int > list;
  list.pushBack(1);
  list.pushBack(2);

  auto it = list.begin();
  ++it;
  auto old_it = it++;

  BOOST_CHECK(*old_it == 2);
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(postfixDecrement)
{
  madieva::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);

  auto it = list.end();
  auto old_it = it--;

  BOOST_CHECK(old_it == list.end());
  BOOST_CHECK(*it == 30);
}

BOOST_AUTO_TEST_CASE(postfixDecrementBoundary)
{
  madieva::List< int > list;
  list.pushBack(5);
  list.pushBack(15);

  auto it = list.begin();
  ++it;
  auto old_it = it--;

  BOOST_CHECK(*old_it == 15);
  BOOST_CHECK(*it == 5);
}

BOOST_AUTO_TEST_CASE(splice_one_element)
{
  madieva::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);

  madieva::List< int > list2;
  list2.pushBack(10);
  list2.pushBack(20);

  madieva::LIter< int > it = list1.end();

  list1.splice(it, list2, list2.begin());

  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(list2.size(), 1);

  madieva::LIter< int > check = list1.begin();
  BOOST_CHECK(*check == 1);
  ++check;
  BOOST_CHECK(*check == 2);
  ++check;
  BOOST_CHECK(*check == 3);
  ++check;
  BOOST_CHECK(*check == 10);

  BOOST_CHECK(*list2.begin() == 20);
}

BOOST_AUTO_TEST_CASE(splice_whole_list)
{
  madieva::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);

  madieva::List< int > list2;
  list2.pushBack(10);
  list2.pushBack(20);
  list2.pushBack(30);

  madieva::LIter< int > pos = list1.begin();
  list1.splice(pos, list2);

  BOOST_CHECK_EQUAL(list1.size(), 5);
  BOOST_CHECK(list2.empty());

  madieva::LIter< int > it = list1.begin();
  BOOST_CHECK(*it == 10);
  ++it;
  BOOST_CHECK(*it == 20);
  ++it;
  BOOST_CHECK(*it == 30);
  ++it;
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
}

BOOST_AUTO_TEST_CASE(splice_range)
{
  madieva::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(5);

  madieva::List< int > list2;
  list2.pushBack(2);
  list2.pushBack(3);
  list2.pushBack(4);
  list2.pushBack(6);

  madieva::LIter< int > first = list2.begin();
  madieva::LIter< int > last = list2.begin();
  ++last;
  ++last;

  madieva::LIter< int > pos = list1.begin();
  ++pos;

  list1.splice(pos, list2, first, last);

  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(list2.size(), 2);

  madieva::LIter< int > it = list1.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 5);
}

BOOST_AUTO_TEST_CASE(sort_ascending)
{
  madieva::List< int > list;
  list.pushBack(5);
  list.pushBack(2);
  list.pushBack(1);

  list.sort();

  madieva::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 5);
}

BOOST_AUTO_TEST_CASE(sort_descending_comparator)
{
  madieva::List< int > list;
  list.pushBack(5);
  list.pushBack(2);
  list.pushBack(8);
  list.pushBack(1);
  list.pushBack(3);

  list.sort([](int a, int b) { return a > b; });

  madieva::LIter< int > it = list.begin();
  BOOST_CHECK(*it == 8);
  ++it;
  BOOST_CHECK(*it == 5);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 1);
}

BOOST_AUTO_TEST_CASE(merge_two_sorted_lists)
{
  madieva::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(3);
  list1.pushBack(5);

  madieva::List< int > list2;
  list2.pushBack(2);
  list2.pushBack(4);
  list2.pushBack(6);

  list1.merge(list2);

  BOOST_CHECK(list2.empty());
  BOOST_CHECK_EQUAL(list1.size(), 6);

  madieva::LIter< int > it = list1.begin();
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

BOOST_AUTO_TEST_CASE(merge_with_duplicates)
{
  madieva::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(3);
  list1.pushBack(5);

  madieva::List< int > list2;
  list2.pushBack(3);
  list2.pushBack(4);

  list1.merge(list2);

  madieva::LIter< int > it = list1.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(*it == 4);
  ++it;
  BOOST_CHECK(*it == 5);
}

BOOST_AUTO_TEST_CASE(partition_by_value)
{
  madieva::List< int > list;
  list.pushBack(5);
  list.pushBack(2);
  list.pushBack(9);
  list.pushBack(1);
  list.pushBack(7);
  list.pushBack(3);

  madieva::LIter< int > boundary = list.partition(4);

  madieva::LIter< int > it = list.begin();
  for (; it != boundary; ++it) {
    BOOST_CHECK(*it < 4);
  }
  for (; it != list.end(); ++it) {
    BOOST_CHECK(*it >= 4);
  }
}

BOOST_AUTO_TEST_CASE(partition_by_predicate)
{
  madieva::List< int > list;
  list.pushBack(5);
  list.pushBack(2);
  list.pushBack(9);
  list.pushBack(1);
  list.pushBack(7);
  list.pushBack(3);

  madieva::LIter< int > boundary = list.partition([](int x) { return x % 2 == 0; });

  madieva::LIter< int > it = list.begin();
  for (; it != boundary; ++it) {
    BOOST_CHECK(*it % 2 == 0);
  }
  for (; it != list.end(); ++it) {
    BOOST_CHECK(*it % 2 != 0);
  }
}

BOOST_AUTO_TEST_CASE(emplace_back_test)
{
  madieva::List< std::pair< int, std::string > > list;
  
  list.emplace_back(1, "one");
  list.emplace_back(2, "two");
  list.emplace_back(3, "three");
  
  madieva::LIter< std::pair< int, std::string > > it = list.begin();
  BOOST_CHECK(it->first == 1);
  BOOST_CHECK(it->second == "one");
  ++it;
  BOOST_CHECK(it->first == 2);
  BOOST_CHECK(it->second == "two");
  ++it;
  BOOST_CHECK(it->first == 3);
  BOOST_CHECK(it->second == "three");
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(emplace_front_test)
{
  madieva::List< std::pair< int, std::string > > list;
  
  list.emplace_front(1, "one");
  list.emplace_front(2, "two");
  list.emplace_front(3, "three");
  
  madieva::LIter< std::pair< int, std::string > > it = list.begin();
  BOOST_CHECK(it->first == 3);
  BOOST_CHECK(it->second == "three");
  ++it;
  BOOST_CHECK(it->first == 2);
  BOOST_CHECK(it->second == "two");
  ++it;
  BOOST_CHECK(it->first == 1);
  BOOST_CHECK(it->second == "one");
}

BOOST_AUTO_TEST_CASE(emplace_at_position_test)
{
  madieva::List< int > list;
  list.pushBack(1);
  list.pushBack(3);
  list.pushBack(4);
  
  madieva::LIter< int > it = list.begin();
  ++it;
  list.emplace(it, 2);
  
  madieva::LIter< int > check = list.begin();
  BOOST_CHECK(*check == 1);
  ++check;
  BOOST_CHECK(*check == 2);
  ++check;
  BOOST_CHECK(*check == 3);
  ++check;
  BOOST_CHECK(*check == 4);
}

BOOST_AUTO_TEST_CASE(emplace_into_empty_list)
{
  madieva::List< std::string > list;
  
  list.emplace_back("first");
  list.emplace_front("second");
  
  madieva::LIter< std::string > it = list.begin();
  BOOST_CHECK(*it == "second");
  ++it;
  BOOST_CHECK(*it == "first");
}

BOOST_AUTO_TEST_SUITE_END()
