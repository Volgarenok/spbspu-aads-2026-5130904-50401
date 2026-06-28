#include "file_system.hpp"

#include <stdexcept>
#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>

#include "utils.hpp"
#include <my_queue/queue.hpp>

bool zubarev::FileSystem::isEmpty() const
{
  return root_->children_.empty();
}
std::shared_ptr< zubarev::FSNode > zubarev::FileSystem::navigateTo(const std::string& path) const
{
  Queue< std::string > dirs = detail::resolvePath(path);

  if (dirs.empty()) {
    return curr_dir_;
  }

  std::shared_ptr< Directory > current = curr_dir_;

  if (path[0] == '/') {
    current = root_;
  }

  while (!dirs.empty()) {
    std::string name = dirs.top();
    dirs.drop();

    if (name == "." || name.empty()) {
      continue;
    }

    if (name == "..") {
      if (current->getParent()) {
        current = current->getParent();
      }
      continue;
    }

    auto it = current->children_.find(name);
    if (it == current->children_.end()) {
      return nullptr;
    }

    if (!it->val_->isDirectory() && !dirs.empty()) {
      return nullptr;
    }

    current = std::static_pointer_cast< Directory >(it->val_);
  }

  return current;
}

std::pair< std::shared_ptr< zubarev::Directory >, std::string >
zubarev::FileSystem::resolveParent(const std::string& path) const
{
  size_t pos = path.rfind('/');

  std::string parent_path;
  std::string name;

  if (pos == std::string::npos) {
    parent_path = "";
    name = path;
  } else {
    parent_path = path.substr(0, pos);
    name = path.substr(pos + 1);
  }

  std::shared_ptr< FSNode > parent = navigateTo(parent_path);

  if (!parent || !parent->isDirectory()) {
    throw std::runtime_error("cannot resolve parent directory");
  }

  return {std::static_pointer_cast< Directory >(parent), name};
}

zubarev::FileSystem::FileSystem()
{
  FileMetaData root_data;
  root_data.date = detail::getCurrentDateTime();
  root_data.owner = detail::getCurrentUser();
  Directory root_dir(root_data);
  root_ = std::make_shared< Directory >(root_dir);
  curr_dir_ = root_;
}
bool zubarev::FileSystem::mkdir(const std::string& path_dir)
{
  if (path_dir.empty()) {
    throw std::runtime_error("mkdir: missing operand");
  }
  if (path_dir == "." || path_dir == "..") {
    throw std::runtime_error("mkdir: cannot create directory '" + path_dir + "': Invalid argument");
  }

  std::pair< std::shared_ptr< Directory >, std::string > result = resolveParent(path_dir);

  std::shared_ptr< Directory > parent = result.first;
  std::string name = result.second;
  if (parent->children_.has(name)) {
    throw std::runtime_error("mkdir: cannot create directory '" + name + "': File exists");
  }
  FileMetaData new_data;
  new_data.date = detail::getCurrentDateTime();
  new_data.owner = detail::getCurrentUser();

  auto new_dir = std::make_shared< Directory >(new_data);
  parent->addChild(name, new_dir);
  return true;
}

bool zubarev::FileSystem::rmdir(const std::string& path_dir)
{
  if (path_dir.empty()) {
    throw std::runtime_error("rmdir: missing operand");
  }
  if (path_dir == "." || path_dir == "..") {
    throw std::runtime_error("rmdir: failed to remove '" + path_dir + "': Invalid argument");
  }

  std::pair< std::shared_ptr< Directory >, std::string > result = resolveParent(path_dir);

  std::shared_ptr< Directory > parent = result.first;
  std::string name = result.second;
  if (!parent->children_.has(name)) {
    throw std::runtime_error("rmdir: failed to remove '" + path_dir + "': No such file or directory");
  }

  std::shared_ptr< FSNode > node = parent->children_.at(name);
  if (!node->isDirectory()) {
    throw std::runtime_error("rmdir: failed to remove '" + path_dir + "': Not a directory");
  }

  auto dir = std::static_pointer_cast< Directory >(node);
  if (!dir->children_.empty()) {
    throw std::runtime_error("rmdir: failed to remove '" + path_dir + "': Directory not empty");
  }
  parent->removeChild(name);
  return true;
}

