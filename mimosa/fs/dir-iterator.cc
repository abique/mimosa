#include "dir-iterator.hh"

namespace mimosa
{
  namespace fs
  {
    DirIterator::DirIterator(const std::string & path)
      : dir_(::opendir(path.c_str())),
        path_(path),
        entry_(nullptr)
    {
      ++*this;
    }

    DirIterator::~DirIterator()
    {
      if (dir_)
        ::closedir(dir_);
    }

    DirIterator&
    DirIterator::operator++()
    {
      if (dir_)
        entry_ = ::readdir(dir_);

      if (entry_ && entry_->d_name[0] == '.' &&
          (entry_->d_name[1] == '\0' ||
           (entry_->d_name[1] == '.' && entry_->d_name[2] == '\0')))
        return ++*this;
      return *this;
    }

    std::string
    DirIterator::entryPath() const
    {
      std::string entry_path(path_);

      if (*entry_path.rbegin() != '/')
        entry_path.append("/");
      entry_path.append(entry_->d_name);
      return entry_path;
    }
  }
}
