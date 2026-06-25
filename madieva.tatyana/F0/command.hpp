#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "template.hpp"
#include "game.hpp"
#include "cuckoo_hash_table.hpp"
#include "hash_functions.hpp"
namespace madieva
{
  using TemplateTable = CuckooHashTable< std::string,
    Template, StringHash1, StringHash2, StringEqual >;
  using GameTable = CuckooHashTable< std::string, Game,
    StringHash1, StringHash2, StringEqual >;

  using CommandFunction = void (*)(std::istream &, std::ostream &,
    TemplateTable & templates, GameTable & games);
  using CommandTable = CuckooHashTable< std::string, CommandFunction,
    StringHash1, StringHash2, StringEqual >;

  void cmd_load(std::istream & in, std::ostream & out,
    TemplateTable & templates, GameTable & games);
  void cmd_start(std::istream & in, std::ostream & out,
    TemplateTable & templates, GameTable & games);
  void cmd_reset(std::istream & in, std::ostream & out,
    TemplateTable & templates, GameTable & games);
  void cmd_save(std::istream & in, std::ostream & out,
    TemplateTable & templates, GameTable & games);
  void cmd_continue(std::istream & in, std::ostream & out,
    TemplateTable & templates, GameTable & games);
  void cmd_exists(std::istream & in, std::ostream & out,
    TemplateTable & templates, GameTable & games);
void cmd_fill(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_empty(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_fill_row(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_fill_col(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_empty_row(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_empty_col(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_help(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_list_tmpl(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
void cmd_list_game(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games);
}
#endif
