#include <boost/test/unit_test.hpp>
#include <list.hpp>

BOOST_AUTO_TEST_SUITE(list_suite)

BOOST_AUTO_TEST_CASE(test_create_list)
{
  hvostov::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_create_list_with_values)
{
  hvostov::List< int > list1;
  hvostov::List< int > list2(list1);
  BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE(test_insert_element)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  BOOST_CHECK(*it == 6);
  it = list.insertAfter(it, 7);
  BOOST_CHECK(*it == 7);
  it = list.begin();
  list.insertAfter(it, 10);
  BOOST_CHECK(*it == 6);
  it++;
  BOOST_CHECK(*it == 10);
  it++;
  BOOST_CHECK(*it == 7);
  it = list.end();
  list.insertAfter(it, 8);
  it++;
  BOOST_CHECK(*it == 8);
  it++;
  BOOST_CHECK(*it == 6);
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  hvostov::List< int > list;
  list.pushFront(5);
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 5);

  list.pushFront(3);
  BOOST_CHECK_EQUAL(list.size(), 2);
  hvostov::Liter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 3);
  it++;
  BOOST_CHECK_EQUAL(*it, 5);

  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 3);
  it = list.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  it++;
  BOOST_CHECK_EQUAL(*it, 3);
  it++;
  BOOST_CHECK_EQUAL(*it, 5);
}

BOOST_AUTO_TEST_CASE(test_erase_element)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  it = list.begin();
  list.eraseAfter(it);
  it++;
  BOOST_CHECK(!list.empty());
  BOOST_CHECK(it == list.end());
  it = list.begin();
  BOOST_CHECK((*it) == 6);
}

