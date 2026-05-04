#ifndef HASH_FUNC
#define HASH_FUNC
#include <boost/hash2/hmac.hpp>
#include <boost/hash2/hash_append.hpp>
#include <cstddef>
#include <string>
namespace malashenko
{
  class Hash {
  public:
    size_t operator()();
  };
}


#endif