#pragma once

# include <git2.h>

namespace mimosa
{
  namespace git
  {
    class Revwalk
    {
    public:
      explicit Revwalk(git_repository *repo);
      ~Revwalk();

      inline operator git_revwalk *() { return walk_; }

    private:
      git_revwalk *walk_;
    };
  }
}

