#ifndef MY_BLOCKKEY_HPP
#define MY_BLOCKKEY_HPP

#include <my_hashfunc/my_siphash.hpp>
#include "my_xxhash.hpp"

namespace zubarev
{
  struct BlockKey
  {
    uint64_t sip_hash;
    uint64_t xx_hash;

    bool operator==(const BlockKey& other) const
    {
      return sip_hash == other.sip_hash && xx_hash == other.xx_hash;
    }
  };
  struct BlockKeyHash
  {
    size_t operator()(const BlockKey& key) const
    {
      size_t h1 = std::hash< uint64_t >{}(key.sip_hash);
      size_t h2 = std::hash< uint64_t >{}(key.xx_hash) << 1;
      return h1 ^ h2;
    }
  };
  struct BlockKeyEqual
  {
    bool operator()(const BlockKey& a, const BlockKey& b) const
    {
      return a == b;
    }
  };

}

#endif
