#include <cerrno>

#include "writer.hh"

namespace mimosa
{
  namespace archive
  {
    static int archiveOpenCb(struct archive * /*archive*/,
                             void *           ctx)
    {
      auto * stream = reinterpret_cast<stream::Stream *>(ctx);
      stream->addRef();
      return 0;
    }

    static ssize_t archiveWriteCb(struct archive * /*archive*/,
                                  void *           ctx,
                                  const void *     buffer,
                                  size_t           len)
    {
      auto * stream = reinterpret_cast<stream::Stream *>(ctx);
      return stream->loopWrite((const char*)buffer, len);
    }

    static int archiveCloseCb(struct archive * /*archive*/,
                             void *            ctx)
    {
      auto * stream = reinterpret_cast<stream::Stream *>(ctx);
      stream->flush();
      stream->releaseRef();
      return 0;
    }

    int
    Writer::open(stream::Stream::Ptr stream)
    {
      return archive_write_open(
        archive_, stream, archiveOpenCb, archiveWriteCb, archiveCloseCb);
    }

    int64_t
    Writer::write(const char *data, uint64_t nbytes)
    {
      return archive_write_data(archive_, data, nbytes);
    }

    int64_t
    Writer::read(char *, uint64_t)
    {
      assert(false && "should not call");
      errno = EINVAL;
      return -1;
    }
  }
}
