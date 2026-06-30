#include <boost/test/unit_test.hpp>
#include <mylist.hpp>

BOOST_AUTO_TEST_SUITE(List_suite)
BOOST_AUTO_TEST_CASE(designer_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.begin() == goltsov::detail::makeLIterByPtr< int >(nullptr));
  a.push_start(1);
  BOOST_CHECK(a.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
  goltsov::List< int > b(a);
  BOOST_CHECK(b.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
  goltsov::LIter< int > temp = b.begin();
  goltsov::List< int > bc(std::move(b));
  BOOST_CHECK(temp == bc.begin());
  BOOST_CHECK(bc.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
  goltsov::List< int > bo = a;
  BOOST_CHECK(bo.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
  goltsov::LIter< int > tempo = bo.begin();
  goltsov::List< int > boc = std::move(bo);
  BOOST_CHECK(tempo == boc.begin());
  BOOST_CHECK(boc.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.begin() == goltsov::detail::makeLIterByPtr< int >(nullptr));
  BOOST_REQUIRE_THROW(* a.begin(), std::runtime_error);
  a.push_start(1);
  BOOST_CHECK(a.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
  BOOST_CHECK_NO_THROW(*a.begin());

  const goltsov::List< int > b;
  BOOST_CHECK(b.cbegin() == goltsov::detail::makeLCIterByPtr< int >(nullptr));
  BOOST_REQUIRE_THROW(*b.cbegin(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.end() == goltsov::detail::makeLIterByPtr< int >(nullptr));

  const goltsov::List< int > b;
  BOOST_CHECK(b.cend() == goltsov::detail::makeLCIterByPtr< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(getLast_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.getLast() == a.beforeBegin());
  const goltsov::List< int > b;
  BOOST_CHECK(b.getLast() == b.beforeBegin());
  a.push_start(1);
  BOOST_CHECK(a.getLast() != goltsov::detail::makeLIterByPtr< int >(nullptr) && (*a.getLast()) == 1);
  const goltsov::List< int > b1 = a;
  BOOST_CHECK(b1.getLast() != goltsov::detail::makeLCIterByPtr< int >(nullptr) && (*b1.getLast()) == 1);
  a.insertAfter(a.begin(), 2);
  BOOST_CHECK(a.getLast() != goltsov::detail::makeLIterByPtr< int >(nullptr) && (*a.getLast()) == 2);
  const goltsov::List< int > b2 = a;
  BOOST_CHECK(b2.getLast() != goltsov::detail::makeLCIterByPtr< int >(nullptr) && (*b2.getLast()) == 2);
}

BOOST_AUTO_TEST_CASE(push_start_test)
{
  goltsov::List< int > a;
  BOOST_CHECK(a.push_start(1) != goltsov::detail::makeLIterByPtr< int >(nullptr) && (* a.begin()) == 1);
}

BOOST_AUTO_TEST_CASE(pop_start_test)
{
  goltsov::List< int > a;
  a.push_start(2);
  a.push_start(1);
  a.pop_start();
  BOOST_CHECK((* a.begin()) == 2);
  a.pop_start();
  a.pop_start();
  BOOST_CHECK(a.begin() == goltsov::detail::makeLIterByPtr< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(pop_end_test)
{
  goltsov::List< int > a;
  a.push_start(2);
  a.push_start(1);
  a.pop_end();
  BOOST_CHECK((* a.begin()) == 1);
  a.pop_end();
  a.pop_end();
  BOOST_CHECK(a.begin() == goltsov::detail::makeLIterByPtr< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(insert_test)
{
  goltsov::List< int > a;
  BOOST_CHECK((*a.insertAfter(a.beforeBegin(), 1)) == 1 && a.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
  BOOST_CHECK((*a.insertAfter(a.begin(), 2)) == 2);
  int c1 = 1;
  int c2 = 2;
  a.clear();
  BOOST_CHECK((*a.insertAfter(a.beforeBegin(), c1)) == 1 && a.begin() != goltsov::detail::makeLIterByPtr< int >(nullptr));
  BOOST_CHECK((*a.insertAfter(a.begin(), c2)) == 2);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  goltsov::List< int > a;
  a.clear();
  BOOST_CHECK(a.begin() == goltsov::detail::makeLIterByPtr< int >(nullptr));
  a.push_start(1);
  a.push_start(2);
  a.clear();
  BOOST_CHECK(a.begin() == goltsov::detail::makeLIterByPtr< int >(nullptr));
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  goltsov::List< int > a;
  auto it = a.beforeBegin();
  for (int i = 0; i < 5; ++i)
  {
    it = a.insertAfter(it, i);
  }
  auto t = ++(++a.begin());
  BOOST_CHECK(a.eraseAfter(a.begin()) == t && a.size() == 4 && (*(++a.begin())) == 2);
  BOOST_CHECK_THROW(a.eraseAfter(a.getLast()), std::runtime_error);
  BOOST_CHECK_THROW(a.eraseAfter(a.end()), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(spliceAfter_test)
{
  goltsov::List< int > a;
  auto it = a.beforeBegin();
  for (int i = 0; i < 5; ++i)
  {
    it = a.insertAfter(it, i);
  }
  goltsov::List< int > b;
  auto itb = b.beforeBegin();
  for (int i = 8; i >= 6; --i)
  {
    itb = b.insertAfter(itb, i);
  }
  a.spliceAfter(a.beforeBegin(), b);
  int expected1[] = {8, 7, 6, 0, 1, 2, 3, 4};
  auto ito = a.begin();
  for (size_t i = 0; i < 8; ++i, ++ito)
  {
    BOOST_CHECK((*ito) == expected1[i]);
  }
  BOOST_CHECK(b.empty());
  a.spliceAfter(a.beforeBegin(), a);
  ito = a.begin();
  for (size_t i = 0; i < 8; ++i, ++ito)
  {
    BOOST_CHECK((*ito) == expected1[i]);
  }
  goltsov::List< int > c;
  auto itc = c.beforeBegin();
  for (int i = 8; i >= 6; --i)
  {
    itc = c.insertAfter(itc, i);
  }
  auto pos = a.begin();
  auto it_to_move = c.beforeBegin();
  a.spliceAfter(pos, c, it_to_move);
  int expected2[] = {8, 8, 7, 6, 0, 1, 2, 3, 4};
  ito = a.begin();
  for (size_t i = 0; i < 9; ++i, ++ito)
  {
    BOOST_CHECK((*ito) == expected2[i]);
  }
  BOOST_CHECK(c.size() == 2);
  pos = a.begin();
  ++pos;
  it_to_move = a.beforeBegin();
  a.spliceAfter(pos, a, it_to_move);
  ito = a.begin();
  for (size_t i = 0; i < 9; ++i, ++ito)
  {
    BOOST_CHECK((*ito) == expected2[i]);
  }
  goltsov::List< int > d;
  auto itd = d.beforeBegin();
  for (int i = 10; i < 13; ++i)
  {
    itd = d.insertAfter(itd, i);
  }
  auto first = d.beforeBegin();
  auto last = d.begin();
  ++last;
  ++last;
  pos = a.begin();
  ++pos;
  ++pos;
  a.spliceAfter(pos, d, first, last);
  int expected3[] = {8, 8, 7, 10, 11, 6, 0, 1, 2, 3, 4};
  ito = a.begin();
  for (size_t i = 0; i < 11; ++i, ++ito)
  {
    BOOST_CHECK((*ito) == expected3[i]);
  }
  BOOST_CHECK(d.size() == 1);
  BOOST_CHECK(*d.begin() == 12);
  first = a.beforeBegin();
  last = a.begin();
  ++last;
  ++last;
  pos = a.begin();
  ++pos;
  ++pos;
  ++pos;
  ++pos;
  a.spliceAfter(pos, a, first, last);
  ito = a.begin();
  int expected4[] = {7, 10, 11, 8, 8, 6, 0, 1, 2, 3, 4};
  for (size_t i = 0; i < 11; ++i, ++ito)
  {
    BOOST_CHECK((*ito) == expected4[i]);
  }
  goltsov::List< int > empty;
  a.spliceAfter(a.beforeBegin(), empty);
  BOOST_CHECK(a.size() == 11);
  BOOST_CHECK(empty.empty());
  size_t a_size = a.size();
  goltsov::List< int > e;
  auto ite = e.beforeBegin();
  for (int i = 20; i < 23; ++i)
  {
    ite = e.insertAfter(ite, i);
  }
  size_t e_size = e.size();
  a.spliceAfter(a.beforeBegin(), e);
  BOOST_CHECK(a.size() == a_size + e_size);
  BOOST_CHECK(e.size() == 0);
}

BOOST_AUTO_TEST_CASE(merge_test)
{
  int a[] = {1, 2, 3, 4, 5};
  int b[] = {2, 6, 8};
  goltsov::List< int > al;
  goltsov::List< int > bl;
  goltsov::LIter< int > ita = al.beforeBegin();
  for (size_t i = 0; i < 5; ++i)
  {
    ita = al.insertAfter(ita, a[i]);
  }
  goltsov::LIter< int > itb = bl.beforeBegin();
  for (size_t i = 0; i < 3; ++i)
  {
    itb = bl.insertAfter(itb, b[i]);
  }
  goltsov::LIter< int > iter = bl.begin();
  ++iter;
  al.merge(bl, std::less< int >{});
  int exp[] = {1, 2, 2, 3, 4, 5, 6, 8};
  goltsov::LIter< int > ite = al.begin();
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_CHECK(*ite == exp[i]);
    ++ite;
  }
  BOOST_CHECK(*iter == 6);
}

BOOST_AUTO_TEST_CASE(sort_test)
{
  int a[] = {9, 4, 7, 8, 1, 4, 7, 3};
  goltsov::List< int > al;
  goltsov::LIter< int > it = al.beforeBegin();
  for (size_t i = 0; i < 8; ++i)
  {
    al.insertAfter(it, a[i]);
  }
  al.sort(std::less< int >{});
  int exp[] = {1, 3, 4, 4, 7, 7, 8, 9};
  goltsov::LIter< int > ite = al.begin();
  for (size_t i = 0; i < 8; ++i)
  {
    BOOST_CHECK(*ite == exp[i]);
    ++ite;
  }
}

BOOST_AUTO_TEST_CASE(partition_test)
{
  int a[] = {9, 4, 7, 8, 1, 4, 7, 3};
  goltsov::List<int> al;
  goltsov::LIter<int> it = al.beforeBegin();
  for (size_t i = 0; i < 8; ++i)
  {
      it = al.insertAfter(it, a[i]);
  }
  al.partition([](int q){return q > 4;});
  int exp[] = {9, 7, 8, 7, 4, 1, 4, 3};
  goltsov::LIter<int> ite = al.begin();
  for (size_t i = 0; i < 8; ++i, ++ite)
  {
      BOOST_CHECK(*ite == exp[i]);
  }
  goltsov::List<int> empty;
  empty.partition([](int q){return q > 4;});
  BOOST_CHECK(empty.empty());
  BOOST_CHECK(empty.size() == 0);
  goltsov::List<int> one;
  one.push_start(5);
  one.partition([](int q){return q > 4;});
  BOOST_CHECK(one.size() == 1);
  BOOST_CHECK(*one.begin() == 5);
  int all_true[] = {9, 7, 8, 7, 6, 5};
  goltsov::List<int> at;
  it = at.beforeBegin();
  for (size_t i = 0; i < 6; ++i)
  {
    it = at.insertAfter(it, all_true[i]);
  }
  at.partition([](int q){return q > 4;});
  ite = at.begin();
  for (size_t i = 0; i < 6; ++i, ++ite)
  {
    BOOST_CHECK(*ite == all_true[i]);
  }
  int all_false[] = {3, 1, 4, 1, 3, 2};
  goltsov::List<int> af;
  it = af.beforeBegin();
  for (size_t i = 0; i < 6; ++i)
  {
    it = af.insertAfter(it, all_false[i]);
  }
  af.partition([](int q){return q > 4;});
  ite = af.begin();
  for (size_t i = 0; i < 6; ++i, ++ite)
  {
    BOOST_CHECK(*ite == all_false[i]);
  }
  int sorted[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  goltsov::List<int> as;
  it = as.beforeBegin();
  for (size_t i = 0; i < 9; ++i)
  {
    it = as.insertAfter(it, sorted[i]);
  }
  as.partition([](int q){return q > 4;});
  int exp_sorted[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  ite = as.begin();
  for (size_t i = 0; i < 9; ++i, ++ite)
  {
    BOOST_CHECK(*ite == exp_sorted[i]);
  }
  int reverse[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  goltsov::List<int> ar;
  it = ar.beforeBegin();
  for (size_t i = 0; i < 9; ++i)
  {
    it = ar.insertAfter(it, reverse[i]);
  }
  ar.partition([](int q){return q > 4;});
  int exp_reverse[] = {5, 6, 7, 8, 9, 1, 2, 3, 4};
  ite = ar.begin();
  for (size_t i = 0; i < 9; ++i, ++ite)
  {
    BOOST_CHECK(*ite == exp_reverse[i]);
  }
  int duplicates[] = {4, 4, 4, 4, 5, 5, 5, 5};
  goltsov::List<int> ad;
  it = ad.beforeBegin();
  for (size_t i = 0; i < 8; ++i)
  {
    it = ad.insertAfter(it, duplicates[i]);
  }
  ad.partition([](int q){return q > 4;});
  int exp_duplicates[] = {5, 5, 5, 5, 4, 4, 4, 4};
  ite = ad.begin();
  for (size_t i = 0; i < 8; ++i, ++ite)
  {
    BOOST_CHECK(*ite == exp_duplicates[i]);
  }
}

BOOST_AUTO_TEST_SUITE_END()
