#include <cstring>

#include "string-stream.hh"

namespace mimosa
{
  namespace stream
  {
    StringStream::StringStream(const std::string & str)
      : str_(str)
    {
    }

    int64_t
    StringStream::write(const char * data, uint64_t nbytes, runtime::Time /*timeout*/)
    {
      str_.append(data, nbytes);
      return nbytes;
    }

    int64_t
    StringStream::read(char * data, uint64_t nbytes, runtime::Time /*timeout*/)
    {
      if (str_.size() <= read_pos_)
        return 0;

      nbytes = str_.size() - read_pos_;
      ::memcpy(data, str_.data() + read_pos_, nbytes);
      return nbytes;
    }
  }
}
