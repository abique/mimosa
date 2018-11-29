#include <cassert>
#include <algorithm>

#include "../string-ref.hh"
#include "chunked-stream.hh"
#include "../format/print.hh"

namespace mimosa
{
  namespace http
  {
    ChunkedStream::ChunkedStream(const stream::BufferedStream::Ptr& stream)
      : Filter(stream.get()),
        stream_(stream),
        bytes_left_(0),
        first_chunk_(true),
        eof_(false)
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
      if (eof_)
        return 0;

      if (bytes_left_ == 0) {
        if (!readChunkSize())
          return -1;
        return read(data, nbytes);
      }

      auto can_read = std::min(nbytes, bytes_left_);
      auto rbytes = stream_->read(data, can_read);
      if (rbytes > 0)
        bytes_left_ -= rbytes;
      return rbytes;
    }

    bool
    ChunkedStream::readChunkSize()
    {
      bytes_left_ = 0;

      if (!first_chunk_) {
        char line_break[2];
        if (stream_->loopRead(line_break, 2) != 2 ||
            line_break[0] != '\r' || line_break[1] != '\n') {
          eof_ = true;
          return false;
        }
      }

      bool found = false;
      stream::Buffer::Ptr buffer = stream_->readUntil("\r\n", 15, &found);

      if (!found) {
        eof_ = true;
        return false;
      }

      StringRef str(buffer->data(), buffer->size());
      bytes_left_ = str.atoiHex<int64_t>();
      if (bytes_left_ == 0)
        eof_ = true;
      first_chunk_ = false;
      return true;
    }
  }
}
