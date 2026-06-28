#ifndef MY_SIPHASH_HPP
#define MY_SIPHASH_HPP

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstdint>
#include <string>

namespace zubarev
{
  class SipHash
  {
  private:
    boost::hash2::siphash_64 hasher_template_;

  public:
    SipHash(uint64_t k0 = 0x0706050403020100ULL, uint64_t k1 = 0x0F0E0D0C0B0A0908ULL)
    {
      uint64_t keys[2] = {k0, k1};
      hasher_template_ = boost::hash2::siphash_64(keys, sizeof(keys));
    }

    uint64_t
    operator()(const void* data, size_t len) const
    {
      boost::hash2::siphash_64 h = hasher_template_;
      h.update(data, len);
      return boost::hash2::get_integral_result< uint64_t >(h);
    }

    uint64_t
    operator()(const std::string& s) const
    {
      return (*this)(s.data(), s.size());
    }

    uint64_t
    operator()(const std::pair< std::string, std::string >& p) const
    {
      boost::hash2::siphash_64 h = hasher_template_;

      size_t s1 = p.first.size();
      size_t s2 = p.second.size();

      h.update(&s1, sizeof(s1));
      h.update(p.first.data(), p.first.size());

      h.update(&s2, sizeof(s2));
      h.update(p.second.data(), p.second.size());

      return boost::hash2::get_integral_result< uint64_t >(h);
    }
    uint64_t
    operator()(uint64_t x) const
    {
      return (*this)(&x, sizeof(x));
    }
  };
}

#endif