bool zubarev::FileSystem::rm(const std::string& file_path)
{
  if (file_path.empty()) {
    throw std::runtime_error("rm: missing operand");
  }
  if (file_path == "." || file_path == "..") {
    throw std::runtime_error("rm: cannot remove '" + file_path + "': Is a directory");
  }

  std::pair< std::shared_ptr< Directory >, std::string > result = resolveParent(file_path);

  std::shared_ptr< Directory > parent = result.first;
  std::string name = result.second;
  if (!parent->children_.has(name)) {
    throw std::runtime_error("rm: failed to remove '" + file_path + "': No such file or directory");
  }

  std::shared_ptr< FSNode > node = parent->children_.at(name);
  if (node->isDirectory()) {
    throw std::runtime_error("rm: cannot remove '" + file_path + "': Is a directory");
  }

  parent->removeChild(name);
  return true;
}

bool zubarev::FileSystem::touch(const std::string& file_path)
{
  if (file_path.empty()) {
    throw std::runtime_error("touch: missing file operand");
  }
  if (file_path == "." || file_path == "..") {
    throw std::runtime_error("touch: cannot touch '" + file_path + "': Invalid argument");
  }

  std::pair< std::shared_ptr< Directory >, std::string > result = resolveParent(file_path);

  std::shared_ptr< Directory > parent = result.first;
  std::string name = result.second;
  if (parent->children_.has(name)) {
    write(file_path, "");
  }

  FileMetaData new_data;
  new_data.date = detail::getCurrentDateTime();
  new_data.owner = detail::getCurrentUser();

  auto new_file = std::make_shared< File >(new_data);
  parent->addChild(name, new_file);
  return true;
}

bool zubarev::FileSystem::write(const std::string& file_path, const std::string& text)
{
  std::pair< std::shared_ptr< Directory >, std::string > result = resolveParent(file_path);
  std::shared_ptr< Directory > parent = result.first;
  std::string name = result.second;

  if (!parent->children_.has(name)) {
    touch(file_path);
  }

  auto node = parent->children_.find(name);
  if (node == parent->children_.end()) {
    throw std::runtime_error("write: file creation failed");
  }

  if (node->val_->isDirectory()) {
    throw std::runtime_error("write: Is a directory");
  }

  auto file = std::static_pointer_cast< File >(node->val_);
  writeToFile(*file, text, WriteMode::Overwrite);
  return true;
}

bool zubarev::FileSystem::append(const std::string& file_path, const std::string& text)
{
  if (text.empty()) {
    throw std::runtime_error("append: no data provided");
  }

  std::pair< std::shared_ptr< Directory >, std::string > result = resolveParent(file_path);

  std::shared_ptr< Directory > parent = result.first;
  std::string name = result.second;

  if (!parent->children_.has(name)) {
    touch(file_path);
  }

  auto node = parent->children_.find(name);

  if (node == parent->children_.end()) {
    throw std::runtime_error("append: file creation failed");
  }

  if (node->val_->isDirectory()) {
    throw std::runtime_error("append: Is a directory");
  }

  auto file = std::static_pointer_cast< File >(node->val_);
  writeToFile(*file, text, WriteMode::Append);
  return true;
}

void zubarev::FileSystem::writeToFile(File& file, const std::string& text, WriteMode mode)
{
  Vector< std::shared_ptr< DataBlock > > new_blocks;
  if (mode == WriteMode::Append) {
    new_blocks = file.getBlocks();
  }

  const size_t BLOCK_SIZE = 4096;
  size_t offset = 0;

  Vector< std::pair< BlockKey, std::shared_ptr< DataBlock > > > new_cache;
  while (offset < text.size()) {

    size_t cur_size = std::min(BLOCK_SIZE, text.size() - offset);
    std::string cur_str = text.substr(offset, cur_size);
    offset += cur_size;

    SipHash sip_hasher;
    XXHash xx_hasher;

    BlockKey key{sip_hasher(cur_str), xx_hasher(cur_str)};

    std::shared_ptr< DataBlock > block;

    if (data_block_.has(key)) {
      block = data_block_[key];
    } else {
      Huffman huffman;

      std::string bits = huffman.encode(cur_str);

      block = std::make_shared< DataBlock >();
      block->content_hash = key;
      block->original_size = cur_str.size();
      block->total_bits_count = bits.size();
      block->compressed_data = huffman.compress(bits);
      block->out_dictionary = std::make_shared< BSTree< char, std::string, Comparator< char > > >(huffman.getCodes());

      new_cache.pushBack({key, block});
    }
    new_blocks.pushBack(block);
  }
  for (auto it = new_cache.begin(); it != new_cache.end(); ++it) {
    data_block_.add(it->first, it->second);
  }
  file.replaceBlocks(new_blocks);
}

