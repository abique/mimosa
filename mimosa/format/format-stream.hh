#ifndef MIMOSA_FORMAT_FORMAT_STREAM_HH
# define MIMOSA_FORMAT_FORMAT_STREAM_HH

# include "format.hh"
# include "../stream/stream.hh"

namespace mimosa
{
  namespace format
  {
    template <typename ... Args>
    inline
    bool format(stream::Stream::Ptr stream,
                runtime::Time       timeout,
                const char *        fmt,
                Args ...            args)
    {
      std::string str(format::str(fmt, args...));
      int64_t bytes = stream->loopWrite(str.data(), str.size(), timeout);
      return bytes == str.size();
    }

    template <typename ... Args>
    inline
    bool format(stream::Stream::Ptr stream,
                const char *        fmt,
                Args ...            args)
    {
      return format(stream, static_cast<runtime::Time>(0), fmt, args...);
    }
  }
}

#endif /* !MIMOSA_FORMAT_FORMAT_STREAM_HH */
