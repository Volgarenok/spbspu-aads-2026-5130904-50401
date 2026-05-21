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
