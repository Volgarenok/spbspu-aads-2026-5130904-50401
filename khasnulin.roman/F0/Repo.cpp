#include "Repo.hpp"
#include "BSTree.hpp"
#include "Entities.hpp"

bool khasnulin::UserRepo::add(const UserName &name, const Latent &latent)
{
  if (nameIndex_.find(name) != nameIndex_.end())
  {
    return false;
  }
  users_.add(name.value(), User(name, latent));
  try
  {
    nameIndex_.insert({name, true});
  }
  catch (...)
  {
    users_.erase(name.value());
    throw;
  }
  return true;
}

bool khasnulin::UserRepo::erase(const UserName &name)
{
  bool erased = users_.erase(name.value());
  if (!erased)
  {
    return false;
  }
  nameIndex_.erase(name);
  return erased;
}

khasnulin::User *khasnulin::UserRepo::find(const UserName &name)
{
  return users_.find(name.value());
}

const khasnulin::User *khasnulin::UserRepo::find(const UserName &name) const
{
  return users_.find(name.value());
}

khasnulin::UserRepo::size_type khasnulin::UserRepo::size() const noexcept
{
  return nameIndex_.size();
}

bool khasnulin::UserRepo::empty() const noexcept
{
  return nameIndex_.empty();
}

void khasnulin::UserRepo::clear()
{
  users_.clear();
  nameIndex_.clear();
}

void khasnulin::UserRepo::swap(UserRepo &rhs) noexcept
{
  users_.swap(rhs.users_);
  nameIndex_.swap(rhs.nameIndex_);
}

bool khasnulin::ArticleRepo::add(const ArticleName &name, const Category &category,
                                 const Difficulty &difficulty, const Latent &latent)
{
  if (nameIndex_.find(name) != nameIndex_.end())
  {
    return false;
  }
  articles_.add(name.value(), Article(name, category, difficulty, latent));
  try
  {
    nameIndex_.insert({name, true});
    categoryIndex_.acquire(category);
  }
  catch (...)
  {
    articles_.erase(name.value());
    nameIndex_.erase(name);
    throw;
  }
  return true;
}

bool khasnulin::ArticleRepo::erase(const ArticleName &name)
{
  const Article *article = articles_.find(name.value());
  if (!article)
  {
    return false;
  }
  categoryIndex_.release(article->category());
  articles_.erase(name.value());
  nameIndex_.erase(name);
  return true;
}

khasnulin::Article *khasnulin::ArticleRepo::find(const ArticleName &name)
{
  return articles_.find(name.value());
}

const khasnulin::Article *khasnulin::ArticleRepo::find(const ArticleName &name) const
{
  return articles_.find(name.value());
}

khasnulin::ArticleRepo::size_type khasnulin::ArticleRepo::size() const noexcept
{
  return nameIndex_.size();
}

bool khasnulin::ArticleRepo::empty() const noexcept
{
  return nameIndex_.empty();
}

void khasnulin::ArticleRepo::clear()
{
  articles_.clear();
  nameIndex_.clear();
  categoryIndex_.clear();
}

void khasnulin::ArticleRepo::swap(ArticleRepo &rhs) noexcept
{
  articles_.swap(rhs.articles_);
  nameIndex_.swap(rhs.nameIndex_);
  categoryIndex_.swap(rhs.categoryIndex_);
}

typename khasnulin::ArticleRepo::count_type
khasnulin::ArticleRepo::categoryCount(const Category &category) const
{
  return categoryIndex_.count(category);
}
