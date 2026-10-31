#ifndef REPO_HPP
#define REPO_HPP

#include "AutoHashTable.hpp"
#include "BSTree.hpp"
#include "Entities.hpp"
#include "RefCountIndex.hpp"
#include <string>

namespace khasnulin
{
  // Внутренний компонент фасада. Инварианты: ключи users_ == nameIndex_, ключ == name().value().
  // add: строгая гарантия (bad_alloc/overflow_error). erase не бросает.
  // Указатель из find действителен до следующего add.
  class UserRepo
  {
  public:
    UserRepo() = default;

    using size_type = typename BSTree< UserName, bool >::size_type;

    // Создаёт пользователя без предпочтений; false, если имя занято.
    bool add(const UserName &name, const Latent &latent);
    // удаляет из обеих структур; false, если такого нет.
    bool erase(const UserName &name);

    User *find(const UserName &name);
    const User *find(const UserName &name) const;

    size_type size() const noexcept;
    bool empty() const noexcept;

    void clear();

    void swap(UserRepo &rhs) noexcept;

    // f(const User &) по возрастанию имени (обход nameIndex_, поиск каждого в users_);
    // f не должен менять репозиторий.
    template < class F > F traverse(F f) const;

  private:
    AutoHashTable< std::string, User > users_;
    BSTree< UserName, bool > nameIndex_;
  };

  // Внутренний компонент фасада. Инварианты: ключи artilces_ == nameIndex_, ключ == name().value().
  // add: строгая гарантия (bad_alloc/overflow_error). erase не бросает. erase снимает ссылку с категории.
  // Указатель из find действителен до следующего add.
  class ArticleRepo
  {
  public:
    ArticleRepo() = default;

    using size_type = typename BSTree< ArticleName, bool >::size_type;

    using count_type = typename RefCountIndex< Category >::count_type;

    // Создаёт статью без тегов; false, если название занято.
    bool add(const ArticleName &name, const Category &category, const Difficulty &difficulty,
             const Latent &latent);

    // удаляет из всех структур; false, если такого нет.
    bool erase(const ArticleName &name);

    Article *find(const ArticleName &name);
    const Article *find(const ArticleName &name) const;

    size_type size() const noexcept;
    bool empty() const noexcept;

    void clear();

    void swap(ArticleRepo &rhs) noexcept;

    // f(const Article &) по возрастанию названия (обход nameIndex_, поиск каждого в articles_);
    // f не должен менять репозиторий.
    template < class F > F traverse(F f) const;

    // f(const Category &, count_type) по возрастанию категории;
    template < class F > F traverseCategories(F f) const;

    // число статей в категории
    count_type categoryCount(const Category &category) const;

  private:
    AutoHashTable< std::string, Article > articles_;
    BSTree< ArticleName, bool > nameIndex_;
    RefCountIndex< Category > categoryIndex_;
  };

  template < class F > F UserRepo::traverse(F f) const
  {
    nameIndex_.traverseLNR(
        [&](typename BSTree< UserName, bool >::const_iter::reference val)
        {
          const User *user = users_.find(val.first.value());
          assert(user != nullptr);
          f(*user);
        });
    return f;
  }

  template < class F > F ArticleRepo::traverse(F f) const
  {
    nameIndex_.traverseLNR(
        [&](typename BSTree< ArticleName, bool >::const_iter::reference val)
        {
          const Article *article = articles_.find(val.first.value());
          assert(article != nullptr);
          f(*article);
        });
    return f;
  }

  template < class F > F ArticleRepo::traverseCategories(F f) const
  {
    categoryIndex_.traverse([&](const Category &category, ArticleRepo::count_type count)
                            { f(category, count); });
    return f;
  }

}

#endif
