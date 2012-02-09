#include "percent-encoding.hh"
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
          {
            std::string key2;
            percentDecode(key, in - key, &key2, uri::kRfc2396);
            kvs->insert(std::make_pair(key2, std::string()));
          }
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
          {
            std::string key2;
            std::string value2;
            percentDecode(key, key_end - key, &key2, uri::kRfc2396);
            percentDecode(value, in - value, &value2, uri::kRfc2396);
            kvs->insert(std::make_pair(key2, value2));
          }
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
