#ifndef MIMOSA_NON_COPYABLE_HH
# define MIMOSA_NON_COPYABLE_HH

namespace mimosa
{
  class NonCopyable
  {
  public:
    inline NonCopyable() {}
  private:
    NonCopyable(const NonCopyable &);
    NonCopyable & operator=(const NonCopyable &);
  };
}

#endif /* !MIMOSA_NON_COPYABLE_HH */
