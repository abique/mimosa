#include <cstring>

#include "string-stream.hh"

namespace mimosa
{
  namespace stream
  {
    StringStream::StringStream(const std::string & str)
      : read_pos_(0),
        str_(str)
    {
    }

    int64_t
    StringStream::write(const char * data, uint64_t nbytes)
    {
      str_.append(data, nbytes);
      return nbytes;
    }

    int64_t
    StringStream::read(char * data, uint64_t nbytes)
    {
      if (str_.size() <= read_pos_)
        return 0;

      int64_t can_read = std::min(nbytes, str_.size() - read_pos_);
      ::memcpy(data, str_.data() + read_pos_, can_read);
      read_pos_ += can_read;
      return can_read;
    }
  }
}
