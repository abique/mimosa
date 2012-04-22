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

  private:
    NonCopyable(const NonCopyable &);
    NonCopyable & operator=(const NonCopyable &);
  };
}

#endif /* !MIMOSA_NON_COPYABLE_HH */
