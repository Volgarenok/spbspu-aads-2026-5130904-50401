#ifndef RECOMMENDER_HPP
#define RECOMMENDER_HPP

#include "ArticleFilter.hpp"
#include "Model.hpp"
#include "Storage.hpp"
#include "Vector.hpp"
#include <cstddef>

namespace khasnulin
{
  class Recommender
  {
  public:
    // указатели в результате действительны до следующего изменения хранилища.
    struct Recommendation
    {
      const Article *article;
      double score;
    };
    static constexpr std::size_t defaultRecommendLimit = 10;
    static constexpr std::size_t defaultSimilarLimit = 5;

    explicit Recommender(Scorer scorer = Scorer(), std::size_t recommendLimit = defaultRecommendLimit,
                         std::size_t similarLimit = defaultSimilarLimit) noexcept;

    // возвращает вектор из top-k рекомендаций Recommendation для пользователя с UserName на основе фильтров и
    // его предпочтений. порядок выдачи: от лучшего к худшему, не больше лимита;
    // std::invalid_argument, если нет пользователя или категории из фильтра
    Vector< Recommendation > recommend(const Storage &storage, const UserName &name,
                                       const ArticleFilter &articleFilter) const;

    // возвращает вектор из top-k  похожих Recommendation статей на статью с переданным ArticleName. порядок
    // выдачи: от лучшего к худшему, не больше лимита;
    // std::invalid_argument, если нет статьи
    Vector< Recommendation > similar(const Storage &storage, const ArticleName &name) const;

  private:
    Scorer scorer_;

    std::size_t recommendLimit_;
    std::size_t similarLimit_;
  };

}

#endif
