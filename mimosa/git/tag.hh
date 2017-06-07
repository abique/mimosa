#pragma once

# include <git2.h>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace git
  {
    class Tag : public NonCopyable
    {
    public:
      inline Tag() : tag_(nullptr) {}
      inline ~Tag() { git_tag_free(tag_); }

      inline operator git_tag *() const { return tag_; }
      inline git_tag ** ref() { return &tag_; }

    private:
      git_tag * tag_;
    };
  }
}

