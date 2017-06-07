#pragma once

# include <git2/diff.h>

namespace mimosa
{
  namespace git
  {
    class Diff
    {
    public:
      inline Diff(git_diff * diff = nullptr) : diff_(diff) {}
      inline ~Diff() { git_diff_free(diff_); }

      inline operator git_diff *() { return diff_; }
      inline git_diff ** ref() { return &diff_; }

    private:
      git_diff * diff_;
    };
  }
}

