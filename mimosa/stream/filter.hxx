#include "string-stream.hh"

namespace mimosa
{
  namespace stream
  {
    template <typename T>
    std::string && filter(const void * data, uint64_t nbytes)
    {
      mimosa::stream::StringStream::Ptr s = new mimosa::stream::StringStream;
      T filter(s);
      filter.write(data.data(), data.size());
      return s->moveStr();
    }

    template <typename T>
    std::string && filter(const std::string & data)
    {
      return filter<T>(data.data(), data.size());
    }
  }
}
