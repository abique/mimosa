#ifndef MIMOSA_STRING_REF_HH
# define MIMOSA_STRING_REF_HH

# include <cstring>
# include <cassert>
# include <string>

namespace mimosa
{
  namespace string
  {
    class StringRef
    {
    public:
      static const size_t npos = -1;

      inline StringRef() : data_(0), len_(0) {}
      inline StringRef(const char * string) : data_(string), len_(::strlen(string)) {}
      inline StringRef(const char * string, size_t len) : data_(string), len_(len) {}
      inline StringRef(const char * string, const char * end) : data_(string), len_(end - string) {}
      inline StringRef(const std::string & str) : data_(str.data()), len_(str.size()) {}

      inline size_t size() const { return len_; }
      inline const char * data() const { return data_; }

      inline char operator[](size_t pos) const { assert(len_ > pos); return data_[pos]; }

      inline bool operator==(const StringRef & other) const {
        return streq(other);
      }

      inline size_t find(char c, size_t pos) const {
        if (pos >= len_)
          return npos;
        const char * found = static_cast<const char *>(::memchr(data_ + pos, c, len_ - pos));
        return found ? found - data_ : npos;
      }

      inline bool streq(const StringRef & other) const {
        return other.len_ == len_ && (other.data_ == data_ || !::strncmp(other.data_, data_, len_));
      }
      inline bool strcaseeq(const StringRef & other) const {
        return other.len_ == len_ && (other.data_ == data_ || !::strncasecmp(other.data_, data_, len_));
      }

      inline StringRef substr(size_t start = 0, size_t n = npos) const {
        if (start >= len_)
          return StringRef();
        if (n == npos || start + n >= len_)
          n = len_ - start;
        return StringRef(data_ + start, n);
      }

    private:
      const char * data_;
      size_t       len_;
    };
  }
}

#endif /* !MIMOSA_STRING_REF_HH */
