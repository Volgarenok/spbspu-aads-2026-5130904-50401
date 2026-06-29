#include <boost/test/unit_test.hpp>
#include <string>
#include <utility>
#include "mybst.hpp"

namespace goltsov
{
  using bst_si = goltsov::BSTree< std::string, int, std::less< std::string > >;
}

BOOST_AUTO_TEST_SUITE(BST_suite)

BOOST_AUTO_TEST_CASE(constructors_test)
{
  goltsov::bst_si b;
  BOOST_CHECK(b.height() == 0);
  b.insert(std::pair< std::string, int >{"a", 1});
  b.insert(std::pair< std::string, int >{"b", 2});
  goltsov::bst_si b2 (b);
  goltsov::bst_si b3 = b2;
  BOOST_CHECK(b2.at("a") == 1 && b2.at("b") == 2 && b3.at("a") == 1 && b3.at("b") == 2);
  goltsov::bst_si b4 (std::move(b2));
  goltsov::bst_si b5 = std::move(b3);
  BOOST_CHECK(b4.at("a") == 1 && b4.at("b") == 2 && b5.at("a") == 1 && b5.at("b") == 2);
}

BOOST_AUTO_TEST_CASE(insert_at_erase_case)
{
  goltsov::bst_si b;
  b.insert(std::pair< std::string, int >{"a", 1});
  b.insert(std::pair< std::string, int >{"b", 2});
  BOOST_CHECK(b.insert(std::pair< std::string, int >{"a", 1}).second == false);
  BOOST_CHECK(b.at("a") == 1 && b.at("b") == 2);
  BOOST_CHECK_THROW(b.at("c"), std::logic_error);
  BOOST_CHECK(b.erase("a") == 1);
  BOOST_CHECK(b.erase("a") == 0);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  goltsov::bst_si b;
  b.insert(std::pair< std::string, int >{"a", 1});
  b.insert(std::pair< std::string, int >{"b", 2});
  goltsov::bst_si b1;
  b.swap(b1);
  BOOST_CHECK(b.height() == 0 && b1.at("a") == 1 && b1.at("b") == 2);
}

BOOST_AUTO_TEST_CASE(begin_and_test)
{
  goltsov::bst_si b;
  b.insert(std::pair< std::string, int >{"a", 1});
  b.insert(std::pair< std::string, int >{"b", 2});
  const goltsov::bst_si bc = b;
  goltsov::BSTIterator< std::string, int > temp = goltsov::BSTIterator< std::string, int >{};
  goltsov::BSTConstIterator< std::string, int > tempc = goltsov::BSTConstIterator< std::string, int >{};
  BOOST_CHECK((* b.begin()).second == 1 && b.end() == temp &&
    (* bc.begin()).second == 1 && bc.end() == tempc);
}

BOOST_AUTO_TEST_CASE(rotates_test)
{
  goltsov::bst_si b;
  b.insert(std::pair< std::string, int >{"a", 1});
  b.insert(std::pair< std::string, int >{"e", 2});
  b.insert(std::pair< std::string, int >{"g", 3});
  BOOST_CHECK_THROW(b.rotateLeft(b.begin()), std::logic_error);
  auto i1 = ++(b.begin());
  auto i2 = ++(++(b.begin()));
  BOOST_CHECK(b.rotateLeft(i1) == i2 && b.height() == 2);
  b.insert(std::pair< std::string, int >{"k", 4});
  b.insert(std::pair< std::string, int >{"i", 5});
  i1 = ++(++(++b.begin()));
  BOOST_CHECK_THROW(b.rotateLeft(i1), std::logic_error);
  i1 = ++(++(++(++b.begin())));
  BOOST_CHECK(b.rotateLeft(i1) ==
    (goltsov::BSTIterator< std::string, int >{}) && b.height() == 4);
  goltsov::bst_si b1;
  b1.insert(std::pair< std::string, int >{"e", 3});
  b1.insert(std::pair< std::string, int >{"d", 2});
  b1.insert(std::pair< std::string, int >{"c", 1});
  i1 = ++(++(b1.begin()));
  BOOST_CHECK_THROW(b1.rotateRight(i1), std::logic_error);
  i1 = ++(b1.begin());
  BOOST_CHECK(b1.rotateRight(i1) == b1.begin() &&  b1.height() == 2);
  b1.insert(std::pair< std::string, int >{"a", 4});
  b1.insert(std::pair< std::string, int >{"b", 5});
  i1 = ++(b.begin());
  BOOST_CHECK_THROW(b.rotateRight(i1), std::logic_error);
  BOOST_CHECK(b.rotateRight(b.begin()) == (goltsov::BSTIterator< std::string, int >{}) && b.height() == 5);
  goltsov::bst_si b2;
  b2.insert(std::pair< std::string, int >{"c", 3});
  b2.insert(std::pair< std::string, int >{"a", 1});
  b2.insert(std::pair< std::string, int >{"b", 2});
  i1 = ++(b2.begin());
  BOOST_CHECK(b2.rotateLargeRight(i1)
    == (goltsov::BSTConstIterator< std::string, int >{}) && b2.height() == 2);
  goltsov::bst_si b3;
  b3.insert(std::pair< std::string, int >{"a", 1});
  b3.insert(std::pair< std::string, int >{"c", 3});
  b3.insert(std::pair< std::string, int >{"b", 2});
  i1 = ++(b3.begin());
  BOOST_CHECK(b3.rotateLargeLeft(i1)
    == (goltsov::BSTConstIterator< std::string, int >{}) && b3.height() == 2);
}

