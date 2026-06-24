#include "library.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
  constexpr double kSeasonalVariationThreshold = 1.0;
  constexpr double kStableVariationThreshold = 0.5;
  constexpr int kMinDataPoints = 5;
  constexpr int kRecentDays = 30;
  constexpr int kMonthsInYear = 12;
  constexpr int kDaysInMonth = 30;
  constexpr double kMargin = 1.1;
  constexpr double kTransitiveWeight = 0.5;
  constexpr int kTopBooksForSlice = 3;
  constexpr int kTopPercentThreshold = 70;
  constexpr double kP95Percentile = 0.95;
  constexpr int kGrowthTrendThreshold = 10;
  constexpr double kTimeDecayFactor = 0.1;
  constexpr int kPercent = 100;
  constexpr int kHalfPeriodDivisor = 2;
}

burukov::LibraryManager::LibraryManager():
  graphDirty_(false),
  currentDay_(1)
{}

void burukov::LibraryManager::rebuildGraph() const
{
  graph_ = RecommendationGraph();
  for (auto it = coLendLog_.cbegin(); it != coLendLog_.cend(); ++it)
  {
    graph_.addCoLend(it->book1_, it->book2_, it->weight_);
  }
  graph_.calculateScores(kTransitiveWeight);
  graphDirty_ = false;
}

std::string burukov::LibraryManager::findTitleByCopy(const std::string& copyId) const
{
  std::string result;
  books_.traverseInOrder(
    [&](const std::string& title, const BookData& book)
    {
      if (!result.empty())
      {
        return;
      }
      for (auto it = book.copies_.cbegin(); it != book.copies_.cend(); ++it)
      {
        if (*it == copyId)
        {
          result = title;
          break;
        }
      }
    }
  );
  if (result.empty())
  {
    throw std::runtime_error("copy not found");
  }
  return result;
}

bool burukov::LibraryManager::isCopyLent(const BookData& book, const std::string& copyId) const
{
  for (auto it = book.history_.begin(); it != book.history_.end(); ++it)
  {
    if (it->copyId_ == copyId && it->isActive_)
    {
      return true;
    }
  }
  return false;
}

int burukov::LibraryManager::countCopies(const BookData& book) const
{
  return static_cast< int >(book.copies_.size());
}

void burukov::LibraryManager::addCoLendRelations(const std::string& title, const std::string& reader)
{
  books_.traverseInOrder(
    [&](const std::string& otherTitle, const BookData& otherBook)
    {
      if (otherTitle == title)
      {
        return;
      }
      for (auto it = otherBook.history_.cbegin(); it != otherBook.history_.cend(); ++it)
      {
        if (it->reader_ == reader && it->startDay_ >= currentDay_ - kRecentDays)
        {
          double timeDecay = 1.0;
          int daysDiff = currentDay_ - it->startDay_;
          if (daysDiff > 0)
          {
            timeDecay = 1.0 / (1.0 + static_cast< double >(daysDiff) * kTimeDecayFactor);
          }
          coLendLog_.pushBack(CoLendEvent{title, otherTitle, timeDecay});
          graphDirty_ = true;
          break;
        }
      }
    }
  );
}

void burukov::LibraryManager::addTitle(const std::string& title, const std::string& author, int year,
  const std::string& genre)
{
  if (books_.hasKey(title))
  {
    throw std::runtime_error("exists");
  }
  books_.push(title, BookData(title, author, year, genre));
}

void burukov::LibraryManager::addCopy(const std::string& title, const std::string& copyId)
{
  if (!books_.hasKey(title))
  {
    throw std::runtime_error("no title");
  }
  bool copyExists = false;
  books_.traverseInOrder(
    [&](const std::string&, const BookData& book)
    {
      for (auto it = book.copies_.cbegin(); it != book.copies_.cend(); ++it)
      {
        if (*it == copyId)
        {
          copyExists = true;
          break;
        }
      }
    }
  );
  if (copyExists)
  {
    throw std::runtime_error("copy exists");
  }
  BookData& book = books_.at(title);
  book.copies_.pushBack(copyId);
}

