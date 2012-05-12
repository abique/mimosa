#ifndef MIMOSA_FS_FIND_HH
# define MIMOSA_FS_FIND_HH

# include <string>
# include <functional>

namespace mimosa
{
  namespace fs
  {
    /**
     * @ingroup Fs
     */
    void
    find(const std::string &                                    root,
         bool                                                   is_recursive,
         const std::function<void (const std::string & path)> & cb);
  }
}

#endif /* !MIMOSA_FS_FIND_HH */
