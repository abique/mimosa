#include "print.hh"

namespace mimosa
{
  namespace format
  {
    bool printHex(stream::Stream & stream, uint64_t value, Time timeout)
    {
      // 2 ** 64 - 1 = ffffffffffffffff
      char buffer[16];
      char *it = buffer + sizeof (buffer);

      if (value == 0)
        return stream.loopWrite("0", 1, timeout) == 1;

      while (value > 0)
      {
        --it;
        *it = "0123456789abcdef"[value & 0xf];
        value >>= 4;
      }

      const int64_t len = sizeof (buffer) - (it - buffer);
      return stream.loopWrite(it, len, timeout) == len;
    }
  }
}
