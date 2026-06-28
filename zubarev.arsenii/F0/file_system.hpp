#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <string>
#include <memory>

#include "robin_hashtable.hpp"
#include "fs_nodes.hpp"
#include "blockkey.hpp"

namespace zubarev
{

  class FileSystem
  {
  public:
    FileSystem();
    ~FileSystem() = default;
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;
    bool isEmpty() const;

    bool mkdir(const std::string& name_dir);
    bool rmdir(const std::string& name_dir);
    bool rm(const std::string& name);
    bool touch(const std::string& name_file);
    bool write(const std::string& name, const std::string& text);
    bool append(const std::string& name, const std::string& text);
    bool cd(const std::string& path);
    bool mv(const std::string& from, const std::string& to);
    bool cp(const std::string& from, const std::string& to);

    std::string cat(const std::string& name) const;
    std::string pwd() const;
    Vector< std::string > ls(const std::string& path) const;
    std::tuple< std::string, size_t, size_t > tree(const std::string& path) const;
    Vector< std::shared_ptr< FSNode > > search(const std::string& name) const;

    bool save(const std::string& name_node);
    bool load(const std::string& name_node);
    struct StateInfo
    {
      std::string name;
      size_t size_kb;
      std::string date;
    };
    Vector< StateInfo > states(const std::string& path = ".") const;
    bool import_file(const std::string& real_path, const std::string& virtual_name);
    bool export_file(const std::string& virtual_name, const std::string& real_path);

    void traverse_dir(std::ostream& out, std::shared_ptr< Directory >* dir);
    bool save_state(const std::string& state_name, bool rewrite);
    bool start_state(const std::string& state_name);

    bool archive(const std::string& archive_name, const std::string& dir_path);
    bool extract(const std::string& archive_path, const std::string& dir_path);

  private:
    enum class WriteMode { Overwrite, Append };
    void writeToFile(File& file, const std::string& text, WriteMode mode);

    bool isDescendant(const std::shared_ptr< Directory >& root, const std::shared_ptr< FSNode >& candidate) const;
    std::shared_ptr< FSNode > cloneFile(const std::shared_ptr< File >& old_file);
    std::shared_ptr< FSNode > cloneDirectory(const std::shared_ptr< Directory >& old_directory);

    std::shared_ptr< FSNode > navigateTo(const std::string& path) const;
    std::pair< std::shared_ptr< Directory >, std::string > resolveParent(const std::string& path) const;

    void treeImpl(std::shared_ptr< Directory >, const std::string&, std::string&, size_t&, size_t&) const;
    void packUint32(uint32_t value, std::string& out);
    void archiveImpl(const std::string& base_path, std::shared_ptr< Directory > dir, std::string& archive_data);
    void ensurePathExists(const std::string& path);
    uint32_t unpackUint32(const std::string& data, size_t& offset);

    void search_impl(const std::shared_ptr< Directory > root,
                     Vector< std::shared_ptr< FSNode > >& results,
                     const Vector< std::string >& nodes) const;

    std::shared_ptr< Directory > root_;
    std::shared_ptr< Directory > curr_dir_;

    RobinHashTable< BlockKey, std::shared_ptr< DataBlock >, BlockKeyHash, BlockKeyEqual > data_block_;
    RobinHashTable< std::string, std::shared_ptr< FSNode >, SipHash, Equaler< std::string > > session_storage_;
  };

}

#endif
