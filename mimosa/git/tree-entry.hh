#pragma once

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

