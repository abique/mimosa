#include <cassert>
#include <cstring>

namespace mimosa
{
  namespace format
  {
    template <typename T>
    inline bool printDecimal(stream::Stream & stream, T value)
    {
      // 2 ** 64 = 18446744073709551616
      char buffer[32];
      bool neg = false;
      char *it = buffer + sizeof (buffer);

      if (value < 0)
      {
        neg = true;
        value = -value;
      }

      if (value == 0)
        return stream.loopWrite("0", 1) == 1;

      while (value > 0)
      {
        --it;
        *it = '0' + (value % 10);
        value /= 10;
      }

      if (neg)
      {
        --it;
        *it = '-';
      }

      const int64_t len = sizeof (buffer) - (it - buffer);
      return stream.loopWrite(it, len) == len;
    }

    inline bool print(stream::Stream & stream, const std::string & str)
    {
      return stream.loopWrite(str.data(), str.size()) == (int64_t)str.size();
    }

    inline bool print(stream::Stream & stream, const char *str, size_t len)
    {
      return stream.loopWrite(str, len) == (int64_t)len;
    }

    template <size_t len>
    inline bool printStatic(stream::Stream & stream, const char (&str)[len])
    {
      return print(stream, str, len - 1);
    }

    inline bool print(stream::Stream & stream, const char *str)
    {
      return print(stream, str, ::strlen(str));
    }
  }
}
