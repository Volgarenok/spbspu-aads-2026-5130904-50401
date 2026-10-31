#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include "BSTree.hpp"
#include <cstddef>
#include <string>

namespace khasnulin
{
  enum class Difficulty : unsigned char
  {
    beginner,
    intermediate,
    advanced
  };

  class Category
  {
  public:
    // Нормализует через normalizeLabel и запрещает имена уровней сложности
    // ("advanced cpp" допустимо, "advanced" — нет). При нарушении бросает std::invalid_argument.
    explicit Category(const std::string &category);
    const std::string &value() const noexcept;

    bool operator<(const Category &rhs) const noexcept;
    bool operator==(const Category &rhs) const noexcept;

  private:
    Category() = default;

    std::string category_;
    friend class Article;
  };

  class Tag
  {
  public:
    explicit Tag(const std::string &tag);
    const std::string &value() const noexcept;

    bool operator<(const Tag &rhs) const noexcept;

    bool operator==(const Tag &rhs) const noexcept;

  private:
    std::string tag_;
  };

  using TagSet = BSTree< Tag, bool >;

  class User;
  class Article;

  template < class Kind > class Name
  {
  public:
    // бросает std::invalid_argument, если name не проходит normalizeText
    explicit Name(const std::string &name);
    const std::string &value() const noexcept;

    bool operator==(const Name &rhs) const noexcept;
    bool operator<(const Name &rhs) const noexcept;

  private:
    Name() = default;

    std::string value_;
    friend class User;
    friend class Article;
  };

  struct UserNameKind;
  struct ArticleNameKind;

  using UserName = Name< UserNameKind >;
  using ArticleName = Name< ArticleNameKind >;

  extern template class Name< UserNameKind >;
  extern template class Name< ArticleNameKind >;

  constexpr std::size_t latentDimensionCount = 8;

  struct Latent
  {
    double v[latentDimensionCount];
  };

  class User
  {
  public:
    // Только для HashTable; доменный код такие объекты не создаёт и не читает.
    // Пустое имя, нулевой латентный вектор, нет предпочтений.
    User();

    // Все аргументы валидны по построению, std::invalid_argument не бросает
    // (возможен только std::bad_alloc).
    User(const UserName &name, const Latent &latent);

    const UserName &name() const noexcept;

    bool addPreference(const Tag &tag);
    const TagSet &preferences() const noexcept;

    const Latent &latent() const noexcept;
    Latent &latent() noexcept;

  private:
    UserName name_;
    TagSet preferences_;
    Latent latent_;
  };

  class Article
  {

  public:
    // Только для HashTable; доменный код такие объекты не создаёт и не читает.
    // Пустое имя, пустая категория, beginner, нулевой латентный вектор, нет тегов.
    Article();

    // Все аргументы валидны по построению, std::invalid_argument не бросает
    // (возможен только std::bad_alloc).
    Article(const ArticleName &name, const Category &category, const Difficulty &difficulty,
            const Latent &latent);

    const ArticleName &name() const noexcept;
    const Category &category() const noexcept;
    Difficulty difficulty() const noexcept;

    bool addTag(const Tag &tag);
    const TagSet &tags() const noexcept;

    const Latent &latent() const noexcept;
    Latent &latent() noexcept;

  private:
    ArticleName name_;
    Category category_;
    Difficulty difficulty_;
    TagSet tags_;
    Latent latent_;
  };

  class Rating
  {
  public:
    static constexpr int minRating = 1;
    static constexpr int maxRating = 10;

    explicit Rating(int rating);
    int value() const noexcept;

    bool operator==(const Rating &rhs) const noexcept;

  private:
    int rating_;
  };

  // Приводит строку к единому формату: слова, разделённые ровно одним пробелом.
  // Словом считается любая последовательность латинских букв, цифр и спецсимволов (@, +, -, ~ и т. д.).
  // Правила:
  //   - допустимы только печатные ASCII-символы (0x20–0x7E), кроме ";
  //   - в строке должна быть хотя бы одна латинская буква;
  //   - пробелы в начале и в конце удаляются, серии пробелов внутри схлопываются в один;
  //   - результат приводится к нижнему регистру.
  // Если нарушено хотя бы одно правило, бросает std::invalid_argument.
  // Рассчитана на локаль "C" (использует <cctype>).
  std::string normalizeLabel(const std::string &label);

  // То же, что и normalizeLabel, но регистр сохраняется.
  std::string normalizeText(const std::string &text);

  // нормализует raw и сверяет по спец. значениям difficulty, если ничего не подошло -- бросает
  // std::invalid argument
  Difficulty parseDifficulty(const std::string &raw);

  // Перевод из difficulty в строку. Варианты строк: "beginner" / "intermediate" / "advanced"
  const char *toString(Difficulty d) noexcept;

  // Нужна, чтобы при вводе категории не было конфликта или путаницы с сложностью. Правило для названия
  // категории: имя категории не должно совпадать с спец именами использующимися в системе. В данном случае
  // сверяем с спец. именами для уровней сложности
  bool isDifficultyName(const std::string &normalized) noexcept;
}

#endif
