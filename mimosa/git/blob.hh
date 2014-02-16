#ifndef MIMOSA_GIT_BLOB_HH
# define MIMOSA_GIT_BLOB_HH

# include <git2/blob.h>

namespace mimosa
{
  namespace git
  {
    class Blob
    {
    public:
      inline Blob(git_blob * blob = nullptr) : blob_(blob) {}
      inline ~Blob() { git_blob_free(blob_); }

      inline operator git_blob *() { return blob_; }
      inline git_blob ** ref() { return &blob_; }

    private:
      git_blob * blob_;
    };
  }
}

#endif /* !MIMOSA_GIT_BLOB_HH */