bool zubarev::FileSystem::cd(const std::string& path)
{
  if (path.empty()) {
    return true;
  }

  std::shared_ptr< FSNode > node = navigateTo(path);

  if (!node || !node->isDirectory()) {
    throw std::runtime_error("cd: '" + path + "': No such file or directory");
  }

  curr_dir_ = std::static_pointer_cast< Directory >(node);
  return true;
}

bool zubarev::FileSystem::isDescendant(const std::shared_ptr< Directory >& root,
                                       const std::shared_ptr< FSNode >& candidate) const
{
  if (!root) {
    return false;
  }

  std::shared_ptr< FSNode > current = candidate;

  while (current) {
    if (current == root) {
      return true;
    }
    current = current->getParent();
  }
  return false;
}

bool zubarev::FileSystem::mv(const std::string& from, const std::string& to)
{
  if (from.empty() || to.empty()) {
    throw std::runtime_error("mv: missing file operand");
  }

  if (from == "~" || to == "~") {
    throw std::runtime_error("mv: cannot move root directory");
  }

  if (from == "." || from == ".." || to == "." || to == "..") {
    throw std::runtime_error("mv: cannot move '" + from + "': Invalid argument");
  }
  std::shared_ptr< FSNode > src_from = navigateTo(from);
  std::shared_ptr< FSNode > src_to = navigateTo(to);

  if (!src_from || !src_from->getParent()) {
    throw std::runtime_error("mv: cannot stat '" + from + "': No such file or directory");
  }
  if (src_from->isDirectory() && isDescendant(std::static_pointer_cast< Directory >(src_from), src_to)) {
    throw std::runtime_error("mv: cannot move '" + from + "' to a subdirectory of itself '" + to + "'");
  }
  if (src_from == src_to) {
    throw std::runtime_error("mv: '" + from + "' and '" + to + "' are the same file");
  }

  std::shared_ptr< Directory > target_dir = nullptr;
  std::string target_name = "";

  if (src_to && src_to->isDirectory()) {
    target_dir = std::static_pointer_cast< Directory >(src_to);
    target_name = src_from->getName();
    if (target_dir->children_.has(target_name)) {
      throw std::runtime_error("mv: cannot move '" + from + "' to '" + to + "/" + target_name + "': File exists");
    }
  } else {

    std::pair< std::shared_ptr< zubarev::Directory >, std::string > parent_result = resolveParent(to);

    target_dir = parent_result.first;
    target_name = parent_result.second;

    if (src_to && !src_to->isDirectory()) {

      target_dir->removeChild(target_name);

    } else if (target_dir->children_.has(target_name)) {

      throw std::runtime_error("mv: cannot move '" + from + "' to '" + to + "': File exists");
    }
  }
  std::shared_ptr< Directory > old_parent = src_from->getParent();
  std::string saved_name = src_from->getName();
  old_parent->removeChild(saved_name);

  try {
    target_dir->addChild(target_name, src_from);
    src_from->setParent(target_dir);
  } catch (...) {
    old_parent->addChild(saved_name, src_from);
    src_from->setParent(old_parent);
    throw;
  }
  return true;
}

std::shared_ptr< zubarev::FSNode > zubarev::FileSystem::cloneFile(const std::shared_ptr< File >& old_file)
{
  auto new_file = std::make_shared< File >(old_file->getMeta());
  for (auto it = old_file->getBlocks().begin(); it != old_file->getBlocks().end(); ++it) {
    new_file->addBlock(*it);
  }
  return new_file;
}
std::shared_ptr< zubarev::FSNode > zubarev::FileSystem::cloneDirectory(const std::shared_ptr< Directory >& old_dir)
{
  auto new_dir = std::make_shared< Directory >(old_dir->getMeta());

  for (auto it = old_dir->children_.begin(); it != old_dir->children_.end(); ++it) {
    std::string child_name = it->key_;
    std::shared_ptr< FSNode > child_node = it->val_;

    if (child_node->isDirectory()) {
      auto sub_dir = std::static_pointer_cast< Directory >(child_node);
      new_dir->addChild(child_name, cloneDirectory(sub_dir));

    } else {
      auto sub_file = std::static_pointer_cast< File >(child_node);
      new_dir->addChild(child_name, cloneFile(sub_file));
    }
  }
  return new_dir;
}

