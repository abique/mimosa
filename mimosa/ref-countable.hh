#pragma once

#include <cassert>
#include <atomic>

#include "ref-counted-ptr.hh"

namespace mimosa
{
  class RefCountableBase
  {
  public:
    inline constexpr RefCountableBase() noexcept = default;

    inline constexpr RefCountableBase(const RefCountableBase &) noexcept
    {
    }

    inline constexpr RefCountableBase(RefCountableBase &&) noexcept
    {
    }

    virtual ~RefCountableBase() noexcept
    {
       assert(ref_count_ == 0 || ref_count_ == 1);
    }

    inline constexpr RefCountableBase & operator=(const RefCountableBase &) noexcept
    {
      return *this;
    }

    inline constexpr RefCountableBase & operator=(RefCountableBase &&) noexcept
    {
      return *this;
    }

    inline int addRef() const noexcept
    {
      auto ret = ++ref_count_;
      assert(ret >= 1);
      return ret;
    }

    inline int releaseRef() const noexcept
    {
      auto ret = --ref_count_;
      assert(ret >= 0);
      return ret;
    }

    inline int32_t refCount() const noexcept
    {
       return ref_count_;
    }

    inline void allocatedOnStack() noexcept
    {
       assert(ref_count_ == 0);
       addRef();
    }

  private:
    mutable std::atomic<int32_t> ref_count_ = {0};
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
