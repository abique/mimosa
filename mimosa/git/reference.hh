#pragma once

# include <git2.h>

# include "../non-copyable.hh"
# include "repository.hh"

namespace mimosa
{
  namespace git
  {
    class Reference : private NonCopyable
    {
    public:
      inline Reference() : ref_(nullptr) {}
      inline ~Reference() { git_reference_free(ref_); }

      inline operator git_reference *() { return ref_; }
      inline git_reference ** ref() { return &ref_; }

    private:
      git_reference * ref_;
    };

    bool referenceToOid(const Repository &  repo,
                        const std::string & ref,
                        git_oid *           oid);
  }
}