bool zubarev::FileSystem::cp(const std::string& from, const std::string& to)
{
  if (from.empty() || to.empty()) {
    throw std::runtime_error("cp: missing operand");
  }

  auto src_from = navigateTo(from);
  auto src_to = navigateTo(to);

  if (!src_from || !src_from->getParent()) {
    throw std::runtime_error("cp: cannot stat '" + from + "'");
  }

  if (src_from == src_to) {
    throw std::runtime_error("cp: same file");
  }

  if (src_from->isDirectory() && src_to && isDescendant(std::static_pointer_cast< Directory >(src_from), src_to)) {
    throw std::runtime_error("cp: cannot copy directory into itself");
  }

  std::shared_ptr< Directory > target_dir;
  std::string target_name;

  if (src_to && src_to->isDirectory()) {
    target_dir = std::static_pointer_cast< Directory >(src_to);

    target_name = src_from->getName();
  } else {
    std::pair< std::shared_ptr< zubarev::Directory >, std::string > parent = resolveParent(to);

    target_dir = parent.first;
    target_name = parent.second;

    if (target_dir->children_.has(target_name)) {
      target_dir->removeChild(target_name);
    }
  }

  std::shared_ptr< FSNode > clone;

  if (src_from->isDirectory()) {
    clone = cloneDirectory(std::static_pointer_cast< Directory >(src_from));
  } else {
    clone = cloneFile(std::static_pointer_cast< File >(src_from));
  }

  target_dir->addChild(target_name, clone);

  return true;
}

std::string zubarev::FileSystem::cat(const std::string& name) const
{
  std::shared_ptr< FSNode > src = navigateTo(name);
  if (!src) {
    throw std::runtime_error("cat: '" + name + "': No such file or directory");
  } else if (src->isDirectory()) {
    throw std::runtime_error("cat: '" + name + "': Is a directory");
  }
  std::shared_ptr< File > file = std::static_pointer_cast< File >(src);
  std::string output_str = "";
  const Vector< std::shared_ptr< DataBlock > >& blocks = file->getBlocks();
  Huffman huffman;
  for (auto it = blocks.begin(); it != blocks.end(); ++it) {
    if (!(*it)) {
      throw std::runtime_error("cat: '" + name + "': corrupted block");
    }

    if (!(*it)->out_dictionary) {
      throw std::runtime_error("cat: '" + name + "': corrupted dictionary");
    }

    size_t max_bits = (*it)->compressed_data.getSize() * 8;
    if ((*it)->total_bits_count > max_bits) {
      throw std::runtime_error("cat: '" + name + "': corrupted block");
    }
    huffman.buildTreeFromDictionary(*(*it)->out_dictionary);
    std::string bit_string = huffman.decompress((*it)->compressed_data, (*it)->total_bits_count);
    output_str += huffman.decode(bit_string);
  }
  return output_str;
}

std::string zubarev::FileSystem::pwd() const
{
  if (curr_dir_ == root_) {
    return "~";
  }
  Vector< std::string > path_parts;
  std::shared_ptr< Directory > current = curr_dir_;
  while (current && current != root_) {
    path_parts.pushBack(current->getName());
    current = current->getParent();
  }
  std::reverse(path_parts.begin(), path_parts.end());
  std::string res = "~";
  for (auto it = path_parts.begin(); it != path_parts.end(); ++it) {
    res += '/' + *it;
  }
  return res;
}

zubarev::Vector< std::string > zubarev::FileSystem::ls(const std::string& path) const
{
  Vector< std::string > result;
  std::shared_ptr< Directory > target_dir = nullptr;

  if (path.empty()) {
    target_dir = curr_dir_;
  } else {
    std::shared_ptr< FSNode > search_node = navigateTo(path);
    if (!search_node) {
      return result;
    }
    if (search_node->isDirectory()) {
      target_dir = std::static_pointer_cast< Directory >(search_node);
    } else {
      result.pushBack(search_node->getName());
      return result;
    }
  }
  if (target_dir) {
    for (auto it = target_dir->children_.begin(); it != target_dir->children_.end(); ++it) {
      result.pushBack(it->key_);
    }
  }
  return result;
}

