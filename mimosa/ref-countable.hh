#ifndef MIMOSA_REF_COUNTABLE_HH
# define MIMOSA_REF_COUNTABLE_HH

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

    inline RefCountableBase(const RefCountableBase & other)
      : ref_count_(0)
    {
    }

    inline operator=(const RefCountableBase & other)
    {
    }

    inline int addRef() const
    {
      return __sync_add_and_fetch(&obj->ref_count_, 1);
    }

    inline int releaseRef() const
    {
      return __sync_add_and_fetch(&obj->ref_count_, -1);
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

  template <typename T>
  class RefCountable : public RefCountableBase
  {
  public:
    typedef RefCountedPtr<T> Ptr;
    typedef RefCountedPtr<const T> ConstPtr;
  };
}

#endif /* !MIMOSA_REF_COUNTABLE_HH */
