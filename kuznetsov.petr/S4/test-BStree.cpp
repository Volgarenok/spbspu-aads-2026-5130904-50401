#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>

#include "./BSTree.hpp"

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tree;
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.height() == 0);
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2(tr1);
  BOOST_TEST(tr2.size() == 1);
  BOOST_TEST(tr2.at(1) == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2(std::move(tr1));
  BOOST_TEST(tr2.size() == 1);
  BOOST_TEST(tr2.at(1) == 2);
  BOOST_TEST(tr1.size() == 0);
}

BOOST_AUTO_TEST_CASE(CopyAssignmentOperatorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2;
  tr2 = tr1;
  BOOST_TEST(tr2.size() == 1);
  BOOST_TEST(tr2.at(1) == 2);
  BOOST_TEST(tr1.size() == 1);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentOperatorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2;
  tr2 = std::move(tr1);
  BOOST_TEST(tr2.size() == 1);
  BOOST_TEST(tr2.at(1) == 2);
  BOOST_TEST(tr1.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushAndAtTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  BOOST_TEST(tr1.size() == 0);
  tr1.push(143, 2);
  tr1.push(32, 1);
  tr1.push(72, 3);
  BOOST_TEST(tr1.size() == 3);
  BOOST_TEST(tr1.at(143) == 2);
  BOOST_TEST(tr1.at(32) == 1);
  BOOST_TEST(tr1.at(72) == 3);
}

BOOST_AUTO_TEST_CASE(DropTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  int ks[]{ 10, 5, 15, 2, 9, 13 };
  int vs[]{ 1, 2, 3, 4, 5, 6 };
  for (size_t i = 0; i < 6; ++i) {
    tr1.push(ks[i], vs[i]);
  }
  BOOST_TEST(tr1.size() == 6);
  for (size_t i = 0; i < 6; ++i) {
    tr1.drop(ks[i]);
    BOOST_TEST(tr1.size() == 6 - i - 1);
    for (size_t j = 0; j < 6; ++j) {
      if (j <= i) {
        BOOST_CHECK_THROW(tr1.at(ks[j]), std::logic_error);
      } else {
        BOOST_TEST(tr1.at(ks[j]) == vs[j]);
      }
    }
  }
  BOOST_CHECK_THROW(tr1.drop(10), std::logic_error);
}

BOOST_AUTO_TEST_CASE(HeightAndIteratorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  BOOST_CHECK(tr1.begin() == tr1.end());
  int ks[]{ 10, 5, 15, 2, 9, 13 };
  int vs[]{ 1, 2, 3, 4, 5, 6 };
  int hs[]{ 1, 2, 1, 3, 1, 2 };
  for (size_t i = 0; i < 6; ++i) {
    tr1.push(ks[i], vs[i]);
  }
  int sks[]{ 2, 5, 9, 10, 13, 15 };
  int svs[]{ 4, 2, 5, 1, 6, 3 };
  BOOST_TEST(tr1.height() == 3);
  auto it = tr1.cbegin();
  size_t i = 0;
  for (; it != tr1.cend(); ++it) {
    BOOST_TEST(tr1.height(it) == hs[i]);
    BOOST_TEST(it->first == sks[i]);
    BOOST_TEST(it->second == svs[i]);
    BOOST_TEST((*it).first == sks[i]);
    BOOST_TEST((*it).second == svs[i]);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(leftRotateTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > testing;
  kuznetsov::BSTree< int, int, std::less< int > > model;

  testing.push(10, 1);
  testing.push(15, 1);
  testing.push(6, 1);
  testing.push(13, 1);
  testing.push(18, 1);
  auto it = testing.cbegin();
  ++it;
  ++it;
  ++it;
  testing.rotateLeft(it);
  --it;
  --it;
  BOOST_CHECK_THROW(testing.rotateLeft(it), std::logic_error);
  model.push(15, 1);
  model.push(10, 1);
  model.push(18, 1);
  model.push(6, 1);
  model.push(13, 1);
  auto ittest = testing.cbegin();
  auto modelTest = model.cbegin();
  for (; ittest != testing.cend(); ++ittest, ++modelTest) {
    BOOST_TEST(testing.height(ittest) == model.height(modelTest));
  }
}

BOOST_AUTO_TEST_CASE(rightRotateTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > testing;
  kuznetsov::BSTree< int, int, std::less< int > > model;

  testing.push(15, 1);
  testing.push(10, 1);
  testing.push(18, 1);
  testing.push(6, 1);
  testing.push(13, 1);
  auto it = testing.cbegin();
  ++it;
  testing.rotateRight(it);
  model.push(10, 1);
  model.push(15, 1);
  model.push(6, 1);
  model.push(13, 1);
  model.push(18, 1);
  ++it;
  ++it;
  BOOST_CHECK_THROW(testing.rotateRight(it), std::logic_error);
  auto ittest = testing.cbegin();
  auto modelTest = model.cbegin();
  for (; ittest != testing.cend(); ++ittest, ++modelTest) {
    BOOST_TEST(testing.height(ittest) == model.height(modelTest));
  }
}

BOOST_AUTO_TEST_CASE(leftLargeRotateTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > testing;
  kuznetsov::BSTree< int, int, std::less< int > > model;

  testing.push(10, 1);
  testing.push(15, 1);
  testing.push(6, 1);
  testing.push(12, 1);
  testing.push(18, 1);
  testing.push(11, 1);
  testing.push(13, 1);
  auto it = testing.cbegin();
  ++it;
  BOOST_CHECK_THROW(testing.rotateLargeLeft(it), std::logic_error);
  ++it;
  ++it;
  testing.rotateLargeLeft(it);
  model.push(12, 1);
  model.push(10, 1);
  model.push(6, 1);
  model.push(11, 1);
  model.push(15, 1);
  model.push(13, 1);
  model.push(18, 1);
  auto ittest = testing.cbegin();
  auto modelTest = model.cbegin();
  for (; ittest != testing.cend(); ++ittest, ++modelTest) {
    BOOST_TEST(testing.height(ittest) == model.height(modelTest));
  }
}

BOOST_AUTO_TEST_CASE(rightLargeRotateTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > testing;
  kuznetsov::BSTree< int, int, std::less< int > > model;

  testing.push(10, 1);
  testing.push(3, 1);
  testing.push(15, 1);
  testing.push(1, 1);
  testing.push(7, 1);
  testing.push(5, 1);
  testing.push(8, 1);
  auto it = testing.cbegin();
  ++it;
  BOOST_CHECK_THROW(testing.rotateLargeRight(it), std::logic_error);
  ++it;
  ++it;
  testing.rotateLargeRight(it);
  model.push(7, 1);
  model.push(3, 1);
  model.push(10, 1);
  model.push(1, 1);
  model.push(5, 1);
  model.push(15, 1);
  model.push(8, 1);
  auto ittest = testing.cbegin();
  auto modelTest = model.cbegin();
  for (; ittest != testing.cend(); ++ittest, ++modelTest) {
    BOOST_TEST(testing.height(ittest) == model.height(modelTest));
  }
}
