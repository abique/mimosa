#include "request-reader.hh"

namespace mimosa
{
  namespace http
  {
    RequestReader::RequestReader(stream::FdStream::Ptr stream)
      : stream_(stream)
    {
    }

    int64_t
    RequestReader::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      assert(false && "TODO");
      return -1;
    }

    int64_t
    RequestReader::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      assert(false && "TODO");
      return -1;
    }
  }
}