void burukov::LibraryManager::lend(const std::string& title, const std::string& reader)
{
  if (!books_.hasKey(title))
  {
    throw std::runtime_error("no title");
  }
  BookData& book = books_.at(title);
  std::string availableCopy;
  bool found = false;
  for (auto it = book.copies_.begin(); it != book.copies_.end(); ++it)
  {
    if (!isCopyLent(book, *it))
    {
      availableCopy = *it;
      found = true;
      break;
    }
  }
  if (!found)
  {
    throw std::runtime_error("no copies");
  }
  Transaction record(availableCopy, reader, currentDay_);
  book.history_.pushBack(record);
  book.lendCount_++;
  book.lastLendDate_ = currentDay_;
  addCoLendRelations(title, reader);
  currentDay_++;
}

void burukov::LibraryManager::returnCopy(const std::string& copyId)
{
  const std::string title = findTitleByCopy(copyId);
  BookData& book = books_.at(title);
  bool found = false;
  for (auto it = book.history_.begin(); it != book.history_.end(); ++it)
  {
    if (it->copyId_ == copyId && it->isActive_)
    {
      it->endDay_ = currentDay_;
      it->isActive_ = false;
      found = true;
      break;
    }
  }
  if (!found)
  {
    throw std::runtime_error("not lent");
  }
  currentDay_++;
}

double burukov::LibraryManager::calculateP95(
  const List< Transaction >& history,
  int currentDay,
  int period) const
{
  List< LoadEvent > events;
  const int startDay = currentDay - period;
  for (auto it = history.cbegin(); it != history.cend(); ++it)
  {
    if (it->isActive_)
    {
      if (it->startDay_ >= startDay)
      {
        events.pushBack(LoadEvent{it->startDay_, 1});
        events.pushBack(LoadEvent{currentDay + 1, -1});
      }
    }
    else
    {
      if (it->startDay_ >= startDay || it->endDay_ >= startDay)
      {
        int s = std::max(it->startDay_, startDay);
        int e = std::min(it->endDay_, currentDay);
        if (s <= e)
        {
          events.pushBack(LoadEvent{s, 1});
          events.pushBack(LoadEvent{e + 1, -1});
        }
      }
    }
  }
  if (events.empty())
  {
    return 0.0;
  }
  events.sort(
    [](const LoadEvent& a, const LoadEvent& b)
    {
      if (a.day_ != b.day_)
      {
        return a.day_ < b.day_;
      }
      return a.delta_ < b.delta_;
    }
  );
  List< int > loads;
  int curLoad = 0;
  int lastDay = startDay;
  for (auto it = events.begin(); it != events.end(); ++it)
  {
    while (lastDay < it->day_ && lastDay <= currentDay)
    {
      loads.pushBack(curLoad);
      ++lastDay;
    }
    curLoad += it->delta_;
    lastDay = it->day_;
  }
  while (lastDay <= currentDay)
  {
    loads.pushBack(curLoad);
    ++lastDay;
  }
  if (loads.empty())
  {
    return 0.0;
  }
  loads.sort();
  size_t idx = static_cast< size_t >(static_cast< double >(loads.size()) * kP95Percentile);
  if (idx >= loads.size())
  {
    idx = loads.size() - 1;
  }
  auto loadIt = loads.begin();
  for (size_t i = 0; i < idx; ++i)
  {
    ++loadIt;
  }
  return static_cast< double >(*loadIt);
}

double burukov::LibraryManager::calculateSeasonality(
  const List< Transaction >& history,
  int currentDay,
  int period,
  double& maxCoef) const
{
  int monthlyCounts[kMonthsInYear] = {0};
  const int startDay = currentDay - period;
  for (auto it = history.cbegin(); it != history.cend(); ++it)
  {
    int activeStart = std::max(it->startDay_, startDay);
    int activeEnd = (it->isActive_) ? currentDay : std::min(it->endDay_, currentDay);
    if (activeEnd < activeStart)
    {
      continue;
    }
    for (int d = activeStart; d <= activeEnd; ++d)
    {
      int monthOfYear = ((d - 1) / kDaysInMonth) % kMonthsInYear;
      monthlyCounts[monthOfYear]++;
    }
  }
  double sum = 0.0;
  for (int i = 0; i < kMonthsInYear; ++i)
  {
    sum += static_cast< double >(monthlyCounts[i]);
  }
  double mean = sum / static_cast< double >(kMonthsInYear);
  if (mean < 1.0)
  {
    maxCoef = 1.0;
    return 0.0;
  }
  double variance = 0.0;
  maxCoef = 0.0;
  for (int i = 0; i < kMonthsInYear; ++i)
  {
    double diff = static_cast< double >(monthlyCounts[i]) - mean;
    variance += diff * diff;
    double coef = static_cast< double >(monthlyCounts[i]) / mean;
    if (coef > maxCoef)
    {
      maxCoef = coef;
    }
  }
  variance /= static_cast< double >(kMonthsInYear);
  double stddev = std::sqrt(variance);
  return stddev / mean;
}

