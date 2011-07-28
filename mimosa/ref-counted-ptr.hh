#ifndef MIMOSA_REF_COUNTED_PTR_HH
# define MIMOSA_REF_COUNTED_PTR_HH

namespace mimosa
{
  template <typename T>
  class RefCountedPtr
  {
  public:
    inline RefCountedPtr()
      : ptr_(0)
    {
    }

    inline RefCountedPtr(T * ptr)
      : ptr_(ptr)
    {
      if (ptr)
        addRef(ptr);
    }

    template <typename U>
    inline RefCountedPtr(const RefCountedPtr<U> & ptr)
    {
      assign(ptr.get());
    }

    inline ~RefCountedPtr()
    {
      assign(0);
    }

    inline RefCountedPtr<T> & operator=(T * ptr)
    {
      assign(ptr);
      return *this;
    }

    inline RefCountedPtr<T> & operator=(const RefCountedPtr<T> & ptr)
    {
      assign(ptr.get());
      return *this;
    }

    inline T & operator*() const { return *ptr_; }
    inline T * operator->() const { return ptr_; }
    inline T * get() const { return ptr_; }
    inline void assign(T * ptr)
    {
      if (ptr)
        addRef(ptr);
      if (ptr_)
        releaseRef(ptr_);
      ptr_ = ptr;
    }

  private:
    T * ptr_;
  };
}

#endif /* !MIMOSA_REF_COUNTED_PTR_HH */
