#ifndef COMMAND_ARGUMENTS_HPP
#define COMMAND_ARGUMENTS_HPP

#include "Entities.hpp"
#include "Tokenizer.hpp"
#include <Vector.hpp>
#include <cstddef>
#include <string>

namespace khasnulin
{
  // читатель не владеет токенами; начинает после имени команды; finish() вызывается до изменений хранилища
  class ArgumentReader
  {
  public:
    explicit ArgumentReader(const Vector< Token > &tokens);
    ArgumentReader(Vector< Token > &&tokens) = delete;

    UserName userName();
    Category category();
    Difficulty difficulty();

    std::string sessionName();

    ArticleName articleName();

    Rating rating();
    std::size_t limit();

    TagSet restTags();

    std::size_t remaining() const noexcept;

    bool nextIsDifficulty() const noexcept;

    void finish() const;

  private:
    const Token &nextQuoted();
    const Token &nextPlain();
    const Vector< Token > &tokens_;
    std::size_t position_;
  };

  std::size_t parseUnsignedNumber(const std::string &str);
}

#endif
