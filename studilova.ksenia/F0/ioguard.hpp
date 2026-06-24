#ifndef IOGUARD_HPP
#define IOGUARD_HPP

#include <ios>

namespace studilova
{
  class IOGuard
  {
    public:
      explicit IOGuard(std::basic_ios< char >& stream);
      ~IOGuard();

    private:
      std::basic_ios< char >& stream_;
      std::streamsize width_;
      std::streamsize precision_;
      std::basic_ios< char >::fmtflags flags_;
      char fill_;
  };
}

studilova::IOGuard::IOGuard(std::basic_ios< char >& stream):
  stream_(stream),
  width_(stream.width()),
  precision_(stream.precision()),
  flags_(stream.flags()),
  fill_(stream.fill())
{}

studilova::IOGuard::~IOGuard()
{
  stream_.width(width_);
  stream_.precision(precision_);
  stream_.flags(flags_);
  stream_.fill(fill_);
}

#endif