void zubarev::FileSystem::treeImpl(std::shared_ptr< Directory > dir,
                                   const std::string& prefix,
                                   std::string& tree_str,
                                   size_t& count_dir,
                                   size_t& count_files) const
{
  const std::string TRAIT = "──";
  const std::string COMPOUND = "├";
  const std::string END_COMPOUND = "└";

  Vector< std::pair< std::string, std::shared_ptr< FSNode > > > sorted_children;
  for (auto it = dir->children_.begin(); it != dir->children_.end(); ++it) {
    sorted_children.pushBack({it->key_, it->val_});
  }

  std::sort(
      sorted_children.begin(), sorted_children.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

  for (size_t i = 0; i < sorted_children.getSize(); ++i) {
    const std::pair< std::string, std::shared_ptr< FSNode > >& child = sorted_children[i];
    bool isLast = (i == sorted_children.getSize() - 1);

    std::string childPrefix = prefix;
    if (isLast) {
      childPrefix += "    ";
    } else {
      childPrefix += "│   ";
    }
    tree_str += prefix;
    tree_str += (isLast ? END_COMPOUND : COMPOUND);
    tree_str += TRAIT;
    tree_str += " ";
    tree_str += sorted_children[i].first;
    tree_str += '\n';

    if (child.second->isDirectory()) {
      ++count_dir;
      auto child_dir = std::static_pointer_cast< Directory >(child.second);
      treeImpl(child_dir, childPrefix, tree_str, count_dir, count_files);
    } else {
      count_files += 1;
    }
  }
}
std::tuple< std::string, size_t, size_t > zubarev::FileSystem::tree(const std::string& path) const
{
  size_t count_dir = 0;
  size_t count_files = 0;
  std::string res_str = "";
  res_str += ".\n";

  std::shared_ptr< Directory > target_dir = nullptr;
  if (path.empty()) {
    target_dir = curr_dir_;

  } else {
    std::shared_ptr< FSNode > found_target = navigateTo(path);
    if (!found_target) {
      throw std::runtime_error("tree: " + path + ": No such file or directory");
    }
    if (!found_target->isDirectory()) {
      throw std::runtime_error("tree: '" + path + "': Not a directory");
    }
    target_dir = std::static_pointer_cast< Directory >(found_target);
  }

  treeImpl(target_dir, "", res_str, count_dir, count_files);

  return {res_str, count_dir, count_files};
}

zubarev::Vector< std::string > getNodes(const std::string& name)
{
  zubarev::Vector< std::string > trigrams;
  if (name.size() > 3) {
    for (size_t i = 0; i <= name.size() - 3; ++i) {
      trigrams.pushBack(name.substr(i, 3));
    }
  } else {
    trigrams.pushBack(name);
  }
  return trigrams;
}
bool equal_nodes(const zubarev::Vector< std::string >& node_trigrams, const zubarev::Vector< std::string >& query_trigrams)
{
  if (query_trigrams.isEmpty()) {
    return false;
  }

  size_t matched = 0;
  for (size_t i = 0; i < query_trigrams.getSize(); ++i) {
    for (size_t j = 0; j < node_trigrams.getSize(); ++j) {
      if (query_trigrams[i] == node_trigrams[j]) {
        ++matched;
        break;
      }
    }
  }
  return matched * 2 >= query_trigrams.getSize();
}
void zubarev::FileSystem::search_impl(const std::shared_ptr< Directory > root,
                                      zubarev::Vector< std::shared_ptr< FSNode > >& results,
                                      const zubarev::Vector< std::string >& nodes) const
{
  for (auto it = root->children_.begin(); it != root->children_.end(); ++it) {
    if (equal_nodes(getNodes(it->key_), nodes)) {
      results.pushBack(it->val_);
    }
    if (it->val_->isDirectory()) {
      search_impl(std::dynamic_pointer_cast< Directory >(it->val_), results, nodes);
    }
  }
}

zubarev::Vector< std::shared_ptr< zubarev::FSNode > > zubarev::FileSystem::search(const std::string& name) const
{
  if (name.empty()) {
    throw std::runtime_error("search: Empty name");
  }

  Vector< std::string > nodes;
  if (name.size() > 3) {
    for (size_t i = 0; i <= name.size() - 3; ++i) {
      nodes.pushBack(name.substr(i, 3));
    }
  } else {
    nodes.pushBack(name);
  }

  Vector< std::shared_ptr< FSNode > > results;
  search_impl(root_, results, nodes);
  return results;
}

bool zubarev::FileSystem::save(const std::string& path)
{
  std::shared_ptr< FSNode > found_node = navigateTo(path);
  if (!found_node) {
    throw std::runtime_error("save: '" + path + "': No such file or directory");
  }

  const std::string& save_name = found_node->getName();
  if (session_storage_.find(save_name) != session_storage_.end()) {
    throw std::runtime_error("save: '" + path + "': already exists in storage");
  }

  if (found_node->isDirectory()) {
    session_storage_.add(save_name, cloneDirectory(std::static_pointer_cast< Directory >(found_node)));
  } else {
    session_storage_.add(save_name, cloneFile(std::static_pointer_cast< File >(found_node)));
  }

  return true;
}
bool zubarev::FileSystem::load(const std::string& path)
{
  std::shared_ptr< FSNode > found_node = navigateTo(path);
  if (!found_node) {
    throw std::runtime_error("load: '" + path + "': No such file or directory");
  }

  const std::string& save_name = found_node->getName();
  auto save_node = session_storage_.find(save_name);
  if (save_node == session_storage_.end()) {
    throw std::runtime_error("load: '" + path + "': no such state saved");
  }

  std::shared_ptr< FSNode > new_node = save_node->val_;
  if (new_node->isDirectory()) {
    curr_dir_->addChild(save_name, cloneDirectory(std::static_pointer_cast< Directory >(new_node)));
  } else {
    curr_dir_->addChild(save_name, cloneFile(std::static_pointer_cast< File >(new_node)));
  }

  return true;
}

zubarev::Vector< zubarev::FileSystem::StateInfo > zubarev::FileSystem::states(const std::string& path) const
{
  Vector< StateInfo > result;

  DIR* dir = opendir(path.c_str());
  if (!dir) {
    std::cout << "here" << '\n';
    return result;
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string filename = entry->d_name;

    if (filename == "." || filename == "..") {
      continue;
    }

    if (filename.size() < 6) {
      continue;
    }

    std::string extension = filename.substr(filename.size() - 6);
    if (extension != ".state") {
      continue;
    }

    std::string full_path = path;
    if (!full_path.empty() && full_path.back() != '/') {
      full_path += '/';
    }
    full_path += filename;

    struct stat file_stat;
    if (stat(full_path.c_str(), &file_stat) == 0) {
      if (S_ISREG(file_stat.st_mode)) {
        StateInfo info;
        info.name = filename;
        info.size_kb = file_stat.st_size / 1024;

        struct tm* tm_info = localtime(&file_stat.st_mtime);
        char time_buf[20];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d", tm_info);
        info.date = time_buf;
        result.pushBack(info);
      }
    }
  }
  closedir(dir);
  return result;
}

bool zubarev::FileSystem::import_file(const std::string& real_path, const std::string& virtual_name)
{
  std::ifstream input(real_path, std::ios::binary);

  if (!input) {
    throw std::runtime_error("import: cannot open '" + real_path + "': No such file or directory");
  }
  std::string new_file_name = "";
  if (virtual_name.empty()) {
    size_t pos = real_path.find_last_of("/\\");
    if (pos != std::string::npos) {
      new_file_name = real_path.substr(pos + 1);
    } else {
      new_file_name = real_path;
    }
  } else {
    new_file_name = virtual_name;
  }
  FileMetaData new_data;
  new_data.date = detail::getCurrentDateTime();
  new_data.owner = detail::getCurrentUser();

  auto new_file = std::make_shared< File >(new_data);
  std::ostringstream buffer;
  buffer << input.rdbuf();
  curr_dir_->addChild(new_file_name, new_file);

  write(new_file_name, buffer.str());

  return true;
}
bool zubarev::FileSystem::export_file(const std::string& virtual_name, const std::string& real_path)
{

  if (virtual_name.empty() || !navigateTo(virtual_name)) {
    throw std::runtime_error("export: cannot open '" + virtual_name + "': No such file or directory");
  }

  std::ofstream output_file(real_path);
  if (!output_file) {
    throw std::runtime_error("export: cannot create '" + real_path + "': Permission denied");
  }
  output_file << cat(virtual_name);
  output_file.close();
  return true;
}

void zubarev::FileSystem::traverse_dir(std::ostream& out, std::shared_ptr< Directory >* dir)
{
  for (auto it = (*dir)->children_.begin(); it != (*dir)->children_.end(); ++it) {
    std::string name = it->key_;
    std::shared_ptr< FSNode > node = it->val_;

    if (node->isDirectory()) {
      out << "mkdir " << name << '\n';
      out << "cd " << name << '\n';
      auto sub_dir = std::static_pointer_cast< Directory >(node);
      std::shared_ptr< Directory > old_curr = curr_dir_;
      curr_dir_ = sub_dir;
      traverse_dir(out, &sub_dir);
      curr_dir_ = old_curr;
      out << "cd .." << '\n';
    } else {
      out << "touch " << name << '\n';

      std::string content = this->cat(name);
      out << "write " << name << " " << content.size() << "\n";
      out << content;
      out << "\nend_write\n";
    }
  }
}

bool zubarev::FileSystem::save_state(const std::string& state_name, bool rewrite)
{
  const std::string dir_path = "zubarev.arsenii/F0/states";
  system(("mkdir -p " + dir_path).c_str());
  if (root_->children_.empty()) {
    throw std::runtime_error("save-state: file system is empty, nothing to save");
  }
  const std::string state_path = "zubarev.arsenii/F0/states/" + state_name;
  size_t dot_pos = state_name.rfind('.');
  if (dot_pos == std::string::npos || state_name.substr(dot_pos) != ".state") {
    throw std::runtime_error("save-state: '" + state_name + "': invalid extension");
  }

  std::ifstream input(state_path, std::ios::binary);
  if (input && !rewrite) {
    throw std::runtime_error("save-state: '" + state_name + "': File exists");
  }
  input.close();

  std::ofstream output_file(state_path);
  if (input && !rewrite) {
    throw std::runtime_error("save-state: '" + state_name + "': File exists");
  }
  std::shared_ptr< Directory > old_curr_dir = curr_dir_;
  curr_dir_ = root_;
  traverse_dir(output_file, &root_);
  curr_dir_ = old_curr_dir;
  output_file.close();
  return true;
}

bool zubarev::FileSystem::start_state(const std::string& state_name)
{
  const std::string state_path = "zubarev.arsenii/F0/states/" + state_name;

  std::ifstream input(state_path, std::ios::binary);

  if (!input) {
    throw std::runtime_error("start-state: cannot open '" + state_path + "': No such file or directory");
  }
  FileMetaData root_data;
  root_data.date = detail::getCurrentDateTime();
  root_data.owner = detail::getCurrentUser();

  auto new_root = std::make_shared< Directory >(root_data);
  auto new_curr = new_root;

  std::string line;
  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    if (command == "mkdir") {
      std::string name;
      iss >> name;
      this->mkdir(name);
    } else if (command == "touch") {
      std::string name;
      iss >> name;
      this->touch(name);
    } else if (command == "cd") {
      std::string path;
      iss >> path;
      this->cd(path);
    } else if (command == "write") {
      std::string name;
      size_t content_size;
      iss >> name >> content_size;

      std::string content(content_size, '\0');
      input.read(&content[0], content_size);

      char dummy;
      input.get(dummy);

      std::string end_marker;
      std::getline(input, end_marker);
      if (end_marker != "end_write") {
        throw std::runtime_error("start-state: corrupted state file");
      }

      this->write(name, content);
    }
  }
  input.close();
  root_ = new_root;
  curr_dir_ = new_curr;
  this->cd("~");
  return true;
}
void zubarev::FileSystem::packUint32(uint32_t value, std::string& out)
{
  out.push_back((value >> 24) & 0xFF);
  out.push_back((value >> 16) & 0xFF);
  out.push_back((value >> 8) & 0xFF);
  out.push_back(value & 0xFF);
}
void zubarev::FileSystem::archiveImpl(const std::string& base_path,
                                      std::shared_ptr< Directory > dir,
                                      std::string& archive_data)
{
  for (auto it = dir->children_.begin(); it != dir->children_.end(); ++it) {
    std::string child_name = it->key_;
    std::string full_virt_path_ = base_path.empty() ? child_name : base_path + "/" + child_name;
    std::shared_ptr< FSNode > node = it->val_;
    if (node->isDirectory()) {
      auto sub_dir = std::static_pointer_cast< Directory >(node);
      archiveImpl(full_virt_path_, sub_dir, archive_data);
    } else {
      auto file = std::static_pointer_cast< File >(node);
      std::string content = "";

      const Vector< std::shared_ptr< DataBlock > >& blocks = file->getBlocks();
      Huffman huffman;
      for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        huffman.buildTreeFromDictionary(*(*it)->out_dictionary);
        std::string bit_string = huffman.decompress((*it)->compressed_data, (*it)->total_bits_count);
        content += huffman.decode(bit_string);
      }

      packUint32(full_virt_path_.size(), archive_data);
      archive_data += full_virt_path_;

      packUint32(content.size(), archive_data);
      archive_data += content;
    }
  }
}

