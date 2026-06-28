#include "commands.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "utils.hpp"

void zubarev::cmd_mkdir(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    file_sys.mkdir(name);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}
void zubarev::cmd_rmdir(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    file_sys.rmdir(name);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_rm(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    file_sys.rm(name);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_touch(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    file_sys.touch(name);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_write(std::istream& in, std::ostream& out, FileSystem& file_sys)
{

  std::string line;
  std::getline(in, line);

  std::istringstream iss(line);
  std::string name;

  if (!(iss >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::string rest;
  std::getline(iss, rest);

  size_t start = rest.find_first_not_of(" \t");
  if (start == std::string::npos) {
    out << "write: missing text\n";
    return;
  }
  rest = rest.substr(start);

  if (rest.size() < 2 || rest.front() != '"' || rest.back() != '"') {
    out << "write: text must be in quotes\n";
    return;
  }

  std::string text = rest.substr(1, rest.size() - 2);
  try {
    file_sys.write(name, text);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_append(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string line;
  std::getline(in, line);

  std::istringstream iss(line);
  std::string name;

  if (!(iss >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::string rest;
  std::getline(iss, rest);

  size_t start = rest.find_first_not_of(" \t");
  if (start == std::string::npos) {
    out << "append: missing text\n";
    return;
  }
  rest = rest.substr(start);

  if (rest.size() < 2 || rest.front() != '"' || rest.back() != '"') {
    out << "append: text must be in quotes\n";
    return;
  }

  std::string text = rest.substr(1, rest.size() - 2);
  try {
    file_sys.append(name, text);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_cd(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string path;
  if (!(in >> path)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    file_sys.cd(path);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_mv(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string from, to;
  if (!(in >> from >> to)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    file_sys.mv(from, to);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_cp(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string from, to;
  if (!(in >> from >> to)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    file_sys.cp(from, to);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_cat(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    std::string text_out = file_sys.cat(name);
    out << text_out << '\n';
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_pwd(std::istream&, std::ostream& out, FileSystem& file_sys)
{
  try {
    out << file_sys.pwd() << "\n";
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_ls(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string path = "";
  if (in.peek() != '\n' && in.peek() != EOF) {
    in >> path;
  }
  try {
    Vector< std::string > names = file_sys.ls(path);
    std::sort(names.begin(), names.end());
    out << detail::formatLsColumns(names);
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_tree(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string path = "";
  if (in.peek() != '\n' && in.peek() != EOF) {
    in >> path;
  }
  try {
    std::tuple< std::string, size_t, size_t > res = file_sys.tree(path);
    out << std::get< 0 >(res) << "\n";
    out << std::get< 1 >(res) << " directories, ";
    out << std::get< 2 >(res) << " files\n";
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_search(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string query;
  std::getline(in >> std::ws, query);

  Vector< std::shared_ptr< FSNode > > results = file_sys.search(query);

  if (results.isEmpty()) {
    out << "No matches found.\n";
    return;
  }

  auto buildPath = [](std::shared_ptr< FSNode > node) -> std::string {
    std::string path = node->getName();
    std::shared_ptr< Directory > parent = node->getParent();
    while (parent) {
      path = parent->getName() + "/" + path;
      parent = parent->getParent();
    }
    return path;
  };

  auto handleFileActions = [&](std::shared_ptr< FSNode > node) {
    std::string path = buildPath(node);
    out << "Selected: " << path << "\n";

    if (node->isDirectory()) {
      out << "<This is a directory. File operations are not allowed>\n";
      return;
    }

    out << "What would you like to do?\n"
        << "  [1] Read (cat)\n"
        << "  [2] Overwrite\n"
        << "  [3] Append\n"
        << "  [4] Cancel\n"
        << "Select: ";

    std::string choice;
    std::getline(std::cin, choice);

    try {
      if (choice == "1") {
        out << file_sys.cat(path) << '\n';

      } else if (choice == "2") {
        out << "Enter new content (end with empty line):\n";
        std::string content;
        std::string line;
        while (std::getline(std::cin, line) && !line.empty()) {
          if (!content.empty())
            content += "\n";
          content += line;
        }
        file_sys.write(path, content);
        out << "File overwritten.\n";

      } else if (choice == "3") {
        out << "Enter content to append (end with empty line or 'q'):\n";
        std::string content;
        std::string line;
        while (std::getline(std::cin, line)) {
          if (line.empty() || line == "q") {
            break;
          }

          if (!content.empty()) {
            content += "\n";
          }

          content += line;
        }
        file_sys.append(path, content);
        out << "Content appended.\n";

      } else if (choice == "4" || choice.empty()) {
        out << "Cancelled.\n";

      } else {
        out << "Invalid choice.\n";
      }
    } catch (const std::exception& e) {
      out << "<ERROR: " << e.what() << ">\n";
    }
  };

  if (results.getSize() == 1) {
    out << "Found 1 file.\n";
    handleFileActions(results[0]);
    return;
  }

  out << "Found " << results.getSize() << " files:\n";
  for (size_t i = 0; i < results.getSize(); ++i) {
    std::string path = buildPath(results[i]);
    out << "  [" << i + 1 << "] " << path << "\n";
  }

  out << "Select (number or prefix): " << std::flush;
  std::string choice;
  std::getline(std::cin, choice);

  if (choice.empty() || choice == "q") {
    return;
  }

  try {
    size_t idx = std::stoul(choice);
    if (idx >= 1 && idx <= results.getSize()) {
      handleFileActions(results[idx - 1]);
      return;
    }
  } catch (const std::invalid_argument&) {

  } catch (const std::out_of_range&) {
  }

  Vector< std::shared_ptr< FSNode > > filtered;
  for (size_t i = 0; i < results.getSize(); ++i) {
    const std::string& name = results[i]->getName();
    if (name.size() >= choice.size() && name.substr(0, choice.size()) == choice) {
      filtered.pushBack(results[i]);
    }
  }

  if (filtered.isEmpty()) {
    out << "No match for '" << choice << "'\n";

  } else if (filtered.getSize() == 1) {
    handleFileActions(filtered[0]);
    return;

  } else {
    out << "Found " << filtered.getSize() << " matches for prefix '" << choice << "':\n";
    for (size_t i = 0; i < filtered.getSize(); ++i) {
      std::string path = buildPath(filtered[i]);
      out << "  [" << i + 1 << "] " << path << "\n";
    }
    out << "Select number: ";
    std::string numChoice;
    std::getline(std::cin, numChoice);
    try {
      size_t idx = std::stoul(numChoice);
      if (idx >= 1 && idx <= filtered.getSize()) {
        handleFileActions(filtered[idx - 1]);
        return;
      } else {
        out << "Invalid selection.\n";
      }
    } catch (...) {
      out << "Invalid input.\n";
    }
  }
}

void zubarev::cmd_save(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string path;
  if (!(in >> path)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    if (file_sys.save(path)) {
      out << "<SAVED>\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_load(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    if (file_sys.load(name)) {
      out << "<LOADED>\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_states(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string path = "";
  if (in.peek() != '\n' && in.peek() != EOF) {
    in >> path;
  }
  try {
    Vector< FileSystem::StateInfo > states_list = file_sys.states(path);
    if (states_list.isEmpty()) {
      out << "<NO STATES FOUND>\n";
    } else {

      for (auto info = states_list.begin(); info != states_list.end(); ++info) {
        out << info->name << " [" << info->size_kb << "KB, " << info->date << "]\n";
      }
    }
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_import(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string real_path, virtual_name;
  if (!(in >> real_path >> virtual_name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    if (file_sys.import_file(real_path, virtual_name)) {
      out << "<IMPORTED: " << real_path << " -> " << virtual_name << ">\n";
    }
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_export(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string virtual_name, real_path;
  if (!(in >> virtual_name >> real_path)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    if (file_sys.export_file(virtual_name, real_path)) {
      out << "<EXPORTED: " << virtual_name << " -> " << real_path << ">\n";
    }
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_save_state(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string line;
  std::getline(in, line);

  std::istringstream iss(line);

  std::string name = "";
  bool rewrite = false;

  if (!(iss >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  iss >> std::boolalpha >> rewrite;

  try {
    file_sys.save_state(name, rewrite);
    out << "<STATE SAVED COMPLETE>" << '\n';

  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_archive(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string line;
  std::getline(in, line);

  std::istringstream iss(line);

  std::string arhive_name = "";
  std::string dir_path = "";

  if (!(iss >> arhive_name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  iss >> dir_path;
  if (dir_path.empty()) {
    dir_path = file_sys.pwd();
  }

  try {
    if (file_sys.archive(arhive_name, dir_path)) {
      out << "<ARCHIVED: " << dir_path << " -> " << arhive_name << ">\n";
    }
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_extract(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string line;
  std::getline(in, line);

  std::istringstream iss(line);

  std::string arhive_name = "";
  std::string dir_path = "";

  if (!(iss >> arhive_name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  iss >> dir_path;
  if (dir_path.empty()) {
    dir_path = file_sys.pwd();
  }

  try {
    if (file_sys.extract(arhive_name, dir_path)) {
      out << "<EXTRACTED: " << dir_path << " -> " << arhive_name << ">\n";
    }
  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::cmd_start_state(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string line;
  std::getline(in, line);
  std::istringstream iss(line);

  std::string name;
  if (!(iss >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!file_sys.isEmpty()) {
    out << "WARNING: Current file system is not empty.\n";
    out << "Loading a new state will affect your current files.\n";
    out << "Choose an option:\n";
    out << "  1. Save current state to a new file, then load\n";
    out << "  2. Discard current state and load new\n";
    out << "  3. Cancel loading\n";
    out << "Enter choice (1/2/3): ";
    out.flush();

    std::string choice_line;
    std::getline(in, choice_line);

    std::istringstream choice_stream(choice_line);
    int choice = 0;
    choice_stream >> choice;
    if (choice == 1) {
      out << "Enter filename to save current state (e.g., backup.state): ";
      out.flush();

      std::string backup_name;
      std::getline(in, backup_name);
      std::istringstream name_stream(backup_name);
      name_stream >> backup_name;

      if (backup_name.empty()) {
        out << "<INVALID COMMAND>: empty backup filename\n";
        return;
      }

      try {
        file_sys.save_state(backup_name, false);
        out << "<STATE SAVED: " << backup_name << ">\n";
      } catch (const std::exception& e) {
        out << "Failed to save backup: " << e.what() << "\n";
        out << "Loading cancelled.\n";
        return;
      }
    } else if (choice == 2) {
      out << "Discarding current state...\n";

    } else if (choice == 3) {
      out << "<LOADING CANCELLED>\n";
      return;
    } else {
      out << "<INVALID COMMAND>: invalid choice\n";
      return;
    }
  }

  try {
    file_sys.start_state(name);
    out << "<STATE START COMPLETE>" << '\n';

  } catch (const std::exception& e) {
    out << e.what() << '\n';
  }
}

void zubarev::printPrompt(const FileSystem& fs, std::ostream& out)
{
  const std::string RESET = "\033[0m";
  const std::string GREEN = "\033[32m";
  const std::string BLUE = "\033[34m";
  const std::string YELLOW = "\033[33m";

  std::string user = detail::getCurrentUser();
  std::string path = fs.pwd();

  out << GREEN << user << RESET << ":" << BLUE << path << RESET << YELLOW << " $ " << RESET;
}

void zubarev::cmd_help(std::istream&, std::ostream& out, FileSystem&)
{
  const std::string RESET = "\033[0m";
  const std::string BOLD = "\033[1m";
  const std::string YELLOW = "\033[33m";
  const std::string GREEN = "\033[32m";
  const std::string CYAN = "\033[36m";
  const std::string MAGENTA = "\033[35m";

  out << "\n";
  out << BOLD << YELLOW << "╔════════════════"
      << "════════════════"
      << "══════════════════════════╗\n";
  out << "║                     FILE SYSTEM HELP                     ║\n";
  out << "╚════════════════"
      << "════════════════"
      << "══════════════════════════╝" << RESET << "\n\n";

  out << BOLD << GREEN << "  [FILES AND DIRECTORIES]" << RESET << "\n";
  out << CYAN << "  mkdir <dir-name>" << RESET << "           Create a new directory\n";
  out << CYAN << "  rmdir <dir-name>" << RESET << "           Remove an empty directory\n";
  out << CYAN << "  rm <file-name>" << RESET << "             Remove a file\n";
  out << CYAN << "  touch <file-name>" << RESET << "          Create an empty file\n";
  out << CYAN << "  write <file> \"<text>\"" << RESET << "      Write text to a file (replace content)\n";
  out << CYAN << "  cat <file-name>" << RESET << "            Display file content\n";
  out << CYAN << "  append <file> \"<text>\"" << RESET << "     Append text to the end of a file\n";
  out << CYAN << "  mv <from> <to>" << RESET << "             Move or rename a file/directory\n";
  out << CYAN << "  cp <from> <to>" << RESET << "             Copy a file/directory\n";
  out << "\n";

  out << BOLD << GREEN << "  [NAVIGATION]" << RESET << "\n";
  out << CYAN << "  cd <path>" << RESET << "                  Change current directory\n";
  out << CYAN << "  pwd" << RESET << "                        Print current working directory\n";
  out << CYAN << "  ls [path]" << RESET << "                  List directory contents\n";
  out << CYAN << "  tree [path]" << RESET << "                Display directory tree structure\n";
  out << "\n";

  out << BOLD << GREEN << "  [SEARCH]" << RESET << "\n";
  out << CYAN << "  search <name>" << RESET << "              Search files/dirs by exact name\n";
  out << "\n";

  out << BOLD << GREEN << "  [SESSIONS AND STATES]" << RESET << "\n";
  out << CYAN << "  save <name>" << RESET << "                Save object to in-memory session storage\n";
  out << CYAN << "  load <name>" << RESET << "                Load object from session storage\n";
  out << CYAN << "  states [path]" << RESET << "              List available .state files on disk\n";
  out << CYAN << "  save-state <file> [true]" << RESET << "   Save current FS to disk (.state file)\n";
  out << CYAN << "  start-state <file>" << RESET << "         Load FS state from disk (replaces current)\n";
  out << "\n";

  out << BOLD << GREEN << "  [IMPORT / EXPORT]" << RESET << "\n";
  out << CYAN << "  import <real-path> <virtual-name>" << RESET << "   Import file from real disk to VFS\n";
  out << CYAN << "  export <virtual-name> <real-path>" << RESET << "   Export file from VFS to real disk\n";
  out << CYAN << "  archive <dir-name>" << RESET << "                  Create compressed archive (TODO)\n";
  out << CYAN << "  extract <archive> [directory]" << RESET << "       Extract archive contents\n";
  out << "\n";

  out << BOLD << GREEN << "  [SYSTEM]" << RESET << "\n";
  out << CYAN << "  help" << RESET << "                      Show this help message\n";
  out << CYAN << "  test <file>" << RESET << "               Execute command script\n";
  out << CYAN << "  exit" << RESET << "                      Exit the file system\n";
  out << "\n";

  out << BOLD << MAGENTA << "  NOTES:" << RESET << "\n";
  out << "  • Use quotes for text with spaces: write file.txt \"Hello World\"\n";
  out << "  • Paths support ~ (root), . (current), .. (parent)\n";
  out << "  • Data is compressed with Huffman algorithm and deduplicated via "
         "SipHash+XXHash\n";
  out << "\n";
}
void zubarev::cmd_exit(std::istream&, std::ostream& out, FileSystem&)
{
  out << "\n";
  out << "\033[1;32m";
  out << "╔════════════════"
      << "════════════════"
      << "════════════════╗\n";
  out << "║             GOODBYE! SEE YOU SOON!             ║\n";
  out << "╚════════════════"
      << "════════════════"
      << "════════════════╝\n";
  out << "\033[0m";
  out << "\n";

  std::exit(0);
}

void zubarev::cmd_test(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  std::string filename;
  in >> filename;

  std::ifstream script_file(filename);
  if (!script_file.is_open()) {
    throw std::logic_error("Cannot open test script: " + filename);
  }

  out << "<STARTING SCRIPT: " << filename << ">\n";

  process_stream(script_file, out, file_sys);

  out << "<END OF SCRIPT: " << filename << ">\n";
}

void zubarev::process_stream(std::istream& in, std::ostream& out, FileSystem& file_sys)
{
  using Hash = SipHash;
  using Equal = Equaler< std::string >;
  using cmd_t = void (*)(std::istream&, std::ostream&, FileSystem&);

  static RobinHashTable< std::string, cmd_t, Hash, Equal > cmds(128);
  static bool initialized = false;

  if (!initialized) {
    cmds["mkdir"] = cmd_mkdir;
    cmds["rmdir"] = cmd_rmdir;
    cmds["rm"] = cmd_rm;
    cmds["touch"] = cmd_touch;
    cmds["write"] = cmd_write;
    cmds["append"] = cmd_append;
    cmds["cd"] = cmd_cd;
    cmds["mv"] = cmd_mv;
    cmds["cp"] = cmd_cp;
    cmds["cat"] = cmd_cat;
    cmds["pwd"] = cmd_pwd;
    cmds["ls"] = cmd_ls;
    cmds["tree"] = cmd_tree;
    cmds["search"] = cmd_search;
    cmds["save"] = cmd_save;
    cmds["load"] = cmd_load;
    cmds["states"] = cmd_states;
    cmds["import"] = cmd_import;
    cmds["export"] = cmd_export;
    cmds["save-state"] = cmd_save_state;
    cmds["start-state"] = cmd_start_state;
    cmds["archive"] = cmd_archive;
    cmds["extract"] = cmd_extract;
    cmds["help"] = cmd_help;
    cmds["exit"] = cmd_exit;
    cmds["test"] = cmd_test;
    initialized = true;
  }

  bool is_console = (&in == &std::cin);
  std::string line;

  if (is_console) {
    printPrompt(file_sys, out);
  }

  while (std::getline(in, line)) {

    if (line.empty()) {
      if (is_console)
        printPrompt(file_sys, out);
      continue;
    }

    if (!is_console) {
      printPrompt(file_sys, out);
      out << line << '\n';
    }

    std::istringstream iss(line);
    std::string cmd;
    if (!(iss >> cmd)) {
      if (is_console)
        printPrompt(file_sys, out);
      continue;
    }

    try {
      if (!cmds.has(cmd)) {
        throw std::out_of_range("unknown command");
      }
      cmds.at(cmd)(iss, out, file_sys);
    } catch (const std::out_of_range&) {
      out << "<INVALID COMMAND>\n";
    } catch (const std::logic_error& e) {
      out << "<INVALID COMMAND: " << e.what() << ">\n";
    }

    if (is_console) {
      printPrompt(file_sys, out);
    }
  }
}
