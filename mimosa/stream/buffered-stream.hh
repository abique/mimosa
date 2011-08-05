#ifndef MIMOSA_STREAM_BUFFERED_STREAM_HH
# define MIMOSA_STREAM_BUFFERED_STREAM_HH

# include <cstring>
# include <string>

# include "stream.hh"
# include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    class BufferedStream : public Stream
    {
    public:
      MIMOSA_DEF_PTR(BufferedStream);

      BufferedStream(Stream::Ptr stream, uint64_t buffer_size = 64 * 1024);
      virtual ~BufferedStream();

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual bool flush(runtime::Time timeout = 0);

      /** Zero copy stuff
       * @{ */
      inline Buffer::Ptr read(runtime::Time timeout = 0) { return read(buffer_size_, timeout); }
      Buffer::Ptr read(uint64_t buffer_size, runtime::Time timeout = 0);
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
                            runtime::Time      timeout = 0);
      /** @} */

    private:
      inline uint64_t readyWrite() const;
      void append(const char * data, uint64_t nbytes);

      Stream::Ptr    stream_;
      const uint64_t buffer_size_;

      Buffer::Slist wbuffers_; // a list of write buffers
      uint64_t      wpos_;     // write position in the first buffer
      uint64_t      wappend_;  // write append position in the last buffer

      Buffer::Ptr   rbuffer_;  // a single read buffers
      uint64_t      rpos_;     // the read position in the first buffer
      uint64_t      rappend_;  // the append position in the last buffer
    };
  }
}

#endif /* !MIMOSA_STREAM_BUFFERED_STREAM_HH */
