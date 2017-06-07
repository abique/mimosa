#pragma once

# include <cstdint>
# include <string>

namespace mimosa
{
  namespace uri
  {
    enum EncodingType
    {
      kRfc2396, // encodes ' ' as '+', decodes '+' as ' '
      kRfc3986, // encodes ' ' as '%20', decodes '+' as '+'
    };

    void percentEncode(const char *  input,
                       uint32_t      input_len,
                       std::string * output,
                       EncodingType  rfc = kRfc3986);

    void percentDecode(const char *  input,
                       uint32_t      input_len,
                       std::string * output,
                       EncodingType  rfc = kRfc3986);
  }
}

