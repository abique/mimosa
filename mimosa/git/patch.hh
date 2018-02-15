#pragma once

namespace mimosa
{
  namespace git
  {
    class Patch
    {
    public:
      inline explicit Patch(git_patch * patch = nullptr) : patch_(patch) {}
      inline ~Patch() { git_patch_free(patch_); }

      inline operator git_patch *() { return patch_; }
      inline git_patch ** ref() { return &patch_; }

    private:
      git_patch * patch_;
    };
  }
}

