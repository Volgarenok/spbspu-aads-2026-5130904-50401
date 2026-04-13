#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(Petrov_List_Test_Suite)

BOOST_AUTO_TEST_CASE(test_init)
{
	Petrov::List<size_t> empt_l;
	BOOST_CHECK(empt_l.IsEmpty());
	BOOST_CHECK_EQUAL(empt_l.getSize(), 0);
	
	Petrov::List<size_t> unempt_l(2, 5);
	BOOST_CHECK_EQUAL(*unempt_l.begin(), 100);
}

BOOST_AUTO_TEST_SUITE_END()