BOOST_AUTO_TEST_CASE(test_erase_last_element)
{
  hvostov::List< int > list;
  list.pushFront(10);
  BOOST_CHECK_EQUAL(list.size(), 1);

  hvostov::Liter< int > it = list.end();
  list.eraseAfter(it);
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_erase_middle_element)
{
  hvostov::List< int > list;
  list.pushFront(10);
  list.pushFront(20);
  list.pushFront(30);

  hvostov::Liter< int > it = list.end();
  it++;
  list.eraseAfter(it);

  hvostov::Liter< int > check = list.begin();
  BOOST_CHECK_EQUAL(*check, 30);
  check++;
  BOOST_CHECK_EQUAL(*check, 10);
  check++;
  BOOST_CHECK(check == list.end());
  BOOST_CHECK_EQUAL(list.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.size(), 4);
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
  it = list.begin();
  it = list.insertAfter(it, 6);
  it = list.insertAfter(it, 7);
  BOOST_CHECK(!list.empty());
  BOOST_CHECK_EQUAL(list.size(), 2);
  list.clear();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  hvostov::List< int > list1;
  list1.pushFront(10);
  list1.pushFront(20);
  list1.pushFront(30);

  hvostov::List< int > list2(list1);

  BOOST_CHECK_EQUAL(list2.size(), list1.size());

  hvostov::Liter< int > it1 = list1.begin();
  hvostov::Liter< int > it2 = list2.begin();

  while (it1 != list1.end() && it2 != list2.end()) {
    BOOST_CHECK_EQUAL(*it1, *it2);
    ++it1;
    ++it2;
  }

  BOOST_CHECK(it1 == list1.end());
  BOOST_CHECK(it2 == list2.end());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor_empty)
{
  hvostov::List< int > list1;
  hvostov::List< int > list2(list1);

  BOOST_CHECK(list2.empty());
  BOOST_CHECK_EQUAL(list2.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_copy_assignment)
{
  hvostov::List< int > list1;
  list1.pushFront(10);
  list1.pushFront(20);
  list1.pushFront(30);

  hvostov::List< int > list2;
  list2.pushFront(100);
  list2 = list1;

  BOOST_CHECK_EQUAL(list2.size(), list1.size());

  hvostov::Liter< int > it1 = list1.begin();
  hvostov::Liter< int > it2 = list2.begin();

  while (it1 != list1.end() && it2 != list2.end()) {
    BOOST_CHECK_EQUAL(*it1, *it2);
    ++it1;
    ++it2;
  }
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  hvostov::List< int > list1;
  list1.pushFront(10);
  list1.pushFront(20);
  list1.pushFront(30);

  size_t old_size = list1.size();

  hvostov::List< int > list2(std::move(list1));

  BOOST_CHECK_EQUAL(list2.size(), old_size);
  BOOST_CHECK_EQUAL(list1.empty(), true);

  hvostov::Liter< int > it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(test_move_assignment)
{
  hvostov::List< int > list1;
  list1.pushFront(10);
  list1.pushFront(20);
  list1.pushFront(30);

  hvostov::List< int > list2;
  list2.pushFront(100);

  size_t old_size = list1.size();
  list2 = std::move(list1);

  BOOST_CHECK_EQUAL(list2.size(), old_size);
  BOOST_CHECK(list1.empty());

  hvostov::Liter< int > it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  hvostov::List< int > list1;
  list1.pushFront(10);
  list1.pushFront(20);

  hvostov::List< int > list2;
  list2.pushFront(30);
  list2.pushFront(40);
  list2.pushFront(50);

  size_t size1 = list1.size();
  size_t size2 = list2.size();

  list1.swap(list2);

  BOOST_CHECK_EQUAL(list1.size(), size2);
  BOOST_CHECK_EQUAL(list2.size(), size1);

  hvostov::Liter< int > it = list1.begin();
  BOOST_CHECK_EQUAL(*it, 50);
  ++it;
  BOOST_CHECK_EQUAL(*it, 40);
  ++it;
  BOOST_CHECK_EQUAL(*it, 30);

  it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  hvostov::List< int > list;
  BOOST_CHECK(list.empty());

  list.pushFront(5);
  BOOST_CHECK(!list.empty());

  list.clear();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  hvostov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);

  list.pushFront(1);
  BOOST_CHECK_EQUAL(list.size(), 1);

  list.pushFront(2);
  BOOST_CHECK_EQUAL(list.size(), 2);

  list.pushFront(3);
  BOOST_CHECK_EQUAL(list.size(), 3);

  hvostov::Liter< int > it = list.begin();
  list.eraseAfter(it);
  BOOST_CHECK_EQUAL(list.size(), 2);

  list.clear();
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_iterator_increment)
{
  hvostov::List< int > list;
  list.pushFront(10);
  list.pushFront(20);
  list.pushFront(30);

  hvostov::Liter< int > it = list.begin();
  BOOST_CHECK_EQUAL(*it, 30);

  hvostov::Liter< int > it2 = it++;
  BOOST_CHECK_EQUAL(*it2, 30);
  BOOST_CHECK_EQUAL(*it, 20);

  ++it;
  BOOST_CHECK_EQUAL(*it, 10);

  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  hvostov::List< int > list;
  list.pushFront(10);
  list.pushFront(20);
  list.pushFront(30);

  const hvostov::List< int >& const_list = list;

  hvostov::LCiter< int > it = const_list.begin();
  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK(it == const_list.end());
}

BOOST_AUTO_TEST_CASE(test_const_iterator_copy)
{
  hvostov::List< int > list;
  list.pushFront(10);

  const hvostov::List< int >& const_list = list;

  hvostov::LCiter< int > it1 = const_list.begin();
  hvostov::LCiter< int > it2 = it1;

  BOOST_CHECK(it1 == it2);
  BOOST_CHECK_EQUAL(*it1, *it2);
}

BOOST_AUTO_TEST_CASE(test_begin_end_consistency)
{
  hvostov::List< int > list;
  BOOST_CHECK(list.begin() == list.end());

  list.pushFront(5);
  BOOST_CHECK(list.begin() != list.end());

  hvostov::Liter< int > it = list.begin();
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(test_insert_after_end)
{
  hvostov::List< int > list;
  hvostov::Liter< int > it = list.end();

  it = list.insertAfter(it, 10);
  BOOST_CHECK_EQUAL(*it, 10);
  BOOST_CHECK_EQUAL(list.size(), 1);

  it = list.begin();
  BOOST_CHECK_EQUAL(*it, 10);
}

BOOST_AUTO_TEST_CASE(test_complex_type)
{
  hvostov::List< std::string > list;
  list.pushFront("hello");
  list.pushFront("world");

  BOOST_CHECK_EQUAL(list.size(), 2);

  hvostov::Liter< std::string > it = list.begin();
  BOOST_CHECK_EQUAL(*it, "world");
  ++it;
  BOOST_CHECK_EQUAL(*it, "hello");
}

BOOST_AUTO_TEST_CASE(test_splice_whole_list)
{
  hvostov::List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);
  list1.pushFront(3);

  hvostov::List< int > list2;
  list2.pushFront(4);
  list2.pushFront(5);
  list2.pushFront(6);

  size_t size1 = list1.size();
  size_t size2 = list2.size();

  list1.splice(list1.cbegin(), list2);

  BOOST_CHECK_EQUAL(list1.size(), size1 + size2);
  BOOST_CHECK(list2.empty());
  BOOST_CHECK_EQUAL(list2.size(), 0);

  hvostov::LCiter< int > it = list1.cbegin();
  BOOST_CHECK_EQUAL(*it, 6);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_splice_empty_list)
{
  hvostov::List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);

  hvostov::List< int > list2;

  list1.splice(list1.cbegin(), list2);

  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE(test_splice_to_empty_list)
{
  hvostov::List< int > list1;
  hvostov::List< int > list2;
  list2.pushFront(1);
  list2.pushFront(2);
  list2.pushFront(3);

  list1.splice(list1.cbegin(), list2);

  BOOST_CHECK_EQUAL(list1.size(), 3);
  BOOST_CHECK(list2.empty());

  hvostov::LCiter< int > it = list1.cbegin();
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_splice_single_element)
{
  hvostov::List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);

  hvostov::List< int > list2;
  list2.pushFront(3);
  list2.pushFront(4);
  list2.pushFront(5);

  hvostov::LCiter< int > it = list2.cbegin();
  list1.splice(list1.cbegin(), list2, it);

  BOOST_CHECK_EQUAL(list1.size(), 3);
  BOOST_CHECK_EQUAL(list2.size(), 2);

  hvostov::LCiter< int > check1 = list1.cbegin();
  BOOST_CHECK_EQUAL(*check1, 4);
  ++check1;
  BOOST_CHECK_EQUAL(*check1, 2);
  ++check1;
  BOOST_CHECK_EQUAL(*check1, 1);

  hvostov::LCiter< int > check2 = list2.cbegin();
  BOOST_CHECK_EQUAL(*check2, 5);
  ++check2;
  BOOST_CHECK_EQUAL(*check2, 3);
}

