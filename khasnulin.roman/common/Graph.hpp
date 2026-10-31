#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "AutoHashTable.hpp"
#include "BSTree.hpp"

namespace khasnulin
{
  template < class UserKey, class ArticleKey, class Weight > class Graph
  {
  public:
    using size_type = std::size_t;

    Graph();

    void clear();
    void swap(Graph &rhs) noexcept;

    bool rate(const UserKey &user, const ArticleKey &article, const Weight &rating);

    bool hasRating(const UserKey &user, const ArticleKey &article) const;
    const Weight *findRating(const UserKey &user, const ArticleKey &article) const;

    size_type userRatingsCount(const UserKey &user) const;
    size_type articleRatingsCount(const ArticleKey &article) const;

    size_type ratingsCount() const noexcept;
    bool empty() const noexcept;

    bool eraseRating(const UserKey &user, const ArticleKey &article);
    size_type eraseUser(const UserKey &user);
    size_type eraseArticle(const ArticleKey &article);

    // Все оценки пользователя user.
    // Ожидает: void f(const ArticleKey &article, const Weight &rating)
    // Порядок: по возрастанию article (std::less< ArticleKey >).
    template < class F > F traverseUserRatings(const UserKey &user, F f) const;

    // Все оценки статьи article.
    // Ожидает: void f(const UserKey &user, const Weight &rating)
    // Порядок: по возрастанию user (std::less< UserKey >).
    template < class F > F traverseArticleRatings(const ArticleKey &article, F f) const;

    // Все оценки графа, каждая ровно один раз.
    // Ожидает: void f(const UserKey &user, const ArticleKey &article, const Weight &rating)
    // Порядок: пользователи -- в порядке хэш-таблицы (не определён),
    //          статьи внутри пользователя -- по возрастанию article.
    template < class F > F traverseAllRatings(F f) const;

  private:
    using UserTree = BSTree< ArticleKey, Weight >;
    using ArticleTree = BSTree< UserKey, Weight >;
    using UserIndex = AutoHashTable< UserKey, UserTree >;
    using ArticleIndex = AutoHashTable< ArticleKey, ArticleTree >;

    UserIndex byUser_;
    ArticleIndex byArticle_;
    size_type edges_;
  };

  template < class UserKey, class ArticleKey, class Weight >
  Graph< UserKey, ArticleKey, Weight >::Graph():
      byUser_(),
      byArticle_(),
      edges_(0)
  {
  }

  template < class UserKey, class ArticleKey, class Weight >
  void Graph< UserKey, ArticleKey, Weight >::clear()
  {
    Graph tmp;
    swap(tmp);
  }

  template < class UserKey, class ArticleKey, class Weight >
  void Graph< UserKey, ArticleKey, Weight >::swap(Graph &rhs) noexcept
  {
    std::swap(byUser_, rhs.byUser_);
    std::swap(byArticle_, rhs.byArticle_);
    std::swap(edges_, rhs.edges_);
  }

  template < class UserKey, class ArticleKey, class Weight >
  bool Graph< UserKey, ArticleKey, Weight >::rate(const UserKey &user, const ArticleKey &article,
                                                  const Weight &rating)
  {

    UserTree *userTree = byUser_.find(user);
    ArticleTree *articleTree = byArticle_.find(article);
    if (userTree && articleTree)
    {
      auto userIt = userTree->find(article);
      auto articleIt = articleTree->find(user);
      if (userIt != userTree->end() && articleIt != articleTree->end())
      {
        userIt->second = rating;
        articleIt->second = rating;
        return false;
      }
    }

    const bool newUser = (userTree == nullptr);
    const bool newArticle = (articleTree == nullptr);

    if (newUser)
    {
      UserTree tmp;
      tmp.insert({article, rating});
      byUser_.add(user, std::move(tmp));
    }
    else
    {
      userTree->insert({article, rating});
    }
    try
    {
      if (newArticle)
      {
        ArticleTree tmp;
        tmp.insert({user, rating});
        byArticle_.add(article, std::move(tmp));
      }
      else
      {
        articleTree->insert({user, rating});
      }
    }
    catch (...)
    {
      if (newUser)
      {
        byUser_.erase(user);
      }
      else
      {
        userTree->erase(article);
      }
      throw;
    }

    ++edges_;
    return true;
  }

  template < class UserKey, class ArticleKey, class Weight >
  bool Graph< UserKey, ArticleKey, Weight >::hasRating(const UserKey &user, const ArticleKey &article) const
  {
    return findRating(user, article) != nullptr;
  }

  template < class UserKey, class ArticleKey, class Weight >
  const Weight *Graph< UserKey, ArticleKey, Weight >::findRating(const UserKey &user,
                                                                 const ArticleKey &article) const
  {
    const UserTree *userTree = byUser_.find(user);
    if (userTree)
    {
      auto userIt = userTree->find(article);
      if (userIt != userTree->end())
      {
        return &userIt->second;
      }
    }
    return nullptr;
  }

