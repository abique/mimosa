#pragma once

# include <cstdint>
# include <string>

# include <git2.h>

namespace mimosa
{
  namespace git
  {
    class Repository
    {
    public:
      explicit Repository(const std::string & repo_path);
      ~Repository();

      inline operator git_repository * () const { return repo_; }

      int64_t lastCommitTime() const;

    private:
      git_repository *repo_;
    };
  }
}

