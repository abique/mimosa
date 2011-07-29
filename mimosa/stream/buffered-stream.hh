#ifndef MIMOSA_STREAM_BUFFERED_STREAM_HH
# define MIMOSA_STREAM_BUFFERED_STREAM_HH

# include "stream.hh"
# include "buffer.hh"

namespace mimosa
{
  namespace stream
  {
    class BufferedStream : public Stream, public Ptr<BufferedStream>
    {
    public:
      BufferedStream(Stream::Ptr stream, uint64_t buffer_size = 64 * 1024);
      virtual ~BufferedStream();

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual bool flush(runtime::Time timeout = 0);

      /* zero copy stuff */
      inline Buffer::Ptr read(runtime::Time timeout = 0) { return read(buffer_size_, timeout); }
      Buffer::Ptr read(uint64_t buffer_size, runtime::Time timeout = 0);

    private:
      inline uint64_t readyWrite() const;
      void append(const char * data, uint64_t nbytes);

      Stream::Ptr    stream_;
      const uint64_t buffer_size_;

      Buffer::Slist wbuffers_; // a list of write buffers, should not exceed 2 buffers
      uint64_t      wpos_;     // write position in the first buffer
      uint64_t      wappend_;  // write append position in the last buffer

      Buffer * rbuffer_; // a single read buffer
      uint64_t rpos_;    // the read position
      uint64_t rappend_; // the append position
    };
  }
}

#endif /* !MIMOSA_STREAM_BUFFERED_STREAM_HH */
