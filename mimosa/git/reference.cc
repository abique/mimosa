#include <cstring>

#include "reference.hh"
#include "tag.hh"

namespace mimosa
{
  namespace git
  {
    bool referenceToOid(const Repository &  repo,
                        const std::string & ref,
                        git_oid *           oid)
    {
      if (!git_oid_fromstrp(oid, ref.c_str()))
        return true;
      if (!git_reference_name_to_id(oid, repo, ref.c_str()))
        return true;

      Reference r;
      if (!git_reference_dwim(r.ref(), repo, ref.c_str())) {
        const git_oid *target = git_reference_target(r);
        if (target) {
          memcpy(oid, target, sizeof (*target));
          return true;
        }
      }

      return false;
    }
  }
}
