#include "parse-query.hh"

namespace mimosa
{
  namespace uri
  {
    static void extractKeyValue(const char *&      in,
                                const char * const end,
                                container::kvs *   kvs)
    {
      const char * key     = in;
      const char * key_end = in;

      while (true)
      {
        if (in == end || *in == '&')
        {
          if (key < in)
            kvs->insert(std::make_pair(std::string(key, in - key),
                                       std::string()));
          return;
        }

        if (*in == '=')
        {
          key_end = in;
          break;
        }

        ++in;
      }

      const char * value     = ++in;

      while (true)
      {
        if (in == end || *in == '&')
        {
          if (key < key_end)
            kvs->insert(std::make_pair(std::string(key, key_end - key),
                                       std::string(value, in - value)));
          return;
        }

        ++in;
      }
    }

    void parseQuery(const char * const input,
                    uint32_t const     len,
                    container::kvs *   kvs)
    {
      const char *       in  = input;
      const char * const end = input + len;

      while (in < end)
      {
        if (*in == '&')
        {
          ++in;
          continue;
        }

        extractKeyValue(in, end, kvs);
      }
    }
  }
}
