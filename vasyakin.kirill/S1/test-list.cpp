#include <boost/test/unit_test.hpp>
#include "../common/list.hpp"

auto l = [](int x)
{
  return x % 2 == 0;
};

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(DefConstructTest)
{
  vasyakin::List< int > list;
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(ValConstructTest)
{
  vasyakin::List< int > list(52);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK(list.begin() != list.end());
  BOOST_CHECK_EQUAL(*list.begin(), 52);
}

BOOST_AUTO_TEST_CASE(DestructEmptyListTest)
{
  vasyakin::List< int > list;
}

BOOST_AUTO_TEST_CASE(DestructSingleElemTest)
{
  vasyakin::List< int > list(52);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(DestructMultElemTest)
{
  vasyakin::List< int > list;
  for (size_t i = 0; i < 10; ++i)
  {
    list.pushBack(i);
  }
  BOOST_CHECK_EQUAL(list.getSize(), 10);
}

BOOST_AUTO_TEST_CASE(DestructAfterCopyTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > copy(list);
  BOOST_CHECK_EQUAL(list.getSize(), 2);
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(DestructAfterMoveTest)
{
  vasyakin::List< int > list;
  list.pushBack(52);
  vasyakin::List< int > moved(std::move(list));
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 1);
}

BOOST_AUTO_TEST_CASE(CopyConstructTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > copy(list);
  list.pushBack(3);
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
  copy.pushBack(52);
  BOOST_CHECK_EQUAL(list.getSize(), 3);
  auto it = copy.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 52);
}

BOOST_AUTO_TEST_CASE(MoveConstructTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > moved(std::move(list));
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 2);
  auto it = moved.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  moved.pushBack(99);
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 3);
}

