#ifndef ARTICLE_FILTER_HPP
#define ARTICLE_FILTER_HPP

#include "Entities.hpp"

namespace khasnulin
{
  // не владеет: категория и сложность должны жить дольше фильтра
  class ArticleFilter
  {
  public:
    // если пробросить nullptr -- фильтр отменяется (условие не задано), по критерию пропускаются все статьи.
    explicit ArticleFilter(const Category *category = nullptr,
                           const Difficulty *difficulty = nullptr) noexcept;

    // Проверяет article на удовлетворение условиям. Если условия не заданы -- всегда true
    bool matches(const Article &article) const;

    const Category *category() const noexcept;
    const Difficulty *difficulty() const noexcept;

  private:
    const Category *category_;
    const Difficulty *difficulty_;
  };
}
#endif
