#pragma once

# include <git2.h>

# include "../mimosa/mimosa/non-copyable.hh"

namespace mimosa
{
  namespace git
  {
    class Commit : public NonCopyable
    {
    public:
      inline Commit() : commit_(nullptr) {}
      inline Commit(git_repository *repo, const git_oid *id) {
        git_commit_lookup(&commit_, repo, id);
      }
      inline ~Commit() { git_commit_free(commit_); }

      inline git_commit** ref() { return &commit_; }
      inline operator git_commit *() const { return commit_; }

    private:
      git_commit *commit_;
    };
  }
}