burukov::LibraryManager::DemandStats
burukov::LibraryManager::calculateStats(const BookData& book, int period) const
{
  DemandStats stats;
  const int startDay = currentDay_ - period;
  for (auto it = book.history_.cbegin(); it != book.history_.cend(); ++it)
  {
    if (it->startDay_ >= startDay || it->endDay_ >= startDay || it->endDay_ == 0)
    {
      ++stats.total_;
    }
  }
  stats.p95_ = calculateP95(book.history_, currentDay_, period);
  stats.seasonCoef_ = 1.0;
  double variation = calculateSeasonality(book.history_, currentDay_, period, stats.seasonCoef_);
  if (stats.total_ < kMinDataPoints)
  {
    stats.isSeasonal_ = false;
  }
  else
  {
    stats.isSeasonal_ = (variation > kSeasonalVariationThreshold);
  }
  stats.isStable_ = (variation < kStableVariationThreshold);
  return stats;
}

void burukov::LibraryManager::demandModelTitle(std::ostream& out, const std::string& title, int period) const
{
  if (!books_.hasKey(title))
  {
    throw std::runtime_error("no title");
  }
  const BookData& book = books_.at(title);
  const DemandStats stats = calculateStats(book, period);
  if (stats.total_ == 0)
  {
    throw std::runtime_error("no data");
  }
  int minimal = static_cast< int >(std::ceil(stats.p95_ * stats.seasonCoef_ * kMargin));
  if (minimal < 1)
  {
    minimal = 1;
  }
  int currentCopies = countCopies(book);
  int need = minimal - currentCopies;
  out << "Total: " << stats.total_ << "\n";
  out << "Trend: " << (stats.total_ > kGrowthTrendThreshold ? "GROWING" : "STABLE") << "\n";
  out << "Seasonality: " << (stats.isSeasonal_ ? "HIGH" : "LOW") << "\n";
  out << "Minimal: " << minimal << "\n";
  if (need > 0)
  {
    out << "Need: +" << need << "\n";
  }
  else
  {
    out << "Need: 0\n";
  }
}

void burukov::LibraryManager::demandModelGenre(std::ostream& out, const std::string& genre, int period) const
{
  int total = 0;
  double maxPeakLoad = 0.0;
  books_.traverseInOrder(
    [&](const std::string&, const BookData& book)
    {
      if (book.genre_ == genre)
      {
        const DemandStats stats = calculateStats(book, period);
        total += stats.total_;
        const double adjusted = stats.p95_ * stats.seasonCoef_;
        if (adjusted > maxPeakLoad)
        {
          maxPeakLoad = adjusted;
        }
      }
    }
  );
  if (total == 0)
  {
    throw std::runtime_error("no data");
  }
  int minimal = static_cast< int >(std::ceil(maxPeakLoad * kMargin));
  if (minimal < 1)
  {
    minimal = 1;
  }
  out << "Total: " << total << "\n";
  out << "Trend: GROWING\n";
  out << "Minimal: " << minimal << "\n";
}

void burukov::LibraryManager::sliceTitle(std::ostream& out, const std::string& title, int period) const
{
  if (!books_.hasKey(title))
  {
    throw std::runtime_error("no title");
  }
  const BookData& book = books_.at(title);
  const DemandStats stats = calculateStats(book, period);
  if (stats.total_ == 0)
  {
    throw std::runtime_error("no data");
  }
  out << "Seasonality: " << (stats.isSeasonal_ ? "HIGH" : "LOW") << "\n";
  out << "Stability: " << (stats.isStable_ ? "HIGH" : "LOW") << "\n";
  out << "Peak load: " << static_cast< int >(std::ceil(stats.p95_)) << "\n";
}

