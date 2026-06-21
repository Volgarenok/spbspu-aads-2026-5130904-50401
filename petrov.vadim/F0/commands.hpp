#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "kuku_hash_table.hpp"
#include "train_structs.hpp"
#include "hash_func.hpp"

namespace petrov
{
  struct FuncManage
  {
    void mk_user(std::ostream& out, std::istream& in, const std::string& str);
    void rm_user(std::ostream& out, std::istream& in, const std::string& str);
    void update_user(std::ostream& out, std::istream& in, const std::string& str);
    void show_users(std::ostream& out, std::istream& in, const std::string& str);

    void save(std::ostream& out, std::istream& in, const std::string& str);
    void load(std::ostream& out, std::istream& in, const std::string& str);

    void train(std::ostream& out, std::istream& in, const std::string& str);

    void mk_ex(std::ostream& out, std::istream& in, const std::string& str);
    void rm_ex(std::ostream& out, std::istream& in, const std::string& str);
    void show_ex(std::ostream& out, std::istream& in, const std::string& str);

    void add_ex_pool(std::ostream& out, std::istream& in, const std::string& str);
    void rm_pool(std::ostream& out, std::istream& in, const std::string& str);
    void show_pools(std::ostream& out, std::istream& in, const std::string& str);

    private:
      using trainPool_t = KukuHashTable< Exercise, std::string, KKHash< std::string >, Equal< std::string > >;

      trainPool_t exercisesPool_;
      KukuHashTable< User, std::string, KKHash< std::string >, Equal< std::string > > users_;
      KukuHashTable< trainPool_t, std::string, KKHash< std::string >, Equal< std::string > > pools_;
  };
}

#endif
