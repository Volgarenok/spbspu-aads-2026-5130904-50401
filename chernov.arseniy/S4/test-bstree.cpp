#include <boost/test/unit_test.hpp>
#include <bstree.hpp>

BOOST_AUTO_TEST_SUITE(bstree_special_members_tests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK(bst.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  bst1.push(123, 42);
  bst1.push(321, 52);

  chernov::BSTree< int, int, std::less< int > > bst2(bst1);
  BOOST_CHECK_EQUAL(bst2.size(), 2);
  BOOST_CHECK_EQUAL(bst2.at(123), 42);
  BOOST_CHECK_EQUAL(bst2.at(321), 52);

  bst2.push(456, 67);
  BOOST_CHECK_EQUAL(bst1.size(), 2);
  BOOST_CHECK_EQUAL(bst2.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_move_constructor)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  bst1.push(123, 42);
  bst1.push(321, 52);

  chernov::BSTree< int, int, std::less< int > > bst2(std::move(bst1));
  BOOST_CHECK_EQUAL(bst2.size(), 2);

  BOOST_CHECK(bst1.empty());

  bst2.push(456, 67);
  BOOST_CHECK_EQUAL(bst1.size(), 0);
  BOOST_CHECK_EQUAL(bst2.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  auto * bst = new chernov::BSTree< int, int, std::less< int > >;
  bst->push(123, 42);
  bst->push(321, 52);
  bst->push(456, 67);

  BOOST_CHECK_EQUAL(bst->size(), 3);
  delete bst;
}

BOOST_AUTO_TEST_CASE(test_copy_operator)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  bst1.push(123, 42);
  bst1.push(321, 52);

  bst1 = bst1;
  BOOST_CHECK_EQUAL(bst1.size(), 2);
  BOOST_CHECK_EQUAL(bst1.at(123), 42);

  chernov::BSTree< int, int, std::less< int > > bst2;
  bst2 = bst1;

  BOOST_CHECK_EQUAL(bst2.size(), 2);
  BOOST_CHECK_EQUAL(bst2.at(123), 42);
  BOOST_CHECK_EQUAL(bst2.at(321), 52);

  bst2.push(456, 67);
  BOOST_CHECK_EQUAL(bst1.size(), 2);
  BOOST_CHECK_EQUAL(bst2.size(), 3);

  bst1.clear();
  BOOST_CHECK_EQUAL(bst1.size(), 0);
  bst2 = bst1;
  BOOST_CHECK_EQUAL(bst2.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_operator)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  bst1.push(123, 42);
  bst1.push(321, 52);

  chernov::BSTree< int, int, std::less< int > > bst2;
  bst2 = std::move(bst1);

  BOOST_CHECK(bst1.empty());

  BOOST_CHECK_EQUAL(bst2.size(), 2);
  BOOST_CHECK_EQUAL(bst2.at(123), 42);
  BOOST_CHECK_EQUAL(bst2.at(321), 52);

  bst2.push(456, 67);
  BOOST_CHECK_EQUAL(bst1.size(), 0);
  BOOST_CHECK_EQUAL(bst2.size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bstree_modifiers_tests)

BOOST_AUTO_TEST_CASE(test_swap)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  bst1.push(123, 42);
  bst1.push(321, 52);

  chernov::BSTree< int, int, std::less< int > > bst2;
  bst1.swap(bst2);

  BOOST_CHECK(bst1.empty());
  BOOST_CHECK_THROW(bst1.at(123), std::out_of_range);

  BOOST_CHECK_EQUAL(bst2.size(), 2);
  BOOST_CHECK_EQUAL(bst2.at(123), 42);
}

BOOST_AUTO_TEST_CASE(test_push)
{
  chernov::BSTree< int, int, std::less< int > > bst;

  bst.push(123, 42);
  BOOST_CHECK_EQUAL(bst.size(), 1);
  BOOST_CHECK_EQUAL(bst.at(123), 42);

  bst.push(321, 52);
  BOOST_CHECK_EQUAL(bst.size(), 2);
  BOOST_CHECK_EQUAL(bst.at(321), 52);

  bst.push(123, 78);
  BOOST_CHECK_EQUAL(bst.size(), 2);
  BOOST_CHECK_EQUAL(bst.at(123), 78);

  int key = 456;
  int value = 67;
  bst.push(key, value);
  BOOST_CHECK_EQUAL(bst.size(), 3);
  BOOST_CHECK_EQUAL(bst.at(456), 67);
}

BOOST_AUTO_TEST_CASE(test_remove)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK_THROW(bst.remove(123), std::out_of_range);

  bst.push(123, 42);
  bst.remove(123);
  BOOST_CHECK(bst.empty());

  bst.push(123, 42);
  bst.push(321, 52);
  bst.push(456, 67);

  bst.remove(456);
  bst.remove(123);
  BOOST_CHECK_EQUAL(bst.size(), 1);
  BOOST_CHECK_EQUAL(bst.at(321), 52);
  bst.remove(321);
  BOOST_CHECK(bst.empty());
  BOOST_CHECK_THROW(bst.remove(321), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  bst.clear();
  BOOST_CHECK(bst.empty());

  bst.push(123, 42);
  bst.push(321, 52);
  bst.clear();
  BOOST_CHECK(bst.empty());
  BOOST_CHECK_THROW(bst.at(123), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  bst.push(10, 210);
  bst.push(5, 205);
  bst.push(2, 202);
  bst.push(9, 209);
  bst.push(15, 215);
  bst.push(13, 213);
  bst.push(12, 212);
  bst.push(14, 214);
  bst.push(16, 216);

  auto iter = bst.cbegin();
  auto new_iter = bst.rotateLeft(iter);

  BOOST_CHECK_EQUAL(iter->first, 10);
  BOOST_CHECK_EQUAL((--iter)->first, 9);
  BOOST_CHECK_EQUAL((--iter)->first, 5);
  ++iter;
  ++iter;

  BOOST_CHECK_EQUAL((++iter)->first, 12);
  BOOST_CHECK_EQUAL((++iter)->first, 13);
  BOOST_CHECK_EQUAL((++iter)->first, 14);
  BOOST_CHECK((++iter) == new_iter);

  BOOST_CHECK_EQUAL(new_iter->first, 15);
  BOOST_CHECK_EQUAL((++new_iter)->first, 16);
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  bst.push(10, 210);
  bst.push(5, 205);
  bst.push(2, 202);
  bst.push(9, 209);
  bst.push(15, 215);
  bst.push(13, 213);
  bst.push(12, 212);
  bst.push(14, 214);
  bst.push(16, 216);

  auto iter = bst.cfindIter(15);
  auto new_iter = bst.rotateRight(iter);

  BOOST_CHECK_EQUAL(iter->first, 15);
  BOOST_CHECK_EQUAL((++iter)->first, 16);
  --iter;
  BOOST_CHECK_EQUAL((--iter)->first, 14);
  BOOST_CHECK((--iter) == new_iter);

  BOOST_CHECK_EQUAL(new_iter->first, 13);
  BOOST_CHECK_EQUAL((--new_iter)->first, 12);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bstree_getters_tests)

BOOST_AUTO_TEST_CASE(test_at)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  BOOST_CHECK_THROW(bst1.at(123), std::out_of_range);

  bst1.push(123, 42);
  BOOST_CHECK_EQUAL(bst1.at(123), 42);

  bst1.push(321, 52);
  BOOST_CHECK_EQUAL(bst1.at(321), 52);

  bst1.clear();
  BOOST_CHECK_THROW(bst1.at(321), std::out_of_range);

  bst1.push(456, 67);
  const chernov::BSTree< int, int, std::less< int > > & bst2 = bst1;
  BOOST_CHECK_EQUAL(bst2.at(456), 67);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK_EQUAL(bst.size(), 0);
  BOOST_CHECK(bst.empty());

  bst.push(123, 42);
  BOOST_CHECK_EQUAL(bst.size(), 1);
  BOOST_CHECK(!bst.empty());

  bst.clear();
  BOOST_CHECK_EQUAL(bst.size(), 0);
  BOOST_CHECK(bst.empty());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK_EQUAL(bst.size(), 0);

  bst.push(123, 42);
  BOOST_CHECK_EQUAL(bst.size(), 1);

  bst.push(321, 52);
  BOOST_CHECK_EQUAL(bst.size(), 2);

  bst.remove(123);
  BOOST_CHECK_EQUAL(bst.size(), 1);

  bst.clear();
  BOOST_CHECK_EQUAL(bst.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_contains)
{
  chernov::BSTree< int, int, std::less< int > > bst;

  BOOST_CHECK(!bst.contains(123));
  bst.push(123, 42);
  bst.push(321, 52);
  BOOST_CHECK(bst.contains(123));

  bst.remove(123);
  BOOST_CHECK(!bst.contains(123));
  BOOST_CHECK(bst.contains(321));
}

BOOST_AUTO_TEST_CASE(test_height)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK_EQUAL(bst.height(), 0);

  auto iter = bst.cbeforeBegin();

  bst.push(321, 52);
  BOOST_CHECK_EQUAL(bst.height(), 1);
  BOOST_CHECK_EQUAL(bst.height(++iter), 1);

  bst.push(123, 42);
  BOOST_CHECK_EQUAL(bst.height(), 2);
  BOOST_CHECK_EQUAL(bst.height(iter), 2);
  BOOST_CHECK_EQUAL(bst.height(--iter), 1);

  bst.push(456, 67);
  BOOST_CHECK_EQUAL(bst.height(), 2);
  BOOST_CHECK_EQUAL(bst.height(iter), 1);
  BOOST_CHECK_EQUAL(bst.height(++iter), 2);
  BOOST_CHECK_EQUAL(bst.height(++iter), 1);

  bst.push(789, 78);
  BOOST_CHECK_EQUAL(bst.height(), 3);
  BOOST_CHECK_EQUAL(bst.height(iter), 2);
  BOOST_CHECK_EQUAL(bst.height(++iter), 1);
  BOOST_CHECK_EQUAL(bst.height(++iter), 0);

  bst.remove(123);
  BOOST_CHECK_EQUAL(bst.height(), 3);

  bst.remove(456);
  BOOST_CHECK_EQUAL(bst.height(), 2);
  BOOST_CHECK_EQUAL(bst.height(--iter), 1);
  BOOST_CHECK_EQUAL(bst.height(--iter), 2);

  bst.remove(789);
  BOOST_CHECK_EQUAL(bst.height(), 1);

  bst.remove(321);
  BOOST_CHECK_EQUAL(bst.height(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bstree_iterator_tests)

BOOST_AUTO_TEST_CASE(test_before_begin_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK(++bst.beforeBegin() == bst.begin());
  BOOST_CHECK(++bst.beforeBegin() == bst.end());

  bst.push(123, 42);
  bst.push(321, 52);

  auto iter = bst.beforeBegin();
  BOOST_CHECK(iter != bst.begin());
  BOOST_CHECK(iter != bst.end());

  ++iter;
  BOOST_CHECK(iter == bst.begin());
  BOOST_CHECK_EQUAL(iter->first, 123);
  ++iter;
  BOOST_CHECK_EQUAL(iter->first, 321);

  BOOST_CHECK(++iter == bst.end());
}

BOOST_AUTO_TEST_CASE(test_cbefore_begin_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  BOOST_CHECK(++bst1.cbeforeBegin() == bst1.cbegin());
  BOOST_CHECK(++bst1.cbeforeBegin() == bst1.cend());

  bst1.push(123, 42);
  bst1.push(321, 52);

  const chernov::BSTree< int, int, std::less< int > > & bst2 = bst1;

  auto iter = bst2.cbeforeBegin();
  BOOST_CHECK(iter != bst2.cbegin());
  BOOST_CHECK(iter != bst2.cend());

  ++iter;
  BOOST_CHECK(iter == bst2.cbegin());
  BOOST_CHECK_EQUAL(iter->first, 123);
  ++iter;
  BOOST_CHECK_EQUAL(iter->first, 321);

  BOOST_CHECK(++iter == bst2.cend());
}

BOOST_AUTO_TEST_CASE(test_begin_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK(bst.begin() == bst.end());

  bst.push(123, 42);
  bst.push(321, 52);

  auto iter = bst.begin();
  BOOST_CHECK(iter != bst.end());
  BOOST_CHECK_EQUAL(iter->first, 123);

  ++iter;
  BOOST_CHECK_EQUAL(iter->first, 321);
  BOOST_CHECK(++iter == bst.end());
}

BOOST_AUTO_TEST_CASE(test_сbegin_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  BOOST_CHECK(bst1.cbegin() == bst1.cend());

  bst1.push(123, 42);
  bst1.push(321, 52);

  const chernov::BSTree< int, int, std::less< int > > & bst2 = bst1;

  auto iter = bst2.cbegin();
  BOOST_CHECK(iter != bst2.cend());
  BOOST_CHECK_EQUAL(iter->first, 123);

  ++iter;
  BOOST_CHECK_EQUAL(iter->first, 321);
  BOOST_CHECK(++iter == bst2.cend());
}

BOOST_AUTO_TEST_CASE(test_end_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK(bst.begin() == bst.end());

  bst.push(123, 42);
  bst.push(321, 52);

  size_t count = 0;
  for (auto iter = bst.begin(); iter != bst.end(); ++iter) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_cend_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  BOOST_CHECK(bst1.cbegin() == bst1.cend());

  bst1.push(123, 42);
  bst1.push(321, 52);

  const chernov::BSTree< int, int, std::less< int > > & bst2 = bst1;

  size_t count = 0;
  for (auto iter = bst2.cbegin(); iter != bst2.cend(); ++iter) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(test_find_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  bst.push(321, 52);
  bst.push(123, 42);
  bst.push(456, 67);
  bst.push(789, 78);

  auto iter = bst.findIter(456);
  BOOST_CHECK_EQUAL(iter->first, 456);
  BOOST_CHECK_EQUAL((++iter)->first, 789);
  BOOST_CHECK((++iter) == bst.end());

  BOOST_CHECK(bst.findIter(234) == bst.end());
}

BOOST_AUTO_TEST_CASE(test_cfind_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  bst1.push(321, 52);
  bst1.push(123, 42);
  bst1.push(456, 67);
  bst1.push(789, 78);

  const chernov::BSTree< int, int, std::less< int > > & bst2 = bst1;

  auto iter = bst2.findIter(456);
  BOOST_CHECK_EQUAL(iter->first, 456);
  BOOST_CHECK_EQUAL((++iter)->first, 789);
  BOOST_CHECK((++iter) == bst2.cend());

  BOOST_CHECK(bst2.cfindIter(234) == bst2.cend());
}

BOOST_AUTO_TEST_CASE(test_operator_double_minus)
{
  chernov::BSTree< int, int, std::less< int > > bst1;
  BOOST_CHECK(--bst1.begin() == bst1.beforeBegin());
  BOOST_CHECK(--bst1.end() == bst1.beforeBegin());

  bst1.push(123, 42);
  bst1.push(321, 52);

  auto iter1 = bst1.begin();
  ++iter1;
  --iter1;
  BOOST_CHECK_EQUAL(iter1->first, 123);

  ++iter1;
  ++iter1;
  --iter1;
  BOOST_CHECK_EQUAL(iter1->first, 321);

  iter1--;
  BOOST_CHECK_EQUAL(iter1->first, 123);

  iter1--;
  BOOST_CHECK(iter1 == bst1.beforeBegin());

  const chernov::BSTree< int, int, std::less< int > > & bst2 = bst1;

  auto iter2 = bst2.cbegin();
  ++iter2;
  --iter2;
  BOOST_CHECK_EQUAL(iter2->first, 123);
}

BOOST_AUTO_TEST_SUITE_END()
