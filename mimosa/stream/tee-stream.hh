#pragma once

# include <vector>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    /**
     * TeeStream is mainly used for debug purpose.
     * It copies everything wrote to ostreams_ and
     * everything read to istreams_.
     */
    class TeeStream : public Stream
    {
    public:
      MIMOSA_DEF_PTR(TeeStream);

      explicit TeeStream(Stream::Ptr stream);

      inline void teeOutput(Stream::Ptr stream) { ostreams_.push_back(stream); }
      inline void teeInput(Stream::Ptr stream) { istreams_.push_back(stream); }

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      virtual void close();
      virtual bool flush();

      Stream::Ptr stream_;
      std::vector<Stream::Ptr> ostreams_;
      std::vector<Stream::Ptr> istreams_;
    };
  }
}

