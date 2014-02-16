#include "repository.hh"
#include "commit.hh"

namespace mimosa
{
  namespace git
  {
    Repository::Repository(const std::string & repo_path)
      : repo_(nullptr)
    {
      git_repository_open(&repo_, repo_path.c_str());
    }

    Repository::~Repository()
    {
      git_repository_free(repo_);
    }

    int64_t
    Repository::lastCommitTime() const
    {
      git_oid   oid;
      if (git_reference_name_to_id(&oid, repo_, "HEAD"))
        return 0;

      Commit commit(repo_, &oid);
      return static_cast<int64_t>(git_commit_time(commit)) * 1000
        + git_commit_time_offset(commit);
    }
  }
}
