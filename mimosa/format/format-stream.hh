#ifndef MIMOSA_FORMAT_FORMAT_STREAM_HH
# define MIMOSA_FORMAT_FORMAT_STREAM_HH

# include "format.hh"
# include "../stream.hh"

namespace mimosa
{
  namespace format
  {
    template <typename ... Args>
    inline
    bool format(stream::Ptr    stream,
                const char *   fmt,
                Args ...       args)
    {
      std::string str(format::str(fmt, args));
      int64_t bytes = stream->loopWrite(str.data(), str.size());
      return bytes == str.size();
    }
  }
}

#endif /* !MIMOSA_FORMAT_FORMAT_STREAM_HH */
