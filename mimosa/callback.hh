#ifndef MIMOSA_CALLBACK_HH
# define MIMOSA_CALLBACK_HH

# include "ref-countable.hh"

namespace mimosa
{
  template <typename Ret, typename ... Args>
  class Callback : public RefCountable<Callback>
  {
  public:
    virtual Ret operator()(Args ... args) const = 0;
    inline virtual ~Callback() {}
  };
}

#endif /* !MIMOSA_CALLBACK_HH */
