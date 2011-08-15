#ifndef MIMOSA_HTTP_FSHANDLER_HH
# define MIMOSA_HTTP_FSHANDLER_HH

# include <string>

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    class FsHandler : public Handler
    {
    public:
      FsHandler(std::string && root, int nskip);

      /** This is the handle method. It must be thread-safe. */
      virtual bool handle(Request & request, ResponseWriter & response) const;

    private:
      std::string root_;  // the root
      int         nskip_; // the number of directory to skip
                          // from the begining of the path
    };
  }
}

#endif /* !MIMOSA_HTTP_FSHANDLER_HH */
