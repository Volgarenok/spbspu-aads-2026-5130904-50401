#ifndef COMMAND_PROCESSOR_HPP
#define COMMAND_PROCESSOR_HPP

#include "ArgumentReader.hpp"
#include "Entities.hpp"
#include "Model.hpp"
#include "Recommender.hpp"
#include "Storage.hpp"
#include <iosfwd>
#include <string>

namespace khasnulin
{

  class CommandProcessor
  {
  public:
    CommandProcessor(const CommandProcessor &rhs) = delete;
    CommandProcessor &operator=(const CommandProcessor &rhs) = delete;

    explicit CommandProcessor(Learner learner = Learner(), Recommender recommender = Recommender());

    void run(std::istream &input, std::ostream &output);

  private:
    using Handler = void (CommandProcessor::*)(ArgumentReader &, std::ostream &);

    struct CommandInfo
    {
      Handler handler = nullptr;
      bool isMutating = false;
    };

    Learner learner_;
    Recommender recommender_;
    Storage storage_;
    BSTree< std::string, CommandInfo > handlers_;
    Vector< std::string > journal_;

    void recommendWithFilter(ArgumentReader &arguments, std::ostream &output, const UserName &name,
                             const Category *category, const Difficulty *difficulty);

    bool executeLine(const std::string &line, std::ostream &output, bool isReplay);

    void showDifficulties(ArgumentReader &arguments, std::ostream &output);
    void addUser(ArgumentReader &arguments, std::ostream &output);
    void addUserPreferences(ArgumentReader &arguments, std::ostream &output);
    void showUsers(ArgumentReader &arguments, std::ostream &output);
    void deleteUser(ArgumentReader &arguments, std::ostream &output);
    void showUserInfo(ArgumentReader &arguments, std::ostream &output);
    void addArticle(ArgumentReader &arguments, std::ostream &output);
    void addArticleTags(ArgumentReader &arguments, std::ostream &output);
    void showArticleInfo(ArgumentReader &arguments, std::ostream &output);
    void deleteArticle(ArgumentReader &arguments, std::ostream &output);
    void listArticles(ArgumentReader &arguments, std::ostream &output);
    void showUserRatings(ArgumentReader &arguments, std::ostream &output);
    void rateArticle(ArgumentReader &arguments, std::ostream &output);

    void showTags(ArgumentReader &arguments, std::ostream &output);
    void showArticleCategories(ArgumentReader &arguments, std::ostream &output);

    void similar(ArgumentReader &arguments, std::ostream &output);
    void recommend(ArgumentReader &arguments, std::ostream &output);

    void save(ArgumentReader &arguments, std::ostream &output);
    void load(ArgumentReader &arguments, std::ostream &output);
  };
}

#endif