bool zubarev::FileSystem::archive(const std::string& archive_name, const std::string& dir_path)
{
  if (archive_name.empty() || dir_path.empty()) {
    throw std::runtime_error("archive: missing operand");
  }

  std::shared_ptr< FSNode > existing = navigateTo(archive_name);
  if (existing) {
    throw std::runtime_error("archive: cannot create '" + archive_name + "': File exists");
  }

  std::shared_ptr< FSNode > target_node = navigateTo(dir_path);

  if (!target_node || !target_node->isDirectory()) {
    throw std::runtime_error("archive: cannot access '" + dir_path + "': Not a directory");
  }
  auto target_dir = std::static_pointer_cast< Directory >(target_node);
  size_t last_slash = dir_path.rfind('/');
  std::string folder_name = (last_slash == std::string::npos) ? dir_path : dir_path.substr(last_slash + 1);

  std::string archived_data = "";

  archiveImpl(folder_name, target_dir, archived_data);

  touch(archive_name);
  write(archive_name, archived_data);
  return true;
}
uint32_t zubarev::FileSystem::unpackUint32(const std::string& data, size_t& offset)
{
  uint32_t value = 0;
  value = (value << 8) | static_cast< uint8_t >(data[offset++]);
  value = (value << 8) | static_cast< uint8_t >(data[offset++]);
  value = (value << 8) | static_cast< uint8_t >(data[offset++]);
  value = (value << 8) | static_cast< uint8_t >(data[offset++]);
  return value;
}

