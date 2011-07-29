#ifndef MIMOSA_BIND_HH
# define MIMOSA_BIND_HH

# include <functional>

# include "function.hh"

namespace mimosa
{
  // TODO

  template <typename Fct, typename ... Args>
  struct GetFunction
  {
    typedef Function<decltype(std::bind(Fct, Args...))>::Ptr Type;
  };

  template <typename Fct, typename ... Args>
  inline GetFunction<Fct, Args...>::Type
  bind(Fct fct, Args ... args)
  {
    return new T(std::bind(fct, args...));
  }
}

#endif /* !MIMOSA_BIND_HH */