BOOST_AUTO_TEST_CASE(test_splice_single_element_to_end)
{
  hvostov::List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);

  hvostov::List< int > list2;
  list2.pushFront(3);
  list2.pushFront(4);

  hvostov::LCiter< int > it = list2.cbegin();
  list1.splice(list1.cend(), list2, it);

  BOOST_CHECK_EQUAL(list1.size(), 3);
  BOOST_CHECK_EQUAL(list2.size(), 1);

  hvostov::LCiter< int > check1 = list1.cbegin();
  BOOST_CHECK_EQUAL(*check1, 2);
  ++check1;
  BOOST_CHECK_EQUAL(*check1, 1);
  ++check1;
  BOOST_CHECK_EQUAL(*check1, 3);
}

BOOST_AUTO_TEST_CASE(test_splice_range)
{
  hvostov::List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);

  hvostov::List< int > list2;
  list2.pushFront(3);
  list2.pushFront(4);
  list2.pushFront(5);
  list2.pushFront(6);
  hvostov::LCiter< int > first = list2.cbegin();
  ++first;
  hvostov::LCiter< int > last = first;
  ++last;
  ++last;

  list1.splice(list1.cbegin(), list2, first, last);

  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK_EQUAL(list2.size(), 2);

  hvostov::LCiter< int > check1 = list1.cbegin();
  BOOST_CHECK_EQUAL(*check1, 5);
  ++check1;
  BOOST_CHECK_EQUAL(*check1, 4);
  ++check1;
  BOOST_CHECK_EQUAL(*check1, 2);
  ++check1;
  BOOST_CHECK_EQUAL(*check1, 1);

  hvostov::LCiter< int > check2 = list2.cbegin();
  BOOST_CHECK_EQUAL(*check2, 6);
  ++check2;
  BOOST_CHECK_EQUAL(*check2, 3);
}

