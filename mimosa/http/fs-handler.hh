#ifndef MIMOSA_HTTP_FSHANDLER_HH
# define MIMOSA_HTTP_FSHANDLER_HH

# include <string>

# include "handler.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class FsHandler : public Handler
    {
    public:
      FsHandler(const std::string & root, int nskip, bool enable_readdir = false);

      /** This is the handle method. It must be thread-safe. */
      virtual bool handle(RequestReader & request, ResponseWriter & response) const;

    private:
      bool streamFile(RequestReader &     request,
                      ResponseWriter &    response,
                      const std::string & real_path,
                      struct stat &       st) const;

      bool readDir(RequestReader &     request,
                   ResponseWriter &    response,
                   const std::string & real_path) const;

      std::string root_;  // the root
      int         nskip_; // the number of directory to skip
                          // from the begining of the path
      bool        can_readdir_;
    };
  }
}

#endif /* !MIMOSA_HTTP_FSHANDLER_HH */
