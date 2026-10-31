#ifndef STORAGE_HPP
#define STORAGE_HPP

#include "Entities.hpp"
#include "Graph.hpp"
#include "RefCountIndex.hpp"
#include "Repo.hpp"
#include <cassert>
#include <cstddef>
#include <string>

namespace khasnulin
{
  class Storage
  {
  public:
    using count_type = std::size_t;

    Storage() = default;

    // false, если имя занято. Латентный вектор даёт модель.
    bool addUser(const UserName &user, const Latent &latent);
    // false, если имя занято. Латентный вектор даёт модель.
    bool addArticle(const ArticleName &article, const Category &category, const Difficulty &difficulty,
                    const Latent &latent);

    // false только если пользователя нет. Повторный тег — не ошибка, счётчик
    // тега при этом не растёт.
    bool addPreference(const UserName &user, const Tag &tag);
    // false, если статьи нет. Аналогично addPreference
    bool addArticleTag(const ArticleName &article, const Tag &tag);

    // false, если статьи или пользователя нет,
    // learn(Latent &user, Latent &article, const Rating &) noexcept для обучения.
    template < class Learn >
    bool rate(const UserName &user, const ArticleName &article, const Rating &rating, Learn learn);

    // false, если юзера нет. Каскадно удаляет пользователя (и чистит его ссылки на теги)
    bool eraseUser(const UserName &user);

    // false, если артикля нет. Каскадно удаляет статью (и чистит её ссылки на теги). Очистка категорий на
    // articleRepo
    bool eraseArticle(const ArticleName &article);

    void clear();

    void swap(Storage &rhs) noexcept;

    const User *findUser(const UserName &user) const;
    const Article *findArticle(const ArticleName &article) const;

    // для несуществующей сущности в обходах рейтингов и счётчиках (f не вызывается, счётчик 0)
    // F(const User&);
    template < class F > F traverseUsers(F f) const;
    // F(const Article&);
    template < class F > F traverseArticles(F f) const;
    // F(const Tag &, count_type)
    template < class F > F traverseTags(F f) const;
    // F(const Category&, count_type);
    template < class F > F traverseCategories(F f) const;
    // F(const Article&, const Rating&);
    template < class F > F traverseUserRatings(const UserName &user, F f) const;
    // F(const User&, const Rating&);
    template < class F > F traverseArticleRatings(const ArticleName &article, F f) const;

    count_type userRatingsCount(const UserName &user) const;

    count_type articleRatingsCount(const ArticleName &article) const;

    bool hasRating(const UserName &user, const ArticleName &article) const;

    bool hasCategory(const Category &category) const;

    count_type articlesCount() const noexcept;
    count_type usersCount() const noexcept;

  private:
    UserRepo users_;
    ArticleRepo articles_;
    Graph< std::string, std::string, Rating > ratings_;
    RefCountIndex< Tag > tagsIndex_;
  };

  // false, если статьи или пользователя нет,
  // learn(Latent &user, Latent &article, const Rating &) noexcept для обучения.
  template < class Learn >
  bool Storage::rate(const UserName &user, const ArticleName &article, const Rating &rating, Learn learn)
  {
    Article *art = articles_.find(article);
    User *us = users_.find(user);
    if (!art || !us)
    {
      return false;
    }
    ratings_.rate(us->name().value(), art->name().value(), rating);
    learn(us->latent(), art->latent(), rating);
    return true;
  }

  template < class F > F Storage::traverseUsers(F f) const
  {
    return users_.traverse(f);
  }

  // F(const Article&);
  template < class F > F Storage::traverseArticles(F f) const
  {
    return articles_.traverse(f);
  }

  // F(const Tag &, count_type)
  template < class F > F Storage::traverseTags(F f) const
  {
    return tagsIndex_.traverse(f);
  }

  // F(const Category&, count_type);
  template < class F > F Storage::traverseCategories(F f) const
  {
    return articles_.traverseCategories(f);
  }

  // F(const Article&, const Rating&);  f(const ArticleKey &article, const Weight &rating)
  template < class F > F Storage::traverseUserRatings(const UserName &user, F f) const
  {

    ratings_.traverseUserRatings(user.value(),
                                 [&](const std::string &key, const Rating &rating)
                                 {
                                   const Article *art = articles_.find(ArticleName(key));
                                   assert(art != nullptr);
                                   return f(*art, rating);
                                 });
    return f;
  }

  // F(const User&, const Rating&);
  template < class F > F Storage::traverseArticleRatings(const ArticleName &article, F f) const
  {
    ratings_.traverseArticleRatings(article.value(),
                                    [&](const std::string &key, const Rating &rating)
                                    {
                                      const User *us = users_.find(UserName(key));
                                      assert(us != nullptr);
                                      return f(*us, rating);
                                    });
    return f;
  }

}

#endif
