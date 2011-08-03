#ifndef MIMOSA_STREAM_BUFFER_HH
# define MIMOSA_STREAM_BUFFER_HH

# include <cstdint>

# include "../ref-countable.hh"
# include "../container/intrusive-slist.hh"

namespace mimosa
{
  namespace stream
  {
    class Buffer : public RefCountable<Buffer>
    {
    public:
      Buffer(uint64_t size = 64 * 1024);
      ~Buffer();

      inline uint64_t size() const { return size_; }
      inline char * data() { return data_; }
      inline const char * data() const { return data_; }
      void resize(uint64_t size);

    private:
      container::IntrusiveSlistHook<Buffer::Ptr> next_;
      uint64_t                                   size_;
      char *                                     data_;

    public:
      typedef container::IntrusiveSlist<Buffer, Buffer::Ptr, &Buffer::next_> Slist;
      friend class container::IntrusiveSlist<Buffer, Buffer::Ptr, &Buffer::next_>;
    };
  }
}

#endif /* !MIMOSA_STREAM_BUFFER_HH */