BOOST_AUTO_TEST_CASE(test_splice_entire_range)
{
  hvostov::List< int > list1;
  list1.pushFront(1);

  hvostov::List< int > list2;
  list2.pushFront(2);
  list2.pushFront(3);
  list2.pushFront(4);

  list1.splice(list1.cbegin(), list2, list2.cbegin(), list2.cend());

  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK(list2.empty());

  hvostov::LCiter< int > check = list1.cbegin();
  BOOST_CHECK_EQUAL(*check, 4);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 1);
}

BOOST_AUTO_TEST_CASE(test_splice_middle_insertion)
{
  hvostov::List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);
  list1.pushFront(3);
  list1.pushFront(4);

  hvostov::List< int > list2;
  list2.pushFront(5);
  list2.pushFront(6);
  hvostov::LCiter< int > pos = list1.cbegin();
  ++pos;
  ++pos;

  list1.splice(pos, list2);

  BOOST_CHECK_EQUAL(list1.size(), 6);
  BOOST_CHECK(list2.empty());

  hvostov::LCiter< int > check = list1.cbegin();
  BOOST_CHECK_EQUAL(*check, 4);
  ++check;
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK_EQUAL(*check, 6);
  ++check;
  BOOST_CHECK_EQUAL(*check, 5);
  ++check;
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 1);
}

BOOST_AUTO_TEST_CASE(test_splice_same_list)
{
  hvostov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);

  list.splice(list.cbegin(), list);

  BOOST_CHECK_EQUAL(list.size(), 3);

  hvostov::LCiter< int > check = list.cbegin();
  BOOST_CHECK_EQUAL(*check, 3);
  ++check;
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;
  BOOST_CHECK_EQUAL(*check, 1);
}

BOOST_AUTO_TEST_CASE(test_sort_empty_list)
{
  hvostov::List< int > list;
  list.sort();
  BOOST_CHECK(list.empty());
  BOOST_CHECK_EQUAL(list.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_sort_single_element)
{
  hvostov::List< int > list;
  list.pushFront(5);
  list.sort();
  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.cbegin(), 5);
}

BOOST_AUTO_TEST_CASE(test_sort_already_sorted)
{
  hvostov::List< int > list;
  list.pushFront(3);
  list.pushFront(2);
  list.pushFront(1);

  list.sort();

  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(test_sort_reverse_order)
{
  hvostov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);
  list.pushFront(4);
  list.pushFront(5);

  list.sort();

  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
}

BOOST_AUTO_TEST_CASE(test_sort_random_order)
{
  hvostov::List< int > list;
  list.pushFront(42);
  list.pushFront(7);
  list.pushFront(100);
  list.pushFront(1);
  list.pushFront(50);

  list.sort();

  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 7);
  ++it;
  BOOST_CHECK_EQUAL(*it, 42);
  ++it;
  BOOST_CHECK_EQUAL(*it, 50);
  ++it;
  BOOST_CHECK_EQUAL(*it, 100);
}