BOOST_AUTO_TEST_CASE(height_test)
{
  goltsov::bst_si b;
  b.insert(std::pair< std::string, int >{"a", 1});
  b.insert(std::pair< std::string, int >{"e", 2});
  b.insert(std::pair< std::string, int >{"g", 3});
  b.insert(std::pair< std::string, int >{"k", 4});
  b.insert(std::pair< std::string, int >{"i", 5});
  BOOST_CHECK((b.height() == 5) && (b.height(++(b.begin())) == 4));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BSTIterators_suite)

using nb_si = goltsov::detail::NodeBST< std::string, int >;
using bi_si = goltsov::BSTIterator< std::string, int >;
using bci_si = goltsov::BSTConstIterator< std::string, int >;

BOOST_AUTO_TEST_CASE(constructors_test)
{
  bi_si it1, it2 (it1);
  bci_si cit1, cit2 (cit1);

  BOOST_CHECK(it1.getPtr() == nullptr && cit1.getPtr() == nullptr);
  BOOST_CHECK(it2.getPtr() == nullptr && cit2.getPtr() == nullptr);

  it1 = it2;
  cit1 = cit2;
  BOOST_CHECK(it1.getPtr() == nullptr && cit1.getPtr() == nullptr);

  it1 = std::move(it2);
  cit1 = std::move(cit2);
  BOOST_CHECK(it1.getPtr() == nullptr && cit1.getPtr() == nullptr);
  BOOST_CHECK(it2.getPtr() == nullptr && cit2.getPtr() == nullptr);
}

BOOST_AUTO_TEST_CASE(test_operators)
{
  nb_si* n1 = new nb_si {{"a", 5}, nullptr, nullptr, nullptr};
  nb_si* n2 = new nb_si {{"b", 3}, nullptr, nullptr, nullptr};

  bi_si it1 = goltsov::detail::makeBSTIterByPtr(n1);
  bi_si it2 = goltsov::detail::makeBSTIterByPtr(n1);
  bi_si it3 = goltsov::detail::makeBSTIterByPtr(n2);
  bci_si cit1 = goltsov::detail::makeBSTConstIterByPtr(n1);
  bci_si cit2 = goltsov::detail::makeBSTConstIterByPtr(n1);
  bci_si cit3 = goltsov::detail::makeBSTConstIterByPtr(n2);

  BOOST_CHECK((++it1).getPtr() == it1.getPtr());
  BOOST_CHECK((++cit1).getPtr() == cit1.getPtr());

  BOOST_CHECK(& (* it2) == it2.operator->());
  BOOST_CHECK(& (* cit2) == cit2.operator->());

  BOOST_CHECK(it1 == ++it2 && cit1 == ++cit2);
  BOOST_CHECK(it1 != it3 && cit1 != cit3);
  BOOST_CHECK(it2 == cit2 && cit2 == it2);
  BOOST_CHECK(it2 != cit3 && cit3 != it2);

  bci_si cit_from_it = it3;
  BOOST_CHECK(cit_from_it.getPtr() == n2);

  delete n1;
  delete n2;
}

BOOST_AUTO_TEST_SUITE_END()
