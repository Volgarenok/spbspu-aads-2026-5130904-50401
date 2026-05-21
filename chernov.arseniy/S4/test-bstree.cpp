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

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(bstree_iterator_tests)

BOOST_AUTO_TEST_CASE(test_begin_iterator)
{
  chernov::BSTree< int, int, std::less< int > > bst;
  BOOST_CHECK(bst.begin() == bst.end());

  bst.push(123, 42);
  bst.push(321, 52);

  auto iter = bst.begin();
  BOOST_CHECK(iter != bst.end());
  if ((iter++)->first == 123) {
    BOOST_CHECK_EQUAL(iter->first, 321);
  } else {
    BOOST_CHECK_EQUAL(iter->first, 123);
  }
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
  if ((iter++)->first == 123) {
    BOOST_CHECK_EQUAL(iter->first, 321);
  } else {
    BOOST_CHECK_EQUAL(iter->first, 123);
  }
  BOOST_CHECK(++iter == bst2.end());
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

BOOST_AUTO_TEST_SUITE_END()
