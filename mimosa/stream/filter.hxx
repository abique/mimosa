#include "string-stream.hh"

namespace mimosa
{
  namespace stream
  {
    template <typename T>
    inline std::string filter(const char * data, uint64_t nbytes)
    {
      mimosa::stream::StringStream::Ptr s = new mimosa::stream::StringStream;
      T filter(s);
      filter.write(data, nbytes);
      return s->moveStr();
    }

    template <typename T>
    inline std::string filter(const std::string & data)
    {
      return filter<T>(data.data(), data.size());
    }
  }
}
