#ifndef MIMOSA_NON_COPYABLE_HH
# define MIMOSA_NON_COPYABLE_HH

namespace mimosa
{
  class NonCopyable
  {
  public:
    inline NonCopyable() {}

    // allow move stuff
    inline NonCopyable(NonCopyable &&) {}
    inline NonCopyable & operator=(NonCopyable&&) { return *this; }

    NonCopyable(const NonCopyable &) = delete;
    NonCopyable & operator=(const NonCopyable &) = delete;
  };
}

#endif /* !MIMOSA_NON_COPYABLE_HH */