void burukov::LibraryManager::sliceGenre(
  std::ostream& out,
  const std::string& genre,
  int period) const
{
  int total = 0;
  List< std::pair< std::string, int > > genreBooks;
  books_.traverseInOrder(
    [&](const std::string&, const BookData& book)
    {
      if (book.genre_ == genre)
      {
        const DemandStats stats = calculateStats(book, period);
        if (stats.total_ > 0)
        {
          genreBooks.pushBack(std::make_pair(book.title_, stats.total_));
          total += stats.total_;
        }
      }
    }
  );
  if (total == 0)
  {
    throw std::runtime_error("no data");
  }
  genreBooks.sort([](const std::pair< std::string, int >& a, const std::pair< std::string, int >& b)
    {
      return a.second > b.second;
    }
  );
  int topLends = 0;
  int cnt = 0;
  for (auto it = genreBooks.cbegin(); it != genreBooks.cend() && cnt < kTopBooksForSlice; ++it, ++cnt)
  {
    topLends += it->second;
  }
  int topPercent = (topLends * kPercent) / total;
  out << "Top books: " << topPercent << "%\n";
  out << "Diversity: " << (topPercent > kTopPercentThreshold ? "LOW" : "HIGH") << "\n";
}

void burukov::LibraryManager::recommend(std::ostream& out, const std::string& title, size_t k) const
{
  if (!books_.hasKey(title))
  {
    throw std::runtime_error("no title");
  }
  if (graphDirty_)
  {
    rebuildGraph();
  }
  auto recs = graph_.getRecommendations(title, k);
  if (recs.empty())
  {
    throw std::runtime_error("no recommendations");
  }
  int rank = 1;
  for (auto it = recs.cbegin(); it != recs.cend(); ++it, ++rank)
  {
    out << rank << ". " << it->first << "\n";
  }
}

void burukov::LibraryManager::deadStock(std::ostream& out, int period, double threshold) const
{
  bool found = false;
  books_.traverseInOrder(
    [&](const std::string&, const BookData& book)
    {
      const DemandStats stats = calculateStats(book, period);
      bool notRecentlyLent = (stats.total_ == 0 || currentDay_ - book.lastLendDate_ > period / kHalfPeriodDivisor);
      if (static_cast< double >(stats.total_) < threshold && !stats.isSeasonal_ && notRecentlyLent)
      {
        out << book.title_ << "\n";
        found = true;
      }
    }
  );
  if (!found)
  {
    out << "<NONE>\n";
  }
}

void burukov::LibraryManager::demandBalance(std::ostream& out, int period) const
{
  List< std::string > buyList;
  List< std::string > removeList;
  books_.traverseInOrder(
    [&](const std::string&, const BookData& book)
    {
      const DemandStats stats = calculateStats(book, period);
      int minimal = static_cast< int >(std::ceil(stats.p95_ * stats.seasonCoef_ * kMargin));
      if (minimal < 1 && stats.total_ > 0)
      {
        minimal = 1;
      }
      int current = countCopies(book);
      if (minimal > current)
      {
        for (int i = 0; i < minimal - current; ++i)
        {
          buyList.pushBack(book.title_ + " +1");
        }
      }
      else if (current > minimal && stats.total_ == 0)
      {
        for (int i = 0; i < current - minimal; ++i)
        {
          removeList.pushBack(book.title_ + " -1");
        }
      }
    }
  );
  out << "Buy:\n";
  if (buyList.empty())
  {
    out << "<NONE>\n";
  }
  else
  {
    for (auto it = buyList.cbegin(); it != buyList.cend(); ++it)
    {
      out << *it << "\n";
    }
  }
  out << "Remove:\n";
  if (removeList.empty())
  {
    out << "<NONE>\n";
  }
  else
  {
    for (auto it = removeList.cbegin(); it != removeList.cend(); ++it)
    {
      out << *it << "\n";
    }
  }
}
