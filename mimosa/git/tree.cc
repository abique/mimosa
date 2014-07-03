#include "tree.hh"
#include "tree-entry.hh"

namespace mimosa
{
  namespace git
  {
    Tree::Tree(git_repository * repo,
               const git_oid *  id)
      : tree_(nullptr)
    {
      if (id)
        git_tree_lookup(&tree_, repo, id);
    }

    Tree::Tree(git_repository *    repo,
               const git_oid *     id,
               const std::string & directory)
      : tree_(nullptr)
    {
      if (directory.empty()) {
        git_tree_lookup(&tree_, repo, id);
        return;
      }

      Tree      root(repo, id);
      TreeEntry entry;
      if (git_tree_entry_bypath(entry.ref(), root, directory.c_str()))
        return;

      if (git_tree_entry_type(entry) != GIT_OBJ_TREE)
        return;

      git_tree_lookup(&tree_, repo, git_tree_entry_id(entry));
    }

    Tree::~Tree()
    {
      git_tree_free(tree_);
    }
  }
}