BOOST_AUTO_TEST_CASE(CopyAssignTest)
{
  vasyakin::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  vasyakin::List< int > list2;
  list2.pushBack(10);
  list2 = list1;
  BOOST_CHECK_EQUAL(list2.getSize(), 2);
  auto it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  list1 = list1;
  BOOST_CHECK_EQUAL(list1.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  vasyakin::List< int > moved;
  moved = std::move(list);
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK_EQUAL(moved.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(EraseTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.erase(list.end());
  BOOST_CHECK_EQUAL(list.getSize(), 2);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
  auto it = list.begin();
  for (size_t i = 0; i < list.getSize() - 2; ++i)
  {
    ++it;
  }
  list.erase(it);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 2);
  list.erase(list.end());
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.erase(list.end());
  BOOST_CHECK_EQUAL(list.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  vasyakin::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);
  list.clear();
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  BOOST_CHECK(list.begin() == list.end());
  list.clear();
  BOOST_CHECK_EQUAL(list.getSize(), 0);
  list.pushBack(42);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 42);
}

BOOST_AUTO_TEST_CASE(PushBackTest)
{
  vasyakin::List< int > list;
  list.pushBack(10);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 10);
  list.pushBack(20);
  list.pushBack(30);
  BOOST_CHECK_EQUAL(list.getSize(), 3);
  auto it = list.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
  ++it;
  BOOST_CHECK_EQUAL(*it, 30);
  ++it;
  BOOST_CHECK(it == list.end());
  list.clear();
  list.pushBack(42);
  BOOST_CHECK_EQUAL(list.getSize(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 42);
}

BOOST_AUTO_TEST_CASE(InsertTest)
{
  vasyakin::List< int > list;
  list.insert(list.end(), 1);
  BOOST_CHECK_EQUAL(*list.begin(), 1);
  list.insert(list.end(), 0);
  BOOST_CHECK_EQUAL(*list.begin(), 0);
  auto it = list.begin();
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(list.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  vasyakin::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  vasyakin::List< int > list2;
  list2.pushBack(10);
  list1.swap(list2);
  BOOST_CHECK_EQUAL(list1.getSize(), 1);
  BOOST_CHECK_EQUAL(*list1.begin(), 10);
  BOOST_CHECK_EQUAL(list2.getSize(), 2);
  BOOST_CHECK_EQUAL(*list2.begin(), 1);
}

BOOST_AUTO_TEST_CASE(ConstIteratorTest)
{
  vasyakin::List< int > list;
  list.pushBack(10);
  list.pushBack(20);
  const vasyakin::List< int >& const_list = list;
  auto cit = const_list.cbegin();
  BOOST_CHECK_EQUAL(*cit, 10);
  ++cit;
  BOOST_CHECK_EQUAL(*cit, 20);
  ++cit;
  BOOST_CHECK(cit == const_list.cend());
  BOOST_CHECK(const_list.cbegin() != const_list.cend());
  auto cit2 = const_list.begin();
  BOOST_CHECK_EQUAL(*cit2, 10);
  vasyakin::List< int > empty;
  BOOST_CHECK(empty.cbegin() == empty.cend());
}

BOOST_AUTO_TEST_CASE(SpliceAllTest)
{
  vasyakin::List< int > a;
  a.pushBack(1);
  a.pushBack(2);

  vasyakin::List< int > b;
  b.pushBack(10);
  b.pushBack(20);
  b.pushBack(30);

  a.splice_after(a.begin(), b);

  BOOST_CHECK_EQUAL(a.getSize(), 5);
  BOOST_CHECK_EQUAL(b.getSize(), 0);

  auto it = a.begin();
  int expected[] = {1, 10, 20, 30, 2};
  for (auto v : expected)
  {
    BOOST_CHECK_EQUAL(*it, v);
    ++it;
  }

  BOOST_CHECK(it == a.end());
}

BOOST_AUTO_TEST_CASE(SpliceSingleTest)
{
  vasyakin::List< int > a;
  a.pushBack(1);
  a.pushBack(3);

  vasyakin::List< int > b;
  b.pushBack(10);
  b.pushBack(20);

  a.splice_after(a.begin(), b, b.begin());

  BOOST_CHECK_EQUAL(a.getSize(), 3);
  BOOST_CHECK_EQUAL(b.getSize(), 1);
  BOOST_CHECK_EQUAL(*b.begin(), 10);

  auto it = a.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;

  BOOST_CHECK_EQUAL(*it, 20);
  ++it;

  BOOST_CHECK_EQUAL(*it, 3);
  ++it;

  BOOST_CHECK(it == a.end());
}

BOOST_AUTO_TEST_CASE(SpliceRangeTest)
{
  vasyakin::List< int > a;
  a.pushBack(1);
  a.pushBack(5);

  vasyakin::List< int > b;
  b.pushBack(10);
  b.pushBack(20);
  b.pushBack(30);
  b.pushBack(40);

  auto first = b.begin();
  auto last = b.begin();
  ++last;
  ++last;
  ++last;

  a.splice_after(a.begin(), b, first, last);

  BOOST_CHECK_EQUAL(a.getSize(), 4);
  BOOST_CHECK_EQUAL(b.getSize(), 2);

  auto check = a.begin();

  BOOST_CHECK_EQUAL(*check, 1);
  ++check;

  BOOST_CHECK_EQUAL(*check, 20);
  ++check;

  BOOST_CHECK_EQUAL(*check, 30);
  ++check;

  BOOST_CHECK_EQUAL(*check, 5);
  ++check;

  BOOST_CHECK(check == a.end());
}

BOOST_AUTO_TEST_CASE(MergeSortedTest)
{
  vasyakin::List< int > a;
  a.pushBack(1);
  a.pushBack(3);
  a.pushBack(5);

  vasyakin::List< int > b;
  b.pushBack(2);
  b.pushBack(4);
  b.pushBack(6);

  a.merge(b);
  BOOST_CHECK_EQUAL(a.getSize(), 6);
  BOOST_CHECK_EQUAL(b.getSize(), 0);

  auto it = a.begin();
  int expected[] = {1, 2, 3, 4, 5, 6};
  for (auto v : expected)
  {
    BOOST_CHECK_EQUAL(*it, v);
    ++it;
  }

  BOOST_CHECK(it == a.end());
}

BOOST_AUTO_TEST_CASE(SortTest)
{
  vasyakin::List< int > list;
  list.pushBack(5);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(2);
  list.pushBack(3);

  list.sort();
  BOOST_CHECK_EQUAL(list.getSize(), 5);

  auto it = list.begin();
  for (int i = 1; i <= 5; ++i)
  {
    BOOST_CHECK_EQUAL(*it, i);
    ++it;
  }

  BOOST_CHECK(it == list.end());

  vasyakin::List< int > empty;
  empty.sort();
  BOOST_CHECK_EQUAL(empty.getSize(), 0);

  vasyakin::List< int > single(42);
  single.sort();
  BOOST_CHECK_EQUAL(*single.begin(), 42);
}

BOOST_AUTO_TEST_CASE(PartitionTest)
{
  vasyakin::List< int > list;
  list.pushBack(3);
  list.pushBack(1);
  list.pushBack(4);
  list.pushBack(1);
  list.pushBack(5);
  list.pushBack(9);
  list.pushBack(2);
  list.pushBack(6);

  auto it = list.partition(l);

  int expected[] = {4, 2, 6, 3, 1, 1, 5, 9};
  auto check = list.begin();
  for (auto v : expected)
  {
    BOOST_CHECK_EQUAL(*check, v);
    ++check;
  }

  BOOST_CHECK(check == list.end());
  BOOST_CHECK(it != list.end());

  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(PartitionAllTrueTest)
{
  vasyakin::List< int > list;
  list.pushBack(2);
  list.pushBack(4);
  list.pushBack(6);

  auto it = list.partition(l);

  auto check = list.begin();
  BOOST_CHECK_EQUAL(*check, 2);
  ++check;

  BOOST_CHECK_EQUAL(*check, 4);
  ++check;

  BOOST_CHECK_EQUAL(*check, 6);
  ++check;

  BOOST_CHECK(check == list.end());
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_SUITE_END()
