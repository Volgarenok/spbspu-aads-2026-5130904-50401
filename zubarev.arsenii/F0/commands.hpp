#ifndef COMMANDS_FS_HPP
#define COMMANDS_FS_HPP

#include <iosfwd>
#include "file_system.hpp"

namespace zubarev
{

  void cmd_mkdir(std::istream&, std::ostream& out, FileSystem& file_sys);
  void cmd_rmdir(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_rm(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_touch(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_write(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_append(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_cd(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_mv(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_cp(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_cat(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_pwd(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_ls(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_tree(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_search(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_save(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_load(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_states(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_import(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_export(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_save_state(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_start_state(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_archive(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_extract(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_help(std::istream&, std::ostream& out, FileSystem&);
  void cmd_exit(std::istream&, std::ostream& out, FileSystem&);
  void cmd_test(std::istream& in, std::ostream& out, FileSystem& file_sys);

  void process_stream(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void printPrompt(const FileSystem& fs, std::ostream& out);

}

#endif
