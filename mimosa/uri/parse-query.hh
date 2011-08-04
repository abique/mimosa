#ifndef MIMOSA_URI_PARSE_QUERY_HH
# define MIMOSA_URI_PARSE_QUERY_HH

# include <string>
# include <unordered_map>

# include "../string/string-ref.hh"

namespace mimosa
{
  namespace uri
  {
    void parseQuery(const string::StringRef &                           query,
                    std::unordered_multimap<std::string, std::string> * kvs);
  }
}

#endif /* !MIMOSA_URI_PARSE_QUERY_HH */
