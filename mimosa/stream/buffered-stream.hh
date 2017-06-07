#pragma once

# include <cstring>
# include <string>

# include "filter.hh"
# include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    class BufferedStream : public Filter
    {
    public:
      MIMOSA_DEF_PTR(BufferedStream);

      BufferedStream(Stream::Ptr stream, uint64_t buffer_size = 64 * 1024);
      virtual ~BufferedStream();

      /** The number of bytes which are in the read buffer */
      uint64_t readyRead() const;
      /** The number of bytes which are in the write buffer */
      uint64_t readyWrite() const;

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);
      virtual bool flush();

      /** Zero copy stuff
       * @{ */
      inline Buffer::Ptr read() { return read(buffer_size_); }
      Buffer::Ptr read(uint64_t buffer_size);
      /** @} */

      /** Helpers
       * @{ */

      /**
       * @param max_size if str is not reached until max_size,
       * returns nullptr and errno is set to EFBIG
       * @param str the string to search
       * @return a valid buffer on success, null otherwise. */
      Buffer::Ptr readUntil(const char * const str,
                            uint64_t           max_size = 0,
                            bool *             found    = nullptr);
      /** @} */

      virtual void close();

      inline Stream* underlyingStream() const { return stream_; }

    private:
      bool flushWith(const char *data, uint64_t nbytes);

      const uint64_t buffer_size_;

      Buffer        wbuffer_;
      uint64_t      wpos_;     // write position in the buffer

      Buffer::Ptr rbuffer_;     // a single read buffers
      uint64_t    rpos_;        // the read position in the first buffer
      uint64_t    rappend_;     // the append position in the last buffer
    };
  }
}

