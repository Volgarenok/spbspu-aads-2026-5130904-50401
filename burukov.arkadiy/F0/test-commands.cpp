#include <sstream>
#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "commands.hpp"
#include "library.hpp"

BOOST_AUTO_TEST_SUITE(CommandParsingTests)

BOOST_AUTO_TEST_CASE(readTokenHandlesQuotedString)
{
  std::istringstream in("\"Physics 10\"");
  BOOST_CHECK_EQUAL(burukov::readToken(in), "Physics 10");
}

BOOST_AUTO_TEST_CASE(readTokenHandlesUnquotedString)
{
  std::istringstream in("simple");
  BOOST_CHECK_EQUAL(burukov::readToken(in), "simple");
}

BOOST_AUTO_TEST_CASE(readTokenThrowsOnUnclosedQuote)
{
  std::istringstream in("\"unclosed");
  BOOST_CHECK_THROW(burukov::readToken(in), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(parsingAddTitleOutputsAdded)
{
  burukov::LibraryManager lib;
  std::istringstream in("1984 Orwell 1949 dystopia");
  std::ostringstream out;
  burukov::parsingAddTitle(in, out, lib);
  BOOST_CHECK_EQUAL(out.str(), "<ADDED>\n");
}

BOOST_AUTO_TEST_CASE(parsingAddTitleDuplicateThrows)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  std::istringstream in("1984 Orwell 1949 dystopia");
  std::ostringstream out;
  BOOST_CHECK_THROW(
    burukov::parsingAddTitle(in, out, lib),
    std::runtime_error
  );
}

BOOST_AUTO_TEST_CASE(parsingAddCopyOutputsOk)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  std::istringstream in("1984 C1");
  std::ostringstream out;
  burukov::parsingAddCopy(in, out, lib);
  BOOST_CHECK_EQUAL(out.str(), "<OK>\n");
}

BOOST_AUTO_TEST_CASE(parsingLendOutputsIssued)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addCopy("1984", "C1");
  std::istringstream in("1984 Ivanov");
  std::ostringstream out;
  burukov::parsingLend(in, out, lib);
  BOOST_CHECK_EQUAL(out.str(), "<ISSUED>\n");
}

BOOST_AUTO_TEST_CASE(parsingReturnOutputsReturned)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addCopy("1984", "C1");
  lib.lend("1984", "Ivanov");
  std::istringstream in("C1");
  std::ostringstream out;
  burukov::parsingReturn(in, out, lib);
  BOOST_CHECK_EQUAL(out.str(), "<RETURNED>\n");
}

BOOST_AUTO_TEST_CASE(parsingDemandTitleWithValidInput)
{
  burukov::LibraryManager lib;
  lib.addTitle("Physics", "Author", 2020, "education");
  lib.addCopy("Physics", "C1");
  lib.lend("Physics", "Ivanov");
  lib.returnCopy("C1");
  std::istringstream in("Physics 365");
  std::ostringstream out;
  burukov::parsingDemandTitle(in, out, lib);
  std::string result = out.str();
  BOOST_CHECK(result.find("Total:") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(parsingSliceTitleWithValidInput)
{
  burukov::LibraryManager lib;
  lib.addTitle("Physics", "Author", 2020, "education");
  lib.addCopy("Physics", "C1");
  for (int i = 0; i < 10; ++i)
  {
    lib.lend("Physics", "Student");
    lib.returnCopy("C1");
  }
  std::istringstream in("Physics 365");
  std::ostringstream out;
  burukov::parsingSliceTitle(in, out, lib);
  std::string result = out.str();
  BOOST_CHECK(result.find("Seasonality:") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(parsingRecommendWithValidInput)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addTitle("Brave New World", "Huxley", 1932, "dystopia");
  lib.addCopy("1984", "C1");
  lib.addCopy("Brave New World", "C2");
  lib.lend("1984", "Ivanov");
  lib.lend("Brave New World", "Ivanov");
  lib.returnCopy("C1");
  lib.returnCopy("C2");
  std::istringstream in("1984 5");
  std::ostringstream out;
  burukov::parsingRecommend(in, out, lib);
  BOOST_CHECK(out.str().find("Brave New World") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(parsingDeadStockWithValidInput)
{
  burukov::LibraryManager lib;
  lib.addTitle("Old", "Author", 1990, "fiction");
  lib.addCopy("Old", "C1");
  std::istringstream in("365 0.5");
  std::ostringstream out;
  burukov::parsingDeadStock(in, out, lib);
  BOOST_CHECK(out.str().find("Old") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(parsingDemandBalanceWithValidInput)
{
  burukov::LibraryManager lib;
  lib.addTitle("Physics", "Author", 2020, "education");
  lib.addCopy("Physics", "C1");
  for (int i = 0; i < 20; ++i)
  {
    lib.lend("Physics", "Student");
    lib.returnCopy("C1");
  }
  std::istringstream in("365");
  std::ostringstream out;
  burukov::parsingDemandBalance(in, out, lib);
  std::string result = out.str();
  BOOST_CHECK(result.find("Buy:") != std::string::npos);
  BOOST_CHECK(result.find("Remove:") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
