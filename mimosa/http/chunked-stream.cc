#include <cassert>

#include "chunked-stream.hh"
#include "../format/print.hh"

namespace mimosa
{
  namespace http
  {
    ChunkedStream::ChunkedStream(stream::Stream::Ptr stream)
      : Filter(stream)
    {
    }

    int64_t
    ChunkedStream::write(const char * data, uint64_t nbytes)
    {
      bool ok = true;

      ok = ok & format::printHex(*stream_, nbytes);
      ok = ok & format::printStatic(*stream_, "\r\n");
      ok = ok & format::print(*stream_, data, nbytes);
      ok = ok & format::printStatic(*stream_, "\r\n");
      return ok ? nbytes : -1;
    }

    int64_t
    ChunkedStream::read(char * data, uint64_t nbytes)
    {
      assert(false);
      return -1;
    }
  }
}
