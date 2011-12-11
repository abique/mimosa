#ifndef MIMOSA_REF_COUNTED_PTR_HH
# define MIMOSA_REF_COUNTED_PTR_HH

namespace mimosa
{
  template <typename T>
  class RefCountedPtr
  {
  public:
    inline RefCountedPtr()
      : ptr_(nullptr)
    {
    }

    inline RefCountedPtr(T * ptr)
      : ptr_(ptr)
    {
      if (ptr_)
        addRef(ptr_);
    }

    inline RefCountedPtr(const RefCountedPtr<T> & ptr)
      : ptr_(ptr.ptr_)
    {
      if (ptr_)
        addRef(ptr_);
    }

    template <typename U>
    inline RefCountedPtr(const RefCountedPtr<U> & ptr)
      : ptr_(ptr.get())
    {
      if (ptr_)
        addRef(ptr_);
    }

    inline RefCountedPtr(RefCountedPtr<T> && ptr)
      : ptr_(ptr.ptr_)
    {
      ptr.ptr_ = nullptr;
    }

    template <typename U>
    inline RefCountedPtr(RefCountedPtr<U> && ptr)
      : ptr_(ptr.get())
    {
      ptr.ptr_ = nullptr;
    }

    inline ~RefCountedPtr()
    {
      assign(nullptr);
    }

    inline RefCountedPtr<T> & operator=(T * ptr)
    {
      assign(ptr);
      return *this;
    }

    inline RefCountedPtr<T> & operator=(const RefCountedPtr<T> & ptr)
    {
      assign(ptr.ptr_);
      return *this;
    }

    template <typename U>
    inline RefCountedPtr<T> & operator=(const RefCountedPtr<U> & ptr)
    {
      assign(ptr.ptr_);
      return *this;
    }

    inline bool operator!() const
    {
      return !ptr_;
    }

    inline operator bool () const
    {
      return ptr_;
    }

    inline operator T* () const
    {
      return ptr_;
    }

    template <typename U>
    inline bool operator==(const U * ptr) const
    {
      return ptr_ == ptr;
    }

    template <typename U>
    inline bool operator==(const RefCountedPtr<U> & ptr) const
    {
      return ptr_ == ptr.ptr_;
    }

    inline T & operator*() const { return *ptr_; }
    inline T * operator->() const { return ptr_; }
    template <typename Member>
    inline auto operator->*(Member member) const -> decltype(((T*)nullptr)->*member) { return ptr_->*member; }
    inline T * get() const { return ptr_; }
    inline void assign(T * ptr)
    {
      if (ptr)
        addRef(ptr);
      if (ptr_)
        releaseRef(ptr_);
      ptr_ = ptr;
    }

    template <typename U>
    inline operator RefCountedPtr<U>() const {
      return RefCountedPtr<U>(ptr_);
    }

    inline void clear() { assign(nullptr); }

  private:
    template <class U> friend class RefCountedPtr;

    T * ptr_;
  };
}

#endif /* !MIMOSA_REF_COUNTED_PTR_HH */
