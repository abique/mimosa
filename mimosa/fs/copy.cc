#include "copy.hh"

#include "../stream/copy.hh"
#include "../stream/direct-fd-stream.hh"

namespace mimosa
{
  namespace fs
  {
    bool copyFile(const std::string &src, const std::string &dst)
    {
      stream::DirectFdStream srcStream;

      struct ::stat st;

      if (::stat(src.c_str(), &st))
        return false;

      if (!srcStream.open(src.c_str(), O_RDONLY))
        return false;

      stream::DirectFdStream dstStream;
      if (!dstStream.open(dst.c_str(), O_CREAT | O_TRUNC | O_WRONLY))
        return false;

      if (st.st_size != stream::copy(srcStream, dstStream)) {
        ::unlink(dst.c_str());
        return false;
      }

      return true;
    }
  }
}
