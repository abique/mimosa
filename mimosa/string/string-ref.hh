#ifndef MIMOSA_STRING_REF_HH
# define MIMOSA_STRING_REF_HH

# include <cstring>
# include <cassert>

namespace mimosa
{
  namespace string
  {
    class StringRef
    {
    public:
      inline StringRef() : data_(0), len_(0) {}
      inline StringRef(const char * string, size_t len = ::strlen(string)) : data_(string), len_(len) {}
      inline StringRef(const std::string & str) : data_(str.data()), len_(data.length()) {}

      inline size_t size() const { return len_; }
      inline const char * data() const { return data_; }

      inline char operator[](size_t pos) const { assert(len_ > pos); return data_[pos]; }

      inline bool operator==(const StringRef & other) const {
        return streq(other);
      }

      inline bool streq(const StringRef & other) const {
        other.len_ == len_ && (other.data_ == data_ || !::strncmp(other.data_, data_, len_));
      }
      inline bool strcaseeq(const StringRef & other) const {
        other.len_ == len_ && (other.data_ == data_ || !::strncasecmp(other.data_, data_, len_));
      }

      inline StringRef && substring(size_t start = 0, size_t n = -1) const {
        assert(start + n <= len_);
        return StringRef(data_ + start, n);
      }

    private:
      const char * data_;
      size_t       len_;
    };
  }
}

#endif /* !MIMOSA_STRING_REF_HH */
