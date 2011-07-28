#ifndef MIMOSA_BIND_HH
# define MIMOSA_BIND_HH

# include "function.hh"

namespace mimosa
{
  template <typename Proto, typename ... Args>
  inline Function<Proto>::Ptr
  bind(Proto function, Args ... args)
  {
    return new Function<Proto>(std::bind(function, ...args));
  }
}

#endif /* !MIMOSA_BIND_HH */
