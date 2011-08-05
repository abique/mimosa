#ifndef MIMOSA_URI_PARSE_QUERY_HH
# define MIMOSA_URI_PARSE_QUERY_HH

# include <string>
# include <unordered_map>

namespace mimosa
{
  namespace uri
  {
    void parseQuery(const char *                                        input,
                    uint32_t                                            len,
                    std::unordered_multimap<std::string, std::string> * kvs);
  }
}

#endif /* !MIMOSA_URI_PARSE_QUERY_HH */
