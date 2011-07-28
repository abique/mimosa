#ifndef MIMOSA_FUNCTION_HH
# define MIMOSA_FUNCTION_HH

# include <functional>

# include "ref-countable.hh"
# include "non-copyable.hh"

namespace mimosa
{
  template <typename Proto>
  class Function : public std::function<Proto>,
                   public RefCountable<Function<Proto> >,
                   private NonCopyable
  {
  };
}

#endif /* !MIMOSA_FUNCTION_HH */
