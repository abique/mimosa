#pragma once

# include <string>

# include "method-handler.hh"

namespace mimosa
{
  namespace http
  {
    /**
     * @ingroup Http
     */
    class FsHandler : public MethodHandler
    {
    public:
      FsHandler(const std::string & root, int nskip);

      virtual bool head(RequestReader & request, ResponseWriter & response) const;
      virtual bool get(RequestReader & request, ResponseWriter & response) const;
      virtual bool del(RequestReader & request, ResponseWriter & response) const;
      virtual bool put(RequestReader & request, ResponseWriter & response) const;
      virtual bool mkcol(RequestReader & request, ResponseWriter & response) const;
      virtual bool move(RequestReader & request, ResponseWriter & response) const;

      static bool streamFile(RequestReader &     request,
                             ResponseWriter &    response,
                             const std::string & real_path,
                             struct stat &       st);

      static bool streamFile(RequestReader &     request,
                             ResponseWriter &    response,
                             const std::string & real_path);

      inline void enableReaddir(bool enable) { can_readdir_ = enable; }
      inline void enableHead(bool enable) { can_head_ = enable; }
      inline void enableGet(bool enable) { can_get_ = enable; }
      inline void enablePut(bool enable) { can_put_ = enable; }
      inline void enableDelete(bool enable) { can_delete_ = enable; }
      inline void enableMkcol(bool enable) { can_mkcol_ = enable; }
      inline void enableXattr(bool enable) { use_xattr_ = enable; }
      inline void enableSymlink(bool enable) { can_symlink_ = enable; }
      inline void enableMove(bool enable) { can_move_ = enable; }

    private:
      std::string checkPath(const std::string & location) const;
      std::string checkPath(RequestReader & request) const;

      bool readDir(RequestReader &     request,
                   ResponseWriter &    response,
                   const std::string & real_path) const;
      bool readDirHtml(RequestReader &     request,
                       ResponseWriter &    response,
                       const std::string & real_path) const;
      bool readDirJson(RequestReader &     request,
                       ResponseWriter &    response,
                       const std::string & real_path) const;

      std::string root_;  // the root
      int         nskip_; // the number of directory to skip
                          // from the begining of the path
      bool        can_readdir_ : 1;
      bool        can_head_ : 1;
      bool        can_get_ : 1;
      bool        can_put_ : 1;
      bool        can_delete_ : 1;
      bool        can_mkcol_ : 1;
      bool        use_xattr_ : 1;
      bool        can_symlink_ : 1;
      bool        can_move_ : 1;
    };
  }
}

