#ifndef MIMOSA_STREAM_BUFFER_HH
# define MIMOSA_STREAM_BUFFER_HH

# include <cstdint>

# include "../ref-countable.hh"
# include "../intrusive-slist.hh"

namespace mimosa
{
  namespace stream
  {
    /**
     * @todo copy constructor, move constructor, operator=
     * @info the real size of the buffer is size + 4. So it's null terminated
     * and you can pass the buffer to flex without duplicating it.
     */
    class Buffer : public RefCountable<Buffer>
    {
    public:
      Buffer(uint64_t size = 64 * 1024);
      /** constructs a copy of the data buffer */
      Buffer(const char * data, uint64_t size);
      ~Buffer();

      inline uint64_t size() const { return size_; }
      inline char * data() { return data_; }
      inline const char * data() const { return data_; }
      void resize(uint64_t size);

    private:
      union {
        IntrusiveSlistHook<Buffer*>     next_;
        IntrusiveSlistHook<Buffer::Ptr> next_ptr_;
      };
      uint64_t                    size_;
      char *                      data_;

    public:
      typedef IntrusiveSlist<Buffer, Buffer *, &Buffer::next_> Slist;
      friend class IntrusiveSlist<Buffer, Buffer *, &Buffer::next_>;

      typedef IntrusiveSlist<Buffer, Buffer::Ptr, &Buffer::next_ptr_> SlistPtr;
      friend class IntrusiveSlist<Buffer, Buffer::Ptr, &Buffer::next_ptr_>;
    };
  }
}

#endif /* !MIMOSA_STREAM_BUFFER_HH */
