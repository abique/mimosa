#include "cookie.hh"

namespace mimosa
{
  namespace http
  {
    Cookie::Cookie()
      : key_(),
        value_(),
        is_secure_(false)
    {
    }
  }
}
