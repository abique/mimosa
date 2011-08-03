#ifndef MIMOSA_HTTP_COOKIE_HH
# define MIMOSA_HTTP_COOKIE_HH

# include "../ref-countable.hh"
# include "../container/intrusive-slist.hh"

namespace mimosa
{
  namespace http
  {
    class Cookie : public RefCountable<Cookie>
    {
    public:
      container::IntrusiveSlistHook<Cookie::Ptr> next_;

      typedef container::IntrusiveSlist<Cookie, Cookie::Ptr, &Cookie::next_> Slist;
    };
  }
}

#endif /* !MIMOSA_HTTP_COOKIE_HH */
