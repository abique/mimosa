#include "format.hh"
#include "format-stream.hh"
#include "print.hh"

namespace mimosa
{
  namespace format
  {
    bool printByteSize(stream::Stream & stream, uint64_t value)
    {
      static const char *units[] = {
        "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"
      };

      int unit   = 2;

      if (value < 1024)
      {
        return printDecimal(stream, value) && printStatic(stream, "B");
      }

      for (unit = 6; value < 1ull << (10 * unit); --unit)
        ;

      const int integer_part = value / (1ull << (10 * unit));

      if (!printDecimal(stream, integer_part))
        return false;

      int fractional_part = (value % (1ull << (10 * unit))) >> (10 * (unit - 1));
      fractional_part = (100 * fractional_part) >> 10;
      while (!(fractional_part % 10) && fractional_part > 0)
        fractional_part /= 10;

      if (fractional_part > 0 && integer_part < 100)
      {
        printStatic(stream, ".");
        if (integer_part > 10 && fractional_part >= 10)
          fractional_part /= 10;
        if (!printDecimal(stream, fractional_part))
          return false;
      }

      return print(stream, units[unit]);
    }

    bool printHex(stream::Stream & stream, uint64_t value)
    {
      // 2 ** 64 - 1 = ffffffffffffffff
      char buffer[16];
      char *it = buffer + sizeof (buffer);

      if (value == 0)
        return stream.loopWrite("0", 1) == 1;

      while (value > 0)
      {
        --it;
        *it = "0123456789abcdef"[value & 0xf];
        value >>= 4;
      }

      const int64_t len = sizeof (buffer) - (it - buffer);
      return stream.loopWrite(it, len) == len;
    }

    bool printDuration(stream::Stream & stream, Time time)
    {
      uint32_t secs  = (time / second) % 60;
      uint32_t mins  = (time / minute) % 60;
      uint32_t hours = (time / hour) % 24;
      uint32_t days  = time / day;

      return format(stream, "%d days, %d:%d:%d", days, hours, mins, secs);
    }
  }
}
