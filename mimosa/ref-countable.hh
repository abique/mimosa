#pragma once

#include <cassert>
#include <atomic>

#include "ref-counted-ptr.hh"

namespace mimosa
{
  class RefCountableBase
  {
  public:
    inline RefCountableBase()
      : ref_count_(0)
    {
    }

    inline RefCountableBase(const RefCountableBase &)
      : ref_count_(0)
    {
    }

    inline RefCountableBase(RefCountableBase &&)
      : ref_count_(0)
    {
    }

    virtual ~RefCountableBase() {}

    inline RefCountableBase & operator=(const RefCountableBase &)
    {
      return *this;
    }

    inline RefCountableBase & operator=(RefCountableBase &&)
    {
      return *this;
    }

    inline int addRef() const
    {
      auto ret = ++ref_count_;
      assert(ret >= 1);
      return ret;
    }

    inline int releaseRef() const
    {
      auto ret = --ref_count_;
      assert(ret >= 0);
      return ret;
    }

    mutable std::atomic<int> ref_count_;
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

# define MIMOSA_DEF_PTR(T...)                           \
  typedef ::mimosa::RefCountedPtr<T > Ptr;              \
  typedef ::mimosa::RefCountedPtr<const T > ConstPtr

  template <typename T >
  class RefCountable : public ::mimosa::RefCountableBase
  {
  public:
    MIMOSA_DEF_PTR(T);
  };
}
