#ifndef MIMOSA_ARCHIVE_ENTRY_HH
# define MIMOSA_ARCHIVE_ENTRY_HH

# include <cstdint>

# include <archive_entry.h>

namespace mimosa
{
  namespace archive
  {
    class Entry
    {
    public:
      inline Entry() : entry_(archive_entry_new()) {}
      inline Entry(struct archive * archive) : entry_(archive_entry_new2(archive)) {}
      inline Entry(const Entry & e) : entry_(archive_entry_clone(e.entry_)) {}
      inline Entry(Entry && e) : entry_(e.entry_) { e.entry_ = nullptr; }
      inline ~Entry() { archive_entry_free(entry_); }
      inline Entry & operator=(const Entry & e) {
        archive_entry_free(entry_);
        entry_ = archive_entry_clone(e.entry_);
        return *this;
      }
      inline Entry & operator=(Entry && e) {
        archive_entry_free(entry_);
        entry_   = e.entry_;
        e.entry_ = nullptr;
        return *this;
      }

      inline operator struct archive_entry *() const { return entry_; }

      inline void clear() { archive_entry_clear(entry_); }

      ///////////////
      /// SETTERS ///
      ///////////////

      inline void setSize(uint64_t size) { archive_entry_set_size(entry_, size); }

      ///////////////
      /// GETTERS ///
      ///////////////

    private:
      struct archive_entry * entry_;
    };
  }
}

#endif /* !MIMOSA_ARCHIVE_ENTRY_HH */
