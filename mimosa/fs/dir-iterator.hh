#pragma once

# include <sys/types.h>
# include <dirent.h>

# include <string>

namespace mimosa
{
  namespace fs
  {
    /**
     * @ingroup Fs
     */
    class DirIterator
    {
    public:
      DirIterator(const std::string & path);
      ~DirIterator();

      inline bool ok() const { return dir_; }
      inline bool end() const { return !entry_; }

      inline const std::string & dirPath() const { return path_; }
      std::string entryPath() const;
      inline const char * entryName() const { return entry_->d_name; }

      DirIterator& operator++();

    private:
      ::DIR *     dir_;
      std::string path_;
      ::dirent *  entry_;
    };
  }
}

