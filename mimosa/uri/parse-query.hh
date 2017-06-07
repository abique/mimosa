#pragma once

# include <string>
# include <unordered_map>

# include "../kvs.hh"

namespace mimosa
{
  namespace uri
  {
    /** @warning parseQuery does not url decode */
    void parseQuery(const char *     input,
                    uint32_t         len,
                    kvs * kvs);
  }
}

