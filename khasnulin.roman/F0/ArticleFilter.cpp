#include "ArticleFilter.hpp"

khasnulin::ArticleFilter::ArticleFilter(const Category *category, const Difficulty *difficulty) noexcept:
    category_(category),
    difficulty_(difficulty)
{
}

bool khasnulin::ArticleFilter::matches(const Article &article) const
{
  bool result = true;
  if (difficulty_ != nullptr)
  {
    result &= article.difficulty() == (*difficulty_);
  }
  if (result && category_ != nullptr)
  {
    result &= article.category() == (*category_);
  }
  return result;
}

const khasnulin::Category *khasnulin::ArticleFilter::category() const noexcept
{
  return category_;
}
const khasnulin::Difficulty *khasnulin::ArticleFilter::difficulty() const noexcept
{
  return difficulty_;
}
