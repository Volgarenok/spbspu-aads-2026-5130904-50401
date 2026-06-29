#include <cstddef>
#include <string>
#include <myvector.hpp>
#include "my_hash_table.hpp"

namespace goltsov
{
  namespace detail
  {
    void sortStringVector(goltsov::Vector< std::string >&);
    void sortSizeTVector(goltsov::Vector< size_t >&);
    void sortPairStringSizeTVector(goltsov::Vector< std::pair< std::string, goltsov::Vector< size_t > > >&);
  }

  using pairs_vertexes_t = HashTable< std::pair< std::string, std::string >, Vector< size_t >,
      Sha1Hasher< std::pair< std::string, std::string > >, std::equal_to< std::pair< std::string, std::string > > >;

  using graphs_t = HashTable< std::string, pairs_vertexes_t, Sha1Hasher< std::string >,
    std::equal_to< std::string > >;

  using graphs_iter_t = HashTableIterator< std::string, pairs_vertexes_t, Sha1Hasher< std::string >,
    std::equal_to< std::string > >;

  using pairs_vertexes_iter_t = HashTableIterator< std::pair< std::string, std::string >, Vector< size_t >,
      Sha1Hasher< std::pair< std::string, std::string > >, std::equal_to< std::pair< std::string, std::string > > >;

  using ht_graphs_functions = goltsov::HashTable< std::string, void (*)(graphs_t&, std::istream&),
    goltsov::Sha1Hasher< std::string >, std::equal_to< std::string > >;

  void graphsParsing(graphs_t&, std::istream&);
  void vertexesParsing(graphs_t&, std::istream&);
  void outboundParsing(graphs_t&, std::istream&);
  void inboundParsing(graphs_t&, std::istream&);
  void bindParsing(graphs_t&, std::istream&);
  void cutParsing(graphs_t&, std::istream&);
  void createParsing(graphs_t&, std::istream&);
  void mergeParsing(graphs_t&, std::istream&);
  void extractParsing(graphs_t&, std::istream&);

  void read_graphs(std::istream&, graphs_t&);
  void graphs(graphs_t&);
  void vertexes(graphs_t&, std::string);
  void outbound(graphs_t&, std::string, std::string);
  void inbound(graphs_t&, std::string, std::string);
  void bind(graphs_t&, std::string,
    std::string, std::string, size_t);
  void cut(graphs_t&, std::string,
    std::string, std::string, size_t);
  void create(graphs_t&, std::string,
    size_t, goltsov::Vector< std::string >);
  void merge(graphs_t&, std::string,
    std::string, std::string);
  void extract(graphs_t&, std::string, std::string,
    size_t, goltsov::Vector< std::string >);
}
