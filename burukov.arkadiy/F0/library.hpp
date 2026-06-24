#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <iostream>
#include <string>

#include "AVLTree.hpp"
#include "graph.hpp"
#include <list.hpp>

namespace burukov
{
  struct Transaction
  {
    std::string copyId_;
    std::string reader_;
    int startDay_;
    int endDay_;
    bool isActive_;

    Transaction():
      startDay_(0),
      endDay_(0),
      isActive_(false)
    {}

    Transaction(const std::string& copyId, const std::string& reader, int startDay):
      copyId_(copyId),
      reader_(reader),
      startDay_(startDay),
      endDay_(0),
      isActive_(true)
    {}
  };

  struct BookData
  {
    std::string title_;
    std::string author_;
    int year_;
    std::string genre_;
    List< std::string > copies_;
    List< Transaction > history_;
    int lendCount_;
    int lastLendDate_;

    BookData():
      year_(0),
      lendCount_(0),
      lastLendDate_(0)
    {}

    BookData(const std::string& t, const std::string& a, int y, const std::string& g):
      title_(t),
      author_(a),
      year_(y),
      genre_(g),
      lendCount_(0),
      lastLendDate_(0)
    {}
  };

  class LibraryManager
  {
  public:
    LibraryManager();

    void addTitle(const std::string& title, const std::string& author, int year, const std::string& genre);
    void addCopy(const std::string& title, const std::string& copyId);
    void lend(const std::string& title, const std::string& reader);
    void returnCopy(const std::string& copyId);
    void demandModelTitle(std::ostream& out, const std::string& title, int period) const;
    void demandModelGenre(std::ostream& out, const std::string& genre, int period) const;
    void sliceTitle(std::ostream& out, const std::string& title, int period) const;
    void sliceGenre(std::ostream& out, const std::string& genre, int period) const;
    void recommend(std::ostream& out, const std::string& title, size_t k) const;
    void deadStock(std::ostream& out, int period, double threshold) const;
    void demandBalance(std::ostream& out, int period) const;

  private:
    struct LoadEvent
    {
      int day_;
      int delta_;
    };

    struct CoLendEvent
    {
      std::string book1_;
      std::string book2_;
      double weight_;
    };

    struct DemandStats
    {
      int total_;
      double p95_;
      double seasonCoef_;
      bool isSeasonal_;
      bool isStable_;

      DemandStats():
        total_(0),
        p95_(0.0),
        seasonCoef_(0.0),
        isSeasonal_(false),
        isStable_(false)
      {}
    };

    AVLTree< std::string, BookData > books_;
    mutable RecommendationGraph graph_;
    mutable bool graphDirty_;
    List< CoLendEvent > coLendLog_;
    int currentDay_;

    void rebuildGraph() const;
    DemandStats calculateStats(const BookData& book, int period) const;
    double calculateP95(const List< Transaction >& history, int currentDay, int period) const;
    double calculateSeasonality(const List< Transaction >& history, int currentDay, int period, double& maxCoef) const;
    std::string findTitleByCopy(const std::string& copyId) const;
    bool isCopyLent(const BookData& book, const std::string& copyId) const;
    void addCoLendRelations(const std::string& title, const std::string& reader);
    int countCopies(const BookData& book) const;
  };
}

#endif
