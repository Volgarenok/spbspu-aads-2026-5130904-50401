#ifndef FUNC_MANAGE_HPP
#define FUNC_MANAGE_HPP

#include <iostream>
#include "Grath.hpp"

namespace petrov
{
  struct FuncManage
  {
    void graphs(std::ostream& out, std::istream& in, std::string str);
    void vertexes(std::ostream& out, std::istream& in, std::string str);
    void outbound(std::ostream& out, std::istream& in, std::string str);
    void inbound(std::ostream& out, std::istream& in, std::string str);

    void bind(std::ostream& out, std::istream& in, std::string str);
    void cut(std::ostream& out, std::istream& in, std::string str);

    void create(std::ostream& out, std::istream& in, std::string str);
    void merge(std::ostream& out, std::istream& in, std::string str);
    void extract(std::ostream& out, std::istream& in, std::string str);

    void readFile(std::istream & in);
  private:
    HashTable< std::string, Grath, sha1, Equal< std::string > > graphs_;
  };
}

#endif
