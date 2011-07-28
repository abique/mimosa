#ifndef MIMOSA_STREAM_BUFFER_HH
# define MIMOSA_STREAM_BUFFER_HH

# include "../ref-countable.hh"

namespace mimosa
{
  namespace stream
  {
    class Buffer : public RefCountable<Buffer>
    {
    public:
      typedef container::IntrusiveSlist<Buffer, &Buffer::next_> Slist;

      Buffer(uint64_t size = 64 * 1024);
      ~Buffer();

      inline uint64_t size() const { return size_; }
      inline char * data() { return data_; }
      inline const char * data() const { return data_; }

    protected:
      container::IntrusiveSlistHook<Buffer> next_;
      uint64_t                              size_;
      char *                                data_;
    };
  }
}

#endif /* !MIMOSA_STREAM_BUFFER_HH */
