#pragma once

# include <cstdint>
# include <string>

# include <archive_entry.h>

namespace mimosa
{
  namespace archive
  {
    class Entry
    {
    public:
      inline Entry() : entry_(archive_entry_new()) {}
      inline explicit Entry(struct archive * archive) : entry_(archive_entry_new2(archive)) {}
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
      inline Entry & operator=(struct archive_entry *entry) {
        archive_entry_free(entry_);
        entry_ = entry;
        return *this;
      }

      inline operator struct archive_entry *() const { return entry_; }

      inline void clear() { archive_entry_clear(entry_); }

      ///////////////
      /// SETTERS ///
      ///////////////

      inline void setSize(uint64_t size) { archive_entry_set_size(entry_, size); }
      inline void setPathname(const std::string &str) { archive_entry_set_pathname_utf8(entry_, str.c_str()); }

      ///////////////
      /// GETTERS ///
      ///////////////

    private:
      struct archive_entry * entry_;
    };
  }
}

