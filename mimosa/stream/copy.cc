#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "buffer.hh"
#include "copy.hh"

namespace mimosa
{
  namespace stream
  {
    int64_t copy(Stream &      input,
                 Stream &      output,
                 const int64_t max_bytes)
    {
      stream::Buffer::Ptr buffer       = new stream::Buffer;
      int64_t             copied_bytes = 0;
      while (true)
      {
        int64_t to_do = buffer->size();

        // compute the number of bytes to read/write
        if (max_bytes > 0 && max_bytes - copied_bytes < to_do)
          to_do = max_bytes - copied_bytes;

        int64_t rbytes = input.read(buffer->data(), to_do);
        if (rbytes <= 0)
          return copied_bytes;

        int64_t wbytes = output.loopWrite(buffer->data(), rbytes);
        if (wbytes != rbytes)
          return -1;

        copied_bytes += wbytes;
      }
      return copied_bytes;
    }
  }
}
