#pragma once

#include <archive.h>

#include "../stream/stream.hh"
#include "../non-copyable.hh"

namespace mimosa
{
  namespace archive
  {
    class Reader : private NonCopyable
    {
    public:
      inline Reader() : archive_(archive_read_new()) {}
      inline ~Reader() : archive_(archive_read_free(archive_)) {}

      inline operator struct ::archive *() const { return archive_; }

      int open(stream::Stream::Ptr input);

    private:
      struct ::archive *archive_;
    };
  }
}
