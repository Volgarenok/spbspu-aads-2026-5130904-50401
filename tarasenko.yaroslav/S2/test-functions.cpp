#include "boost/test/unit_test.hpp"
#include "functions.hpp"

using namespace tarasenko;

BOOST_AUTO_TEST_CASE(get_number)
{
  std::string s = "12345";
  size_t pos = 0;
  BOOST_TEST(getNumber(s, pos) == 12345);
  BOOST_TEST(pos = 5);
  s = "12345a";
  pos = 1;
  BOOST_TEST(getNumber(s, pos) == 2345);
}

BOOST_AUTO_TEST_CASE(string_to_queue)
{
  std::string s = "( 1 + 2 ) * ( 3 - 4 )";
  Queue< char > queue = stringToQueue(s);
  for (size_t i = 0; i < queue.size(); ++i)
  {
    std::cout << queue.pop() << '\n';
  }
}
