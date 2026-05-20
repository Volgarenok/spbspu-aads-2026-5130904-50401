#include <boost/test/unit_test.hpp>
#include <bstree.hpp>

BOOST_AUTO_TEST_SUITE(bstree_special_members_tests)

BOOST_AUTO_TEST_CASE(test_default_constructor)
{
  chernov::BSTree< int, int, std::less< int > > bstree;
}

BOOST_AUTO_TEST_SUITE_END()
