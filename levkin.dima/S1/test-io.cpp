#include "io.hpp"
#include <boost/test/unit_test.hpp>
using namespace levkin;

BOOST_AUTO_TEST_CASE(read_data_test) {
  std::stringstream input("first 1 1 1\nsecond 2 2 2 2\nthird\nfourth 4 4");
  Data data;

  readData(input, data);
  auto it = data.cbegin();

  BOOST_CHECK_EQUAL(it->first, "first");
  BOOST_CHECK_EQUAL(*(it->second.cbegin()), 1);

  ++it;
  ++it;
  BOOST_CHECK_EQUAL(it->first, "third");
  BOOST_CHECK(it->second.cbegin() == it->second.cend());
}

BOOST_AUTO_TEST_CASE(output_simulator_test) {
  Data data;

  Lst l1;
  l1.pushBack(1);
  l1.pushBack(1);
  l1.pushBack(1);
  Lst l2;
  l2.pushBack(2);
  l2.pushBack(2);
  l2.pushBack(2);
  l2.pushBack(2);
  Lst l3;
  Lst l4;
  l4.pushBack(4);
  l4.pushBack(4);

  data.pushBack({"first", l1});
  data.pushBack({"second", l2});
  data.pushBack({"third", l3});
  data.pushBack({"fourth", l4});

  std::stringstream out;

  printTransposed(printNames(out, data), data);

  std::string expectedOutput = "first second third fourth\n"
                               "1 2 4\n"
                               "1 2 4\n"
                               "1 2\n"
                               "2\n"
                               "7 7 3 2\n";

  BOOST_CHECK_EQUAL(out.str(), expectedOutput);
}

BOOST_AUTO_TEST_CASE(empty_data_test) {
  Data data;
  std::stringstream out;

  printTransposed(out, data);
}
