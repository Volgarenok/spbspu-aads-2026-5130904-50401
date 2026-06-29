#if defined(__GNUC__) && __GNUC__ >= 13
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdangling-reference"
#endif

#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE RBTree_suite
#include <boost/test/included/unit_test.hpp>

#if defined(__GNUC__) && __GNUC__ >= 13
#pragma GCC diagnostic pop
#endif
