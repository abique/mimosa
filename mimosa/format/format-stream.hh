#pragma once

# include "format.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace format
  {
    // XXX take a reference to stream, not a refcounted pointer
    template <typename ... Args>
    inline
    bool format(stream::Stream & stream,
                const char *     fmt,
                Args ...         args)
    {
      std::string str(format::str(fmt, args...));
      if (str.empty())
        return true;
      int64_t bytes = stream.loopWrite(str.data(), str.size());
      return static_cast<std::string::size_type> (bytes) == str.size();
    }
  }
}

