#include <cstdint>

#include "percent-encoding.hh"

namespace mimosa
{
  namespace uri
  {
    /* see tools/gen-percent-encoding-tables */

    static const uint64_t is_valid[4] = {
      0b1010111111111111111101111101101000000000000000000000000000000001ULL,
      0b0100011111111111111111111111111010101111111111111111111111111111ULL,
      0b0000000000000000000000000000000000000000000000000000000000000000ULL,
      0b0000000000000000000000000000000000000000000000000000000000000000ULL,
    };

    static const char hex[] = "0123456789ABCDEF";

    static bool isValid(uint8_t c)
    {
      return is_valid[c >> 6] & (1ULL << (c & 0x3f));
    }

    void percentEncode(const char *  input,
                       uint32_t      input_len,
                       std::string * output,
                       EncodingType  rfc)
    {
      output->clear();
      output->resize(input_len * 3);

      char *             out = &(*output)[0];
      const char * const end = input + input_len;

      for (; input < end; ++input, ++out)
      {
        uint8_t c = *input;
        if (rfc == kRfc2396 && c == ' ')
          *out = '+';
        else if (isValid(c))
          *out = c;
        else
        {
          *out = '%';
          *++out = hex[c >> 4];
          *++out = hex[c & 0xf];
        }
      }
      output->resize(out - &(*output)[0]);
    }

    static uint8_t xdigitValue(char c)
    {
      if ('0' <= c && c <= '9')
        return c - '0';
      if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
      if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
      return 0;
    }

    void percentDecode(const char *  input,
                       uint32_t      input_len,
                       std::string * output,
                       EncodingType  rfc)
    {
      output->clear();
      output->resize(input_len);

      char *             out = &(*output)[0];
      const char * const end = input + input_len;

      for (; input < end; ++input, ++out)
      {
        uint8_t c = *input;
        if (rfc == kRfc2396 && c == '+')
          *out = ' ';
        else if (c == '%' && end - input > 2 && isxdigit(input[1]) && isxdigit(input[2]))
        {
          *out = (xdigitValue(input[1]) << 4) | xdigitValue(input[2]);
          input += 2;
        }
        else
          *out = *input;
      }
      output->resize(out - &(*output)[0]);
    }
  }
}
