#include "CommandProcessor.hpp"
#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

using namespace khasnulin;

namespace
{
  const std::string invalidCommand = "<INVALID COMMAND>\n";

  std::string runCommands(const std::string &input)
  {
    std::istringstream in(input);
    std::ostringstream out;
    CommandProcessor processor;
    processor.run(in, out);
    return out.str();
  }

  std::string repeatInvalid(int count)
  {
    std::string result;
    for (int i = 0; i < count; ++i)
    {
      result += invalidCommand;
    }
    return result;
  }

  bool fileExists(const std::string &path)
  {
    std::ifstream file(path);
    return file.is_open();
  }

  std::string readFile(const std::string &path)
  {
    std::ifstream file(path);
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
  }

  void writeFile(const std::string &path, const std::string &content)
  {
    std::ofstream file(path);
    file << content;
  }

  struct SessionFile
  {
    std::string path;

    explicit SessionFile(const std::string &name):
        path("test-session-" + name + ".txt")
    {
      std::remove(path.c_str());
      std::remove((path + ".tmp").c_str());
    }

    ~SessionFile()
    {
      std::remove(path.c_str());
      std::remove((path + ".tmp").c_str());
    }
  };

  const std::string catalog = "add-user roman cpp templates\n"
                              "add-user alex python\n"
                              "add-article \"Advanced C++\" cpp advanced cpp templates\n"
                              "add-article \"Intro to Python\" python beginner python\n"
                              "add-article \"C++ Basics\" cpp beginner cpp\n"
                              "add-article \"Python Internals\" python advanced python\n"
                              "rate-article roman \"Advanced C++\" 10\n"
                              "rate-article alex \"Intro to Python\" 9\n"
                              "rate-article alex \"Advanced C++\" 3\n"
                              "rate-article roman \"Intro to Python\" 4\n";

  const std::string inspection = "show-users\n"
                                 "list-articles\n"
                                 "show-tags\n"
                                 "show-articles-categories\n"
                                 "show-user-info roman\n"
                                 "show-user-ratings roman\n"
                                 "show-article-info \"Advanced C++\"\n"
                                 "recommend roman\n"
                                 "recommend alex\n"
                                 "similar \"Advanced C++\"\n";
}

BOOST_AUTO_TEST_SUITE(save_tests)

BOOST_AUTO_TEST_CASE(save_prints_nothing_and_creates_file)
{
  SessionFile session("save-creates");
  BOOST_CHECK(runCommands("add-user roman\nsave " + session.path + "\n") == "");
  BOOST_CHECK(fileExists(session.path));
}

BOOST_AUTO_TEST_CASE(save_writes_successful_mutating_commands_in_order)
{
  SessionFile session("save-journal");
  std::string input = "add-user roman cpp\n"
                      "add-article \"Advanced C++\" cpp advanced\n"
                      "rate-article roman \"Advanced C++\" 10\n"
                      "rate-article roman \"Advanced C++\" 7\n"
                      "save " +
                      session.path + "\n";
  runCommands(input);
  std::string expected = "add-user roman cpp\n"
                         "add-article \"Advanced C++\" cpp advanced\n"
                         "rate-article roman \"Advanced C++\" 10\n"
                         "rate-article roman \"Advanced C++\" 7\n";
  BOOST_CHECK(readFile(session.path) == expected);
}

BOOST_AUTO_TEST_CASE(save_skips_failed_and_non_mutating_commands)
{
  SessionFile session("save-skips");
  std::string input = "add-user roman\n"
                      "add-user roman\n"
                      "show-users\n"
                      "add-article Broken cpp advanced\n"
                      "recommend roman\n"
                      "delete-user ghost\n"
                      "unknown-command\n"
                      "\n"
                      "save " +
                      session.path + "\n";
  runCommands(input);
  BOOST_CHECK(readFile(session.path) == "add-user roman\n");
}

BOOST_AUTO_TEST_CASE(save_keeps_deletions_in_history)
{
  SessionFile session("save-deletions");
  std::string input = "add-user roman\n"
                      "add-user alex\n"
                      "delete-user alex\n"
                      "save " +
                      session.path + "\n";
  runCommands(input);
  BOOST_CHECK(readFile(session.path) == "add-user roman\nadd-user alex\ndelete-user alex\n");
}

BOOST_AUTO_TEST_CASE(save_is_not_written_to_journal)
{
  SessionFile first("save-twice-first");
  SessionFile second("save-twice-second");
  runCommands("add-user roman\nsave " + first.path + "\nsave " + second.path + "\n");
  BOOST_CHECK(readFile(second.path) == "add-user roman\n");
}

BOOST_AUTO_TEST_CASE(save_of_empty_session_creates_empty_file)
{
  SessionFile session("save-empty");
  BOOST_CHECK(runCommands("save " + session.path + "\n") == "");
  BOOST_CHECK(fileExists(session.path));
  BOOST_CHECK(readFile(session.path) == "");
}

BOOST_AUTO_TEST_CASE(save_overwrites_existing_file)
{
  SessionFile session("save-overwrite");
  writeFile(session.path, "old content\n");
  runCommands("add-user roman\nsave " + session.path + "\n");
  BOOST_CHECK(readFile(session.path) == "add-user roman\n");
}

