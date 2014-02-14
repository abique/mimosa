#include "writer.hh"

namespace mimosa
{
  namespace archive
  {
    static int archiveOpenCb(struct archive * /*archive*/,
                             void *           ctx)
    {
      stream::Stream * stream = reinterpret_cast<stream::Stream *>(ctx);
      stream->addRef();
      return 0;
    }

    static ssize_t archiveWriteCb(struct archive * /*archive*/,
                                  void *           ctx,
                                  const void *     buffer,
                                  size_t           len)
    {
      stream::Stream * stream = reinterpret_cast<stream::Stream *>(ctx);
      return stream->loopWrite((const char*)buffer, len);
    }

    static int archiveCloseCb(struct archive * /*archive*/,
                             void *            ctx)
    {
      stream::Stream * stream = reinterpret_cast<stream::Stream *>(ctx);
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
  }
}
