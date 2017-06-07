#pragma once

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

