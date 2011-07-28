#ifndef MIMOSA_HTTP_REDIRECT_HH
# define MIMOSA_HTTP_REDIRECT_HH

namespace mimosa
{
  namespace http
  {
    void redirect(Response & response, const std::string & location);
  }
}

#endif /* !MIMOSA_HTTP_REDIRECT_HH */
