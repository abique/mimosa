#ifndef MIMOSA_STREAM_COPY_HH
# define MIMOSA_STREAM_COPY_HH

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    int64_t copy(Stream &      input,
                 Stream &      output,
                 int64_t       max_bytes = 0);
  }
}

#endif /* !MIMOSA_STREAM_COPY_HH */
