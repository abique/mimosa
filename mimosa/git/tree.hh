#pragma once

# include <string>

# include <git2/tree.h>

# include <mimosa/non-copyable.hh>

namespace mimosa
{
  namespace git
  {
    class Tree : private mimosa::NonCopyable
    {
    public:
      Tree(git_repository *    repo,
           const git_oid *     oid);
      Tree(git_repository *    repo,
           const git_oid *     oid,
           const std::string & directory);
      ~Tree();

      inline operator git_tree *() const { return tree_; }

    private:
      git_tree * tree_;
    };
  }
}