BOOST_AUTO_TEST_CASE(test_sort_duplicates)
{
  hvostov::List< int > list;
  list.pushFront(3);
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(2);
  list.pushFront(1);

  list.sort();

  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(test_sort_strings)
{
  hvostov::List< std::string > list;
  list.pushFront("banana");
  list.pushFront("apple");
  list.pushFront("cherry");
  list.pushFront("date");

  list.sort();

  hvostov::LCiter< std::string > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, "apple");
  ++it;
  BOOST_CHECK_EQUAL(*it, "banana");
  ++it;
  BOOST_CHECK_EQUAL(*it, "cherry");
  ++it;
  BOOST_CHECK_EQUAL(*it, "date");
}

BOOST_AUTO_TEST_CASE(test_merge_empty_lists)
{
  hvostov::List< int > list1;
  hvostov::List< int > list2;

  list1.merge(list2);

  BOOST_CHECK(list1.empty());
  BOOST_CHECK(list2.empty());
}

BOOST_AUTO_TEST_CASE(test_merge_empty_into_nonempty)
{
  hvostov::List< int > list1;
  list1.pushFront(1);
  list1.pushFront(2);

  hvostov::List< int > list2;

  list1.merge(list2);

  BOOST_CHECK_EQUAL(list1.size(), 2);
  hvostov::LCiter< int > it = list1.cbegin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_merge_nonempty_into_empty)
{
  hvostov::List< int > list1;
  hvostov::List< int > list2;
  list2.pushFront(1);
  list2.pushFront(2);

  list1.merge(list2);

  BOOST_CHECK_EQUAL(list1.size(), 2);
  BOOST_CHECK(list2.empty());

  hvostov::LCiter< int > it = list1.cbegin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_merge_two_sorted_lists)
{
  hvostov::List< int > list1;
  list1.pushFront(6);
  list1.pushFront(4);
  list1.pushFront(2);

  hvostov::List< int > list2;
  list2.pushFront(5);
  list2.pushFront(3);
  list2.pushFront(1);

  list1.merge(list2);

  BOOST_CHECK_EQUAL(list1.size(), 6);
  BOOST_CHECK(list2.empty());

  hvostov::LCiter< int > it = list1.cbegin();
  for (int i = 1; i <= 6; ++i) {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }
}

BOOST_AUTO_TEST_CASE(test_merge_with_duplicates)
{
  hvostov::List< int > list1;
  list1.pushFront(4);
  list1.pushFront(2);
  list1.pushFront(2);
  list1.pushFront(1);

  hvostov::List< int > list2;
  list2.pushFront(3);
  list2.pushFront(2);
  list2.pushFront(1);

  list1.merge(list2);

  BOOST_CHECK_EQUAL(list1.size(), 7);

  hvostov::LCiter< int > it = list1.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
}

BOOST_AUTO_TEST_CASE(test_merge_move_semantics)
{
  hvostov::List< int > list1;
  list1.pushFront(2);
  list1.pushFront(1);

  hvostov::List< int > list2;
  list2.pushFront(4);
  list2.pushFront(3);

  list1.merge(std::move(list2));

  BOOST_CHECK_EQUAL(list1.size(), 4);
  BOOST_CHECK(list2.empty());

  hvostov::LCiter< int > it = list1.cbegin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
}

BOOST_AUTO_TEST_CASE(test_partition_empty_list)
{
  hvostov::List< int > list;
  list.partition([](int x) { return x > 0; });
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(test_partition_single_element_true)
{
  hvostov::List< int > list;
  list.pushFront(5);

  list.partition([](int x) { return x > 0; });

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.cbegin(), 5);
}

BOOST_AUTO_TEST_CASE(test_partition_single_element_false)
{
  hvostov::List< int > list;
  list.pushFront(-5);

  list.partition([](int x) { return x > 0; });

  BOOST_CHECK_EQUAL(list.size(), 1);
  BOOST_CHECK_EQUAL(*list.cbegin(), -5);
}

