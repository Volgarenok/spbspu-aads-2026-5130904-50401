#include "Recommender.hpp"
#include "BoundedPriorityQueue.hpp"
#include "Entities.hpp"
#include "Vector.hpp"
#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace
{
  class RecommendationComparator
  {
  public:
    using recommendation = khasnulin::Recommender::Recommendation;
    bool operator()(const recommendation &lhs, const recommendation &rhs) const noexcept
    {
      if (lhs.score > rhs.score)
      {
        return true;
      }
      return lhs.score == rhs.score && lhs.article->name() < rhs.article->name();
    }
  };
}

khasnulin::Recommender::Recommender(Scorer scorer, std::size_t recommendLimit,
                                    std::size_t similarLimit) noexcept:
    scorer_(scorer),
    recommendLimit_(recommendLimit),
    similarLimit_(similarLimit)
{
  assert(recommendLimit > 0);
  assert(similarLimit > 0);
}

// возвращает вектор из top-k рекомендаций Recommendation для пользователя с UserName на основе фильтров и
// его предпочтений. порядок выдачи: от лучшего к худшему, не больше лимита;
// std::invalid_argument, если нет пользователя или категории из фильтра
khasnulin::Vector< khasnulin::Recommender::Recommendation >
khasnulin::Recommender::recommend(const Storage &storage, const UserName &name,
                                  const ArticleFilter &articleFilter) const
{
  const User *user = storage.findUser(name);
  if (!user)
  {
    throw std::invalid_argument("recommender: can't find user with such name");
  }

  if (articleFilter.category() != nullptr && !storage.hasCategory(*articleFilter.category()))
  {
    throw std::invalid_argument(
        "recommender: can't find such category from article filter in category index.");
  }
  BoundedPriorityQueue< Recommendation, RecommendationComparator > queue(recommendLimit_,
                                                                         RecommendationComparator());
  std::size_t userRatings = storage.userRatingsCount(user->name());
  storage.traverseArticles(
      [&](const Article &article)
      {
        if (!articleFilter.matches(article) || storage.hasRating(name, article.name()))
        {
          return;
        }
        double score = scorer_.userArticleScore(user->latent(), user->preferences(), userRatings,
                                                article.latent(), article.tags());
        queue.suggest(Recommendation{&article, score});
      });
  Vector< Recommendation > result;
  result.reserve(queue.size());
  queue.traverseAndClear([&](const Recommendation &value) { result.pushBack(value); });
  return result;
}

// возвращает вектор из top-k  похожих Recommendation статей на статью с переданным ArticleName. порядок
// выдачи: от лучшего к худшему, не больше лимита;
// std::invalid_argument, если нет статьи
khasnulin::Vector< khasnulin::Recommender::Recommendation >
khasnulin::Recommender::similar(const Storage &storage, const ArticleName &name) const
{
  const Article *currArticle = storage.findArticle(name);
  if (!currArticle)
  {
    throw std::invalid_argument("recommender: can't find article with such name");
  }

  BoundedPriorityQueue< Recommendation, RecommendationComparator > queue(similarLimit_,
                                                                         RecommendationComparator());
  std::size_t currArticleRatings = storage.articleRatingsCount(currArticle->name());
  storage.traverseArticles(
      [&](const Article &article)
      {
        if (currArticle != std::addressof(article))
        {
          std::size_t articleRatings = storage.articleRatingsCount(article.name());
          double score =
              scorer_.articleArticleScore(currArticle->latent(), currArticle->tags(), currArticleRatings,
                                          article.latent(), article.tags(), articleRatings);
          queue.suggest(Recommendation{&article, score});
        }
      });
  Vector< Recommendation > result;
  result.reserve(queue.size());
  queue.traverseAndClear([&](const Recommendation &value) { result.pushBack(value); });
  return result;
}
