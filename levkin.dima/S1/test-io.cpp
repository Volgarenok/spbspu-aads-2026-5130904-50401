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

BOOST_AUTO_TEST_CASE(overflow_test) {
  Data data;
  Lst l1, l2;
  size_t max = std::numeric_limits<size_t>::max();

  l1.pushBack(max);
  l2.pushBack(1);

  data.pushBack({"list1", l1});
  data.pushBack({"list2", l2});

  std::stringstream out;
  BOOST_CHECK_THROW(printTransposed(out, data), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(completely_empty_input_test) {
  Data data;
  std::stringstream out;

  if (data.cbegin() == data.cend()) {
    out << "0\n";
  } else {
    printTransposed(printNames(out, data), data);
  }

  BOOST_CHECK_EQUAL(out.str(), "0\n");
}

BOOST_AUTO_TEST_CASE(list_with_no_numbers_test) {
  Data data;
  Lst emptyList;
  data.pushBack({"empty_list", emptyList});

  std::stringstream out;
  printNames(out, data);
  printTransposed(out, data);

  std::string expected = "empty_list\n0\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(mixed_empty_and_full_test) {
  Data data;
  Lst l1;
  l1.pushBack(10);
  Lst l2;
  Lst l3;
  l3.pushBack(20);

  data.pushBack({"first", l1});
  data.pushBack({"second", l2});
  data.pushBack({"third", l3});

  std::stringstream out;
  printTransposed(printNames(out, data), data);

  std::string expected = "first second third\n"
                         "10 20\n"
                         "30\n";

  BOOST_CHECK_EQUAL(out.str(), expected);
}
