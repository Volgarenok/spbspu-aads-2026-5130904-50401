#include "Entities.hpp"
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

khasnulin::Category::Category(const std::string &category)
{
  std::string normalized = normalizeLabel(category);
  if (isDifficultyName(normalized))
  {
    throw std::invalid_argument("category can't be named as difficulty level");
  }
  category_ = std::move(normalized);
}

const std::string &khasnulin::Category::value() const noexcept
{
  return category_;
}

bool khasnulin::Category::operator<(const Category &rhs) const noexcept
{
  return category_ < rhs.category_;
}

bool khasnulin::Category::operator==(const Category &rhs) const noexcept
{
  return category_ == rhs.category_;
}

khasnulin::Tag::Tag(const std::string &tag):
    tag_(normalizeLabel(tag))
{
}

const std::string &khasnulin::Tag::value() const noexcept
{
  return tag_;
}

bool khasnulin::Tag::operator<(const Tag &rhs) const noexcept
{
  return tag_ < rhs.tag_;
}

bool khasnulin::Tag::operator==(const Tag &rhs) const noexcept
{
  return tag_ == rhs.tag_;
}

khasnulin::User::User():
    name_(),
    preferences_(),
    latent_{}
{
}

khasnulin::User::User(const UserName &name, const Latent &latent):
    name_(name),
    preferences_(),
    latent_(latent)
{
}

const khasnulin::UserName &khasnulin::User::name() const noexcept
{
  return name_;
}

bool khasnulin::User::addPreference(const Tag &tag)
{
  TagSet::size_type oldSize = preferences_.size();
  preferences_.insert({tag, true});
  return oldSize < preferences_.size();
}

const khasnulin::TagSet &khasnulin::User::preferences() const noexcept
{
  return preferences_;
}

const khasnulin::Latent &khasnulin::User::latent() const noexcept
{
  return latent_;
}

khasnulin::Latent &khasnulin::User::latent() noexcept
{
  return latent_;
}

khasnulin::Article::Article():
    name_(),
    category_(),
    difficulty_(Difficulty::beginner),
    tags_(),
    latent_{}
{
}

khasnulin::Article::Article(const ArticleName &name, const Category &category, const Difficulty &difficulty,
                            const Latent &latent):
    name_(name),
    category_(category),
    difficulty_(difficulty),
    tags_(),
    latent_(latent)
{
}

const khasnulin::ArticleName &khasnulin::Article::name() const noexcept
{
  return name_;
}

const khasnulin::Category &khasnulin::Article::category() const noexcept
{
  return category_;
}

khasnulin::Difficulty khasnulin::Article::difficulty() const noexcept
{
  return difficulty_;
}

bool khasnulin::Article::addTag(const Tag &tag)
{
  TagSet::size_type oldSize = tags_.size();
  tags_.insert({tag, true});
  return oldSize < tags_.size();
}

const khasnulin::TagSet &khasnulin::Article::tags() const noexcept
{
  return tags_;
}

const khasnulin::Latent &khasnulin::Article::latent() const noexcept
{
  return latent_;
}

khasnulin::Latent &khasnulin::Article::latent() noexcept
{
  return latent_;
}

std::string khasnulin::normalizeText(const std::string &text)
{
  std::string result;
  result.reserve(text.size());
  bool hasLetter = false;
  bool hasSpace = false;
  for (char ch : text)
  {
    if (!std::isprint(static_cast< unsigned char >(ch)) || (ch == '"'))
    {
      throw std::invalid_argument("text can't consist of non-printable symbols");
    }
    if (ch == ' ')
    {
      if (!result.empty())
      {
        hasSpace = true;
      }
    }
    else
    {
      if (hasSpace)
      {
        hasSpace = false;
        result.push_back(' ');
      }
      if (std::isalpha(static_cast< unsigned char >(ch)))
      {
        hasLetter = true;
      }
      result.push_back(ch);
    }
  }
  if (!hasLetter)
  {
    throw std::invalid_argument("text must have any letters");
  }
  return result;
}

std::string khasnulin::normalizeLabel(const std::string &label)
{
  std::string result = normalizeText(label);
  for (char &ch : result)
  {
    ch = static_cast< char >(std::tolower(static_cast< unsigned char >(ch)));
  }
  return result;
}

khasnulin::Difficulty khasnulin::parseDifficulty(const std::string &raw)
{
  std::string normalized = normalizeLabel(raw);
  if (normalized == toString(Difficulty::beginner))
  {
    return Difficulty::beginner;
  }
  if (normalized == toString(Difficulty::intermediate))
  {
    return Difficulty::intermediate;
  }
  if (normalized == toString(Difficulty::advanced))
  {
    return Difficulty::advanced;
  }
  throw std::invalid_argument("incorrect difficulty raw");
}

const char *khasnulin::toString(Difficulty d) noexcept
{
  switch (d)
  {
  case Difficulty::beginner:
    return "beginner";
  case Difficulty::intermediate:
    return "intermediate";
  case Difficulty::advanced:
    return "advanced";
  }
  return "";
}

bool khasnulin::isDifficultyName(const std::string &normalized) noexcept
{
  return normalized == toString(Difficulty::beginner) || normalized == toString(Difficulty::intermediate) ||
         normalized == toString(Difficulty::advanced);
}

template < class Kind >
khasnulin::Name< Kind >::Name(const std::string &name):
    value_(normalizeText(name))
{
}

template < class Kind > const std::string &khasnulin::Name< Kind >::value() const noexcept
{
  return value_;
}

template < class Kind > bool khasnulin::Name< Kind >::operator==(const Name &rhs) const noexcept
{
  return value_ == rhs.value_;
}

template < class Kind > bool khasnulin::Name< Kind >::operator<(const Name &rhs) const noexcept
{
  return value_ < rhs.value_;
}

template class khasnulin::Name< khasnulin::UserNameKind >;
template class khasnulin::Name< khasnulin::ArticleNameKind >;

khasnulin::Rating::Rating(int rating)
{
  if (rating < minRating || rating > maxRating)
  {
    throw std::invalid_argument("rating must be in [1, 10]");
  }
  rating_ = rating;
}

int khasnulin::Rating::value() const noexcept
{
  return rating_;
}

bool khasnulin::Rating::operator==(const Rating &rhs) const noexcept
{
  return rating_ == rhs.rating_;
}
