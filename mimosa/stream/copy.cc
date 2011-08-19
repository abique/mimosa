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
                 const int64_t max_bytes,
                 runtime::Time timeout)
    {
      stream::Buffer::Ptr buffer       = new stream::Buffer;
      int64_t             copied_bytes = 0;
      while (true)
      {
        int64_t to_do = buffer->size();

        // compute the number of bytes to read/write
        if (max_bytes > 0 && max_bytes - copied_bytes < to_do)
          to_do = max_bytes;

        printf("pre bande\n");

        int64_t rbytes = input.read(buffer->data(), to_do, timeout);
        printf("bande: %lld, %s\n", rbytes, strerror(errno));
        if (rbytes <= 0)
          return copied_bytes;

        printf("bande\n");

        int64_t wbytes = 0;
        do {
          int64_t bytes = output.write(buffer->data() + wbytes, rbytes - wbytes, timeout);
          if (bytes < 0)
            return copied_bytes + wbytes;
          wbytes += bytes;
        } while (wbytes < rbytes);
        copied_bytes += wbytes;
      }
      return copied_bytes;
    }
  }
}
