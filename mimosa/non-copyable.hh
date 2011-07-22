#ifndef MIMOSA_NON_COPYABLE_HH
# define MIMOSA_NON_COPYABLE_HH

namespace mimosa
{
  class NonCopyable
  {
  private:
    NonCopyable(const NonCopyable &);
  };
}

#endif /* !MIMOSA_NON_COPYABLE_HH */
