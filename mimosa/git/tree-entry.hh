#ifndef MIMOSA_GIT_TREE_ENTRY_HH
# define MIMOSA_GIT_TREE_ENTRY_HH

namespace mimosa
{
  namespace git
  {
    class TreeEntry
    {
    public:
      inline TreeEntry(git_tree_entry * entry = nullptr) : entry_(entry) {}
      inline ~TreeEntry() { git_tree_entry_free(entry_); }

      inline operator git_tree_entry *() { return entry_; }
      inline git_tree_entry ** ref() { return &entry_; }

    private:
      git_tree_entry * entry_;
    };
  }
}

#endif /* !MIMOSA_GIT_TREE_ENTRY_HH */
