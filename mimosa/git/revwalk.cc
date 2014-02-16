#include "revwalk.hh"

namespace mimosa
{
  namespace git
  {
    Revwalk::Revwalk(git_repository * repo)
      : walk_(nullptr)
    {
      git_revwalk_new(&walk_, repo);
    }

    Revwalk::~Revwalk()
    {
      git_revwalk_free(walk_);
    }
  }
}
