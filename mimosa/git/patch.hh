#ifndef MIMOSA_GIT_PATCH_HH
# define MIMOSA_GIT_PATCH_HH

namespace mimosa
{
  namespace git
  {
    class Patch
    {
    public:
      inline Patch(git_patch * patch = nullptr) : patch_(patch) {}
      inline ~Patch() { git_patch_free(patch_); }

      inline operator git_patch *() { return patch_; }
      inline git_patch ** ref() { return &patch_; }

    private:
      git_patch * patch_;
    };
  }
}

#endif /* !MIMOSA_GIT_PATCH_HH */
