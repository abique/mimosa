#ifndef MIMOSA_URI_PARSE_QUERY_HH
# define MIMOSA_URI_PARSE_QUERY_HH

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

#endif /* !MIMOSA_URI_PARSE_QUERY_HH */