  template < class UserKey, class ArticleKey, class Weight >
  typename Graph< UserKey, ArticleKey, Weight >::size_type
  Graph< UserKey, ArticleKey, Weight >::userRatingsCount(const UserKey &user) const
  {
    const UserTree *userTree = byUser_.find(user);
    if (userTree)
    {
      return userTree->size();
    }
    return 0;
  }

  template < class UserKey, class ArticleKey, class Weight >
  typename Graph< UserKey, ArticleKey, Weight >::size_type
  Graph< UserKey, ArticleKey, Weight >::articleRatingsCount(const ArticleKey &article) const
  {
    const ArticleTree *articleTree = byArticle_.find(article);
    if (articleTree)
    {
      return articleTree->size();
    }
    return 0;
  }

  template < class UserKey, class ArticleKey, class Weight >
  typename Graph< UserKey, ArticleKey, Weight >::size_type
  Graph< UserKey, ArticleKey, Weight >::ratingsCount() const noexcept
  {
    return edges_;
  }

  template < class UserKey, class ArticleKey, class Weight >
  bool Graph< UserKey, ArticleKey, Weight >::empty() const noexcept
  {
    return edges_ == 0;
  }

  template < class UserKey, class ArticleKey, class Weight >
  bool Graph< UserKey, ArticleKey, Weight >::eraseRating(const UserKey &user, const ArticleKey &article)
  {
    UserTree *userTree = byUser_.find(user);
    ArticleTree *articleTree = byArticle_.find(article);
    if (userTree && articleTree)
    {
      if (userTree->erase(article) == 0)
      {
        return false;
      }
      articleTree->erase(user);
      if (userTree->empty())
      {
        byUser_.erase(user);
      }
      if (articleTree->empty())
      {
        byArticle_.erase(article);
      }
      --edges_;
      return true;
    }
    return false;
  }

  template < class UserKey, class ArticleKey, class Weight >
  typename Graph< UserKey, ArticleKey, Weight >::size_type
  Graph< UserKey, ArticleKey, Weight >::eraseUser(const UserKey &user)
  {
    UserTree *userTree = byUser_.find(user);
    if (!userTree)
    {
      return 0;
    }
    const size_type removeCount = userTree->size();
    userTree->traverseLNR(
        [&](const typename UserTree::value_type &val) -> void
        {
          ArticleTree *articleTree = byArticle_.find(val.first);
          if (articleTree)
          {
            articleTree->erase(user);
            if (articleTree->empty())
            {
              byArticle_.erase(val.first);
            }
          }
        });
    byUser_.erase(user);
    edges_ -= removeCount;
    return removeCount;
  }

  template < class UserKey, class ArticleKey, class Weight >
  typename Graph< UserKey, ArticleKey, Weight >::size_type
  Graph< UserKey, ArticleKey, Weight >::eraseArticle(const ArticleKey &article)
  {
    ArticleTree *articleTree = byArticle_.find(article);
    if (!articleTree)
    {
      return 0;
    }
    const size_type removeCount = articleTree->size();
    articleTree->traverseLNR(
        [&](const typename ArticleTree::value_type &val) -> void
        {
          UserTree *userTree = byUser_.find(val.first);
          if (userTree)
          {
            userTree->erase(article);
            if (userTree->empty())
            {
              byUser_.erase(val.first);
            }
          }
        });
    byArticle_.erase(article);
    edges_ -= removeCount;
    return removeCount;
  }

  template < class UserKey, class ArticleKey, class Weight >
  template < class F >
  F Graph< UserKey, ArticleKey, Weight >::traverseUserRatings(const UserKey &user, F f) const
  {
    const UserTree *userTree = byUser_.find(user);
    if (userTree)
    {
      userTree->traverseLNR([&](const typename UserTree::value_type &val) -> void
                            { f(val.first, val.second); });
    }
    return f;
  }

  template < class UserKey, class ArticleKey, class Weight >
  template < class F >
  F Graph< UserKey, ArticleKey, Weight >::traverseArticleRatings(const ArticleKey &article, F f) const
  {
    const ArticleTree *articleTree = byArticle_.find(article);
    if (articleTree)
    {
      articleTree->traverseLNR([&](const typename ArticleTree::value_type &val) -> void
                               { f(val.first, val.second); });
    }
    return f;
  }

  // Обход по всем
  template < class UserKey, class ArticleKey, class Weight >
  template < class F >
  F Graph< UserKey, ArticleKey, Weight >::traverseAllRatings(F f) const
  {
    for (const typename UserIndex::public_value_type &pair : byUser_)
    {
      pair.second.traverseLNR([&](const typename UserTree::value_type &val) -> void
                              { f(pair.first, val.first, val.second); });
    }
    return f;
  }
}

#endif
