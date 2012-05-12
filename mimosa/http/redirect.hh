#ifndef MIMOSA_HTTP_REDIRECT_HH
# define MIMOSA_HTTP_REDIRECT_HH

# include <string>

namespace mimosa
{
  namespace http
  {
    class ResponseWriter;

    /**
     * @ingroup Http
     */
    bool redirect(ResponseWriter & response, const std::string & location);
  }
}

#endif /* !MIMOSA_HTTP_REDIRECT_HH */
