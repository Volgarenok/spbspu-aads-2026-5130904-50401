#include <sstream>
#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "library.hpp"

BOOST_AUTO_TEST_SUITE(LibraryBasicTests)

BOOST_AUTO_TEST_CASE(addTitleInsertsBook)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(addTitleDuplicateThrows)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  BOOST_CHECK_THROW(
    lib.addTitle("1984", "Orwell", 1949, "dystopia"),
    std::runtime_error
  );
}

BOOST_AUTO_TEST_CASE(addCopyDuplicateThrows)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addCopy("1984", "C1");
  BOOST_CHECK_THROW(lib.addCopy("1984", "C1"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(lendIssuesAvailableCopy)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addCopy("1984", "C1");
  lib.lend("1984", "Ivanov");
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(lendNoCopiesThrows)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addCopy("1984", "C1");
  lib.lend("1984", "Ivanov");
  BOOST_CHECK_THROW(lib.lend("1984", "Petrov"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(returnCopyFreesCopy)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addCopy("1984", "C1");
  lib.lend("1984", "Ivanov");
  lib.returnCopy("C1");
  lib.lend("1984", "Petrov");
  BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(returnNotLentCopyThrows)
{
  burukov::LibraryManager lib;
  lib.addTitle("1984", "Orwell", 1949, "dystopia");
  lib.addCopy("1984", "C1");
  BOOST_CHECK_THROW(lib.returnCopy("C1"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(LibraryAlgorithmTests)

BOOST_AUTO_TEST_CASE(demandModelTitleNoDataThrows)
{
  burukov::LibraryManager lib;
  lib.addTitle("Physics", "Author", 2020, "education");
  std::ostringstream out;
  BOOST_CHECK_THROW(
    lib.demandModelTitle(out, "Physics", 365),
    std::runtime_error
  );
}

BOOST_AUTO_TEST_CASE(demandModelTitleWithData)
{
  burukov::LibraryManager lib;
  lib.addTitle("Physics", "Author", 2020, "education");
  lib.addCopy("Physics", "C1");
  for (int i = 0; i < 15; ++i)
  {
    lib.lend("Physics", "Student");
    lib.returnCopy("C1");
  }
  std::ostringstream out;
  lib.demandModelTitle(out, "Physics", 365);
  std::string result = out.str();
  BOOST_CHECK(result.find("Total: 15") != std::string::npos);
  BOOST_CHECK(result.find("Minimal:") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(sliceTitleShowsStats)
{
  burukov::LibraryManager lib;
  lib.addTitle("Physics", "Author", 2020, "education");
  lib.addCopy("Physics", "C1");
  for (int i = 0; i < 10; ++i)
  {
    lib.lend("Physics", "R" + std::to_string(i));
    lib.returnCopy("C1");
  }
  std::ostringstream out;
  lib.sliceTitle(out, "Physics", 365);
  std::string result = out.str();
  BOOST_CHECK(result.find("Seasonality:") != std::string::npos);
  BOOST_CHECK(result.find("Stability:") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(sliceGenreShowsStats)
{
  burukov::LibraryManager lib;
  lib.addTitle("A", "Author1", 2020, "fiction");
  lib.addTitle("B", "Author2", 2020, "fiction");
  lib.addCopy("A", "C1");
  lib.addCopy("B", "C2");
  lib.lend("A", "Ivanov");
  lib.returnCopy("C1");
  std::ostringstream out;
  lib.sliceGenre(out, "fiction", 365);
  std::string result = out.str();
  BOOST_CHECK(result.find("Top books:") != std::string::npos);
  BOOST_CHECK(result.find("Diversity:") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(recommendReturnsRelatedBooks)
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
  std::ostringstream out;
  lib.recommend(out, "1984", 5);
  BOOST_CHECK(out.str().find("Brave New World") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(deadStockReturnsNoneForEmptyLibrary)
{
  burukov::LibraryManager lib;
  std::ostringstream out;
  lib.deadStock(out, 365, 1.0);
  BOOST_CHECK_EQUAL(out.str(), "<NONE>\n");
}

BOOST_AUTO_TEST_CASE(deadStockFindsUnusedBook)
{
  burukov::LibraryManager lib;
  lib.addTitle("Old Book", "Author", 1990, "fiction");
  lib.addCopy("Old Book", "C1");
  std::ostringstream out;
  lib.deadStock(out, 365, 0.5);
  BOOST_CHECK(out.str().find("Old Book") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(demandBalanceEmptyLibrary)
{
  burukov::LibraryManager lib;
  std::ostringstream out;
  lib.demandBalance(out, 365);
  BOOST_CHECK(out.str().find("<NONE>") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(demandBalanceWithData)
{
  burukov::LibraryManager lib;
  lib.addTitle("Physics", "Author", 2020, "education");
  lib.addCopy("Physics", "C1");
  lib.addCopy("Physics", "C2");
  for (int i = 0; i < 20; ++i)
  {
    lib.lend("Physics", "Student");
    lib.returnCopy("C1");
  }
  std::ostringstream out;
  lib.demandBalance(out, 365);
  std::string result = out.str();
  BOOST_CHECK(result.find("Buy:") != std::string::npos);
  BOOST_CHECK(result.find("Remove:") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