void zubarev::FileSystem::ensurePathExists(const std::string& path)
{
  std::string current_path_ = "";
  std::string part;
  std::istringstream iss(path);

  while (std::getline(iss, part, '/')) {
    if (part.empty()) {
      continue;
    }
    if (!curr_dir_->children_.has(part)) {
      mkdir(part);
    }
    cd(part);
  }
}

bool zubarev::FileSystem::extract(const std::string& archive_path, const std::string& dir_path_after)
{
  std::shared_ptr< FSNode > archive_node = navigateTo(archive_path);
  if (!archive_node || archive_node->isDirectory()) {
    throw std::runtime_error("extract: invalid archive");
  }

  std::string data = cat(archive_path);

  if (!dir_path_after.empty()) {
    cd(dir_path_after);
  }

  std::shared_ptr< Directory > extract_root_node = curr_dir_;

  size_t offset = 0;
  while (offset < data.size()) {
    if (offset + 4 > data.size())
      break;
    uint32_t name_len = unpackUint32(data, offset);
    if (offset + name_len > data.size())
      break;

    std::string file_path = data.substr(offset, name_len);
    offset += name_len;

    if (offset + 4 > data.size())
      break;
    uint32_t size = unpackUint32(data, offset);
    if (offset + size > data.size())
      break;

    std::string content = data.substr(offset, size);
    offset += size;

    size_t pos = file_path.rfind('/');
    std::string dir;
    std::string file;

    curr_dir_ = extract_root_node;

    if (pos != std::string::npos) {
      dir = file_path.substr(0, pos);
      file = file_path.substr(pos + 1);
      ensurePathExists(dir);
    } else {
      file = file_path;
    }

    touch(file);
    write(file, content);
  }

  curr_dir_ = extract_root_node;
  return true;
}
