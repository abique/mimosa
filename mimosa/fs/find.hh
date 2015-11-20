#ifndef MIMOSA_FS_FIND_HH
# define MIMOSA_FS_FIND_HH

# include <string>
# include <functional>
# include <cstdint>

namespace mimosa
{
  namespace fs
  {
    /**
     * @ingroup Fs
     * @param cb return false to break the loop
     * @param max_depth the maximum depth
     */
    void
    find(const std::string &                                    root,
         uint32_t                                               max_depth,
         const std::function<void (const std::string &)>       &cb);
  }
}

#endif /* !MIMOSA_FS_FIND_HH */