BOOST_AUTO_TEST_CASE(save_leaves_no_temporary_file)
{
  SessionFile session("save-no-tmp");
  runCommands("add-user roman\nsave " + session.path + "\n");
  BOOST_CHECK(fileExists(session.path + ".tmp") == false);
}

BOOST_AUTO_TEST_CASE(save_to_unwritable_path_is_invalid)
{
  BOOST_CHECK(runCommands("add-user roman\nsave no-such-directory/session.txt\n") == invalidCommand);
}

BOOST_AUTO_TEST_CASE(save_with_wrong_arguments_is_invalid)
{
  SessionFile session("save-arguments");
  std::string input = "add-user roman\n"
                      "save\n"
                      "save \"" +
                      session.path + "\"\n" + "save " + session.path + " extra\n";
  BOOST_CHECK(runCommands(input) == repeatInvalid(3));
  BOOST_CHECK(fileExists(session.path) == false);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(load_tests)

BOOST_AUTO_TEST_CASE(load_prints_nothing)
{
  SessionFile session("load-silent");
  runCommands("add-user roman\nsave " + session.path + "\n");
  BOOST_CHECK(runCommands("load " + session.path + "\n") == "");
}

BOOST_AUTO_TEST_CASE(load_in_new_process_restores_same_state)
{
  SessionFile session("load-new-process");
  std::string before = runCommands(catalog + inspection + "save " + session.path + "\n");
  std::string after = runCommands("load " + session.path + "\n" + inspection);
  BOOST_CHECK(after == before);
}

BOOST_AUTO_TEST_CASE(load_restores_state_after_changes)
{
  SessionFile session("load-after-changes");
  std::string expected = runCommands(catalog + inspection);
  std::string input = catalog + "save " + session.path + "\n" +
                      "delete-user alex\n"
                      "delete-article \"C++ Basics\"\n"
                      "rate-article roman \"Python Internals\" 1\n"
                      "add-user ivan go\n"
                      "load " +
                      session.path + "\n" + inspection;
  BOOST_CHECK(runCommands(input) == expected);
}

BOOST_AUTO_TEST_CASE(load_restores_trained_vectors)
{
  SessionFile session("load-vectors");
  std::string input = catalog +
                      "recommend roman\n"
                      "similar \"Intro to Python\"\n"
                      "save " +
                      session.path + "\n" +
                      "rate-article roman \"C++ Basics\" 1\n"
                      "rate-article alex \"Python Internals\" 10\n"
                      "load " +
                      session.path + "\n" +
                      "recommend roman\n"
                      "similar \"Intro to Python\"\n";
  std::string firstHalf = runCommands(catalog + "recommend roman\nsimilar \"Intro to Python\"\n");
  BOOST_CHECK(runCommands(input) == firstHalf + firstHalf);
}

BOOST_AUTO_TEST_CASE(save_load_save_gives_same_file)
{
  SessionFile first("roundtrip-first");
  SessionFile second("roundtrip-second");
  runCommands(catalog + "delete-user alex\nsave " + first.path + "\n");
  runCommands("load " + first.path + "\nsave " + second.path + "\n");
  BOOST_CHECK(readFile(second.path) == readFile(first.path));
}

BOOST_AUTO_TEST_CASE(history_continues_after_load)
{
  SessionFile first("continue-first");
  SessionFile second("continue-second");
  runCommands("add-user roman\nsave " + first.path + "\n");
  runCommands("load " + first.path + "\nadd-user alex\nsave " + second.path + "\n");
  BOOST_CHECK(readFile(second.path) == "add-user roman\nadd-user alex\n");
  BOOST_CHECK(runCommands("load " + second.path + "\nshow-users\n") == "alex\nroman\n");
}

BOOST_AUTO_TEST_CASE(load_replaces_state_instead_of_merging)
{
  SessionFile session("load-replaces");
  runCommands("add-user roman\nsave " + session.path + "\n");
  BOOST_CHECK(runCommands("add-user alex\nload " + session.path + "\nshow-users\n") == "roman\n");
}

BOOST_AUTO_TEST_CASE(repeated_load_gives_same_state)
{
  SessionFile session("load-repeated");
  runCommands("add-user roman\nadd-user alex\nsave " + session.path + "\n");
  std::string input = "load " + session.path + "\nload " + session.path + "\nshow-users\n";
  BOOST_CHECK(runCommands(input) == "alex\nroman\n");
}

BOOST_AUTO_TEST_CASE(load_of_empty_file_clears_state)
{
  SessionFile session("load-empty");
  writeFile(session.path, "");
  BOOST_CHECK(runCommands("add-user roman\nload " + session.path + "\nshow-users\n") == "");
}

BOOST_AUTO_TEST_CASE(load_accepts_seed_style_file)
{
  SessionFile session("load-seed");
  writeFile(session.path, "add-user olga-ivanova politics markets\n"
                          "\n"
                          "add-article \"Stocks fall, bonds rise\" finance intermediate markets newsus\r\n"
                          "   rate-article olga-ivanova \"Stocks fall, bonds rise\" 9   \n"
                          "add-user-preferences olga-ivanova newsus\n"
                          "add-article-tags \"Stocks fall, bonds rise\" economy");
  std::string input = "load " + session.path +
                      "\n"
                      "show-user-info olga-ivanova\n"
                      "show-article-info \"Stocks fall, bonds rise\"\n";
  std::string expected = "User: olga-ivanova\n"
                         "Preferences: markets newsus politics\n"
                         "Ratings count: 1\n"
                         "Article: \"Stocks fall, bonds rise\"\n"
                         "Category: finance | Difficulty: intermediate\n"
                         "Tags: economy markets newsus\n"
                         "Avg Rating: 9.0/10 (1 ratings)\n";
  BOOST_CHECK(runCommands(input) == expected);
}

BOOST_AUTO_TEST_CASE(work_continues_after_load)
{
  SessionFile session("load-continue");
  writeFile(session.path, "add-user roman\n");
  BOOST_CHECK(runCommands("load " + session.path + "\nshow-difficulties\n") ==
              "beginner\nintermediate\nadvanced\n");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(load_errors_tests)

BOOST_AUTO_TEST_CASE(missing_file_is_invalid_and_state_is_kept)
{
  std::string input = "add-user roman\n"
                      "load test-session-does-not-exist.txt\n"
                      "show-users\n";
  BOOST_CHECK(runCommands(input) == invalidCommand + "roman\n");
}

BOOST_AUTO_TEST_CASE(wrong_arguments_are_invalid)
{
  SessionFile session("load-arguments");
  writeFile(session.path, "add-user alex\n");
  std::string input = "add-user roman\n"
                      "load\n"
                      "load \"" +
                      session.path + "\"\n" + "load " + session.path + " extra\n" + "show-users\n";
  BOOST_CHECK(runCommands(input) == repeatInvalid(3) + "roman\n");
}

BOOST_AUTO_TEST_CASE(invalid_line_rejects_whole_file)
{
  SessionFile session("load-invalid-line");
  writeFile(session.path, "add-user alex\n"
                          "add-article \"Advanced C++\" cpp expert\n"
                          "add-user ivan\n");
  BOOST_CHECK(runCommands("add-user roman\nload " + session.path + "\nshow-users\n") ==
              invalidCommand + "roman\n");
}

BOOST_AUTO_TEST_CASE(failing_command_rejects_whole_file)
{
  SessionFile session("load-failing-command");
  writeFile(session.path, "add-user alex\n"
                          "add-user alex\n");
  BOOST_CHECK(runCommands("add-user roman\nload " + session.path + "\nshow-users\n") ==
              invalidCommand + "roman\n");
}

BOOST_AUTO_TEST_CASE(reference_to_missing_entity_rejects_whole_file)
{
  SessionFile session("load-missing-reference");
  writeFile(session.path, "add-user alex\n"
                          "rate-article alex \"Advanced C++\" 5\n");
  BOOST_CHECK(runCommands("add-user roman\nload " + session.path + "\nshow-users\n") ==
              invalidCommand + "roman\n");
}

BOOST_AUTO_TEST_CASE(tokenizer_error_rejects_whole_file)
{
  SessionFile session("load-tokenizer-error");
  writeFile(session.path, "add-user alex\n"
                          "add-article \"Advanced C++ cpp advanced\n");
  BOOST_CHECK(runCommands("add-user roman\nload " + session.path + "\nshow-users\n") ==
              invalidCommand + "roman\n");
}

BOOST_AUTO_TEST_CASE(non_mutating_commands_reject_whole_file)
{
  SessionFile session("load-non-mutating");
  SessionFile other("load-non-mutating-other");
  const std::string forbidden[] = {"show-users", "recommend alex",     "show-difficulties",
                                   "exit",       "save " + other.path, "load " + other.path};
  for (const std::string &line : forbidden)
  {
    writeFile(session.path, "add-user alex\n" + line + "\nadd-user ivan\n");
    BOOST_CHECK_MESSAGE(runCommands("add-user roman\nload " + session.path + "\nshow-users\n") ==
                            invalidCommand + "roman\n",
                        "forbidden line: " << line);
  }
  BOOST_CHECK(fileExists(other.path) == false);
}

BOOST_AUTO_TEST_CASE(unknown_command_rejects_whole_file)
{
  SessionFile session("load-unknown");
  writeFile(session.path, "add-user alex\nunknown-command\n");
  BOOST_CHECK(runCommands("add-user roman\nload " + session.path + "\nshow-users\n") ==
              invalidCommand + "roman\n");
}

BOOST_AUTO_TEST_CASE(failed_load_keeps_journal)
{
  SessionFile broken("load-keeps-journal-broken");
  SessionFile saved("load-keeps-journal-saved");
  writeFile(broken.path, "add-user alex\nshow-users\n");
  runCommands("add-user roman\nload " + broken.path + "\nsave " + saved.path + "\n");
  BOOST_CHECK(readFile(saved.path) == "add-user roman\n");
}

BOOST_AUTO_TEST_SUITE_END()
