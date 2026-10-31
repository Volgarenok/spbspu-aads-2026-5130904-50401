#include "Storage.hpp"
#include "Entities.hpp"

bool khasnulin::Storage::addUser(const UserName &user, const Latent &latent)
{
  return users_.add(user, latent);
}

bool khasnulin::Storage::addArticle(const ArticleName &article, const Category &category,
                                    const Difficulty &difficulty, const Latent &latent)
{
  return articles_.add(article, category, difficulty, latent);
}

bool khasnulin::Storage::addPreference(const UserName &user, const Tag &tag)
{
  User *us = users_.find(user);
  if (us == nullptr)
  {
    return false;
  }
  if (us->preferences().find(tag) != us->preferences().end())
  {
    return true;
  }
  tagsIndex_.acquire(tag);
  try
  {
    us->addPreference(tag);
  }
  catch (...)
  {
    tagsIndex_.release(tag);
    throw;
  }
  return true;
}

bool khasnulin::Storage::addArticleTag(const ArticleName &article, const Tag &tag)
{
  Article *art = articles_.find(article);
  if (art == nullptr)
  {
    return false;
  }
  if (art->tags().find(tag) != art->tags().end())
  {
    return true;
  }
  tagsIndex_.acquire(tag);
  try
  {
    art->addTag(tag);
  }
  catch (...)
  {
    tagsIndex_.release(tag);
    throw;
  }
  return true;
}

bool khasnulin::Storage::eraseUser(const UserName &user)
{
  User *us = users_.find(user);
  if (us == nullptr)
  {
    return false;
  }
  ratings_.eraseUser(user.value());
  us->preferences().traverseLNR([&](TagSet::const_iter::reference val) { tagsIndex_.release(val.first); });
  users_.erase(user);
  return true;
}

bool khasnulin::Storage::eraseArticle(const ArticleName &article)
{
  Article *art = articles_.find(article);
  if (art == nullptr)
  {
    return false;
  }
  ratings_.eraseArticle(article.value());
  art->tags().traverseLNR([&](TagSet::const_iter::reference val) { tagsIndex_.release(val.first); });
  articles_.erase(article);

  return true;
}

void khasnulin::Storage::clear()
{
  users_.clear();
  articles_.clear();
  ratings_.clear();
  tagsIndex_.clear();
}

void khasnulin::Storage::swap(Storage &rhs) noexcept
{
  users_.swap(rhs.users_);
  articles_.swap(rhs.articles_);
  ratings_.swap(rhs.ratings_);
  tagsIndex_.swap(rhs.tagsIndex_);
}

const typename khasnulin::User *khasnulin::Storage::findUser(const UserName &user) const
{
  return users_.find(user);
}

const typename khasnulin::Article *khasnulin::Storage::findArticle(const ArticleName &article) const
{
  return articles_.find(article);
}

typename khasnulin::Storage::count_type khasnulin::Storage::userRatingsCount(const UserName &user) const
{
  return ratings_.userRatingsCount(user.value());
}

typename khasnulin::Storage::count_type
khasnulin::Storage::articleRatingsCount(const ArticleName &article) const
{
  return ratings_.articleRatingsCount(article.value());
}

bool khasnulin::Storage::hasRating(const UserName &user, const ArticleName &article) const
{
  return ratings_.hasRating(user.value(), article.value());
}

bool khasnulin::Storage::hasCategory(const Category &category) const
{
  return articles_.categoryCount(category) > 0;
}

khasnulin::Storage::count_type khasnulin::Storage::articlesCount() const noexcept
{
  return articles_.size();
}

khasnulin::Storage::count_type khasnulin::Storage::usersCount() const noexcept
{
  return users_.size();
}
