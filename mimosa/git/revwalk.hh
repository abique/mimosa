#ifndef MIMOSA_GIT_REVWALK_HH
# define MIMOSA_GIT_REVWALK_HH

# include <git2.h>

namespace mimosa
{
  namespace git
  {
    class Revwalk
    {
    public:
      Revwalk(git_repository *repo);
      ~Revwalk();

      inline operator git_revwalk *() { return walk_; }

    private:
      git_revwalk *walk_;
    };
  }
}

#endif /* !MIMOSA_GIT_REVWALK_HH */
