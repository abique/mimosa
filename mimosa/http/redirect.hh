#ifndef MIMOSA_HTTP_REDIRECT_HH
# define MIMOSA_HTTP_REDIRECT_HH

# include <string>

# include "response-writer.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    bool redirect(ResponseWriter & response, const std::string & location);
  }
}

#endif /* !MIMOSA_HTTP_REDIRECT_HH */
