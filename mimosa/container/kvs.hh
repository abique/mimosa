#ifndef MIMOSA_CONTAINER_KVS_HH
# define MIMOSA_CONTAINER_KVS_HH

# include <string>
# include <unordered_map>

namespace mimosa
{
  namespace container
  {
    typedef std::unordered_multimap<std::string, std::string> kvs;
  }
}

#endif /* !MIMOSA_CONTAINER_KVS_HH */