BOOST_AUTO_TEST_CASE(test_partition_all_true)
{
  hvostov::List< int > list;
  list.pushFront(2);
  list.pushFront(4);
  list.pushFront(6);
  list.pushFront(8);

  list.partition([](int x) { return x % 2 == 0; });

  BOOST_CHECK_EQUAL(list.size(), 4);
  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 8);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(test_partition_all_false)
{
  hvostov::List< int > list;
  list.pushFront(1);
  list.pushFront(3);
  list.pushFront(5);
  list.pushFront(7);

  list.partition([](int x) { return x % 2 == 0; });

  BOOST_CHECK_EQUAL(list.size(), 4);
  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 7);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_partition_even_odd)
{
  hvostov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);
  list.pushFront(4);
  list.pushFront(5);
  list.pushFront(6);

  list.partition([](int x) { return x % 2 == 0; });

  BOOST_CHECK_EQUAL(list.size(), 6);

  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 6);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_partition_positive_negative)
{
  hvostov::List< int > list;
  list.pushFront(-3);
  list.pushFront(10);
  list.pushFront(-1);
  list.pushFront(5);
  list.pushFront(-7);
  list.pushFront(8);

  list.partition([](int x) { return x > 0; });

  BOOST_CHECK_EQUAL(list.size(), 6);

  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 8);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, -7);
  ++it;
  BOOST_CHECK_EQUAL(*it, -1);
  ++it;
  BOOST_CHECK_EQUAL(*it, -3);
}

BOOST_AUTO_TEST_CASE(test_partition_strings_by_length)
{
  hvostov::List< std::string > list;
  list.pushFront("hi");
  list.pushFront("hello");
  list.pushFront("a");
  list.pushFront("world");
  list.pushFront("ok");

  list.partition([](const std::string& s) { return s.length() > 2; });

  BOOST_CHECK_EQUAL(list.size(), 5);

  hvostov::LCiter< std::string > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, "world");
  ++it;
  BOOST_CHECK_EQUAL(*it, "hello");
  ++it;
  BOOST_CHECK_EQUAL(*it, "ok");
  ++it;
  BOOST_CHECK_EQUAL(*it, "a");
  ++it;
  BOOST_CHECK_EQUAL(*it, "hi");
}

BOOST_AUTO_TEST_CASE(test_partition_preserves_order)
{
  hvostov::List< int > list;
  list.pushFront(1);
  list.pushFront(2);
  list.pushFront(3);
  list.pushFront(4);

  list.partition([](int x) { return x > 2; });

  hvostov::LCiter< int > it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(test_combined_sort_merge_partition)
{
  hvostov::List< int > list;
  list.pushFront(6);
  list.pushFront(1);
  list.pushFront(3);
  list.pushFront(8);
  list.pushFront(2);
  list.pushFront(5);
  list.pushFront(7);
  list.pushFront(4);

  list.sort();

  BOOST_CHECK_EQUAL(list.size(), 8);
  hvostov::LCiter< int > it = list.cbegin();
  for (int i = 1; i <= 8; ++i) {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }
  list.partition([](int x) { return x % 2 == 0; });

  BOOST_CHECK_EQUAL(list.size(), 8);
  it = list.cbegin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
  ++it;
  BOOST_CHECK_EQUAL(*it, 8);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 7);
  hvostov::List< int > other;
  other.pushFront(9);
  other.pushFront(10);
  other.pushFront(11);
  other.pushFront(12);
  other.sort();
  list.sort();
  list.merge(other);
  BOOST_CHECK_EQUAL(list.size(), 12);
  BOOST_CHECK(other.empty());

  it = list.cbegin();
  for (int i = 1; i <= 12; ++i) {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }
}

BOOST_AUTO_TEST_CASE(test_list_emplace)
{
  hvostov::List< std::pair< int, std::string > > list;

  auto it = list.begin();
  list.emplace(it, 1, "first");
  list.emplace(it, 2, "second");

  BOOST_CHECK_EQUAL(list.size(), 2);

  auto check_it = list.begin();
  BOOST_CHECK_EQUAL((*check_it).first, 2);
  ++check_it;
  BOOST_CHECK_EQUAL((*check_it).first, 1);
}

BOOST_AUTO_TEST_SUITE_END()
