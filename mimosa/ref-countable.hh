#ifndef MIMOSA_REF_COUNTABLE_HH
# define MIMOSA_REF_COUNTABLE_HH

# include <cassert>

# include "ref-counted-ptr.hh"

namespace mimosa
{
  class RefCountableBase
  {
  public:
    inline RefCountableBase()
      : ref_count_(0)
    {
    }

    inline RefCountableBase(const RefCountableBase & /*other*/)
      : ref_count_(0)
    {
    }

    virtual ~RefCountableBase() {}

    inline RefCountableBase & operator=(const RefCountableBase & /*other*/)
    {
      return *this;
    }

    inline int addRef() const
    {
      auto ret = __sync_add_and_fetch(&ref_count_, 1);
      assert(ret >= 1);
      return ret;
    }

    inline int releaseRef() const
    {
      auto ret = __sync_add_and_fetch(&ref_count_, -1);
      assert(ret >= 0);
      return ret;
    }

    mutable int ref_count_;
  };

  inline void addRef(const RefCountableBase * obj)
  {
    obj->addRef();
  }

  inline void releaseRef(const RefCountableBase * obj)
  {
    if (obj->releaseRef() == 0)
      delete obj;
  }

# define MIMOSA_DEF_PTR(T)                      \
  typedef RefCountedPtr<T> Ptr;                 \
  typedef RefCountedPtr<const T> ConstPtr;

  template <typename T>
  class RefCountable : public RefCountableBase
  {
  public:
    MIMOSA_DEF_PTR(T);
  };
}

#endif /* !MIMOSA_REF_COUNTABLE_HH */
