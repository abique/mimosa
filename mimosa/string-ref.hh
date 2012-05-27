#ifndef MIMOSA_STRING_REF_HH
# define MIMOSA_STRING_REF_HH

# include <cstring>
# include <cassert>
# include <string>
# include <ostream>
# include <vector>

namespace mimosa
{
  class StringRef
  {
  public:

    typedef size_t size_type;

    static const size_type npos = -1;

    inline StringRef() : data_(0), len_(0) {}
    inline StringRef(const char * string) : data_(string), len_(::strlen(string)) {}
    inline StringRef(const char * string, size_type len) : data_(string), len_(len) {}
    inline StringRef(const char * string, const char * end) : data_(string), len_(end - string) {}
    inline StringRef(const std::string & str) : data_(str.data()), len_(str.size()) {}

    inline size_type size() const { return len_; }
    inline const char * data() const { return data_; }
    inline bool empty() const { return !data_ || !len_; }

    inline char operator[](size_type pos) const { assert(len_ > pos); return data_[pos]; }

    inline bool operator==(const StringRef & other) const {
      return streq(other);
    }

    inline bool operator!=(const StringRef & other) const {
      return !(*this == other);
    }

    inline operator std::string() const {
      return std::string(data_, len_);
    }

    inline size_type find(char c, size_type pos) const {
      if (pos >= len_)
        return npos;
      const char * found = static_cast<const char *>(::memchr(data_ + pos, c, len_ - pos));
      return found ? found - data_ : npos;
    }

    inline size_type find(const char * str, size_type pos, size_type n) const {
      if (n + pos > len_ || n == 0)
        return npos;

      const char * const start_limit = data_ + len_ - n + pos;
      for (const char * start = data_; start <= start_limit; start++)
      {
        start = (const char *)::memchr(start, *str, len_ - (start - data_));
        if (!start)
          return npos;
        if (!::memcmp(start + 1, str + 1, n - 1))
          return start - data_;
      }

      return npos;
    }

    inline size_type find(const char * str, size_type pos = 0) const {
      return find(str, pos, ::strlen(str));
    }

    inline size_type find(const std::string & str, size_type pos = 0) const {
      return find(str.c_str(), pos, str.size());
    }

    inline bool streq(const StringRef & other) const {
      return other.len_ == len_ && (other.data_ == data_ || !::memcmp(other.data_, data_, len_));
    }
    inline bool strcaseeq(const StringRef & other) const {
      return other.len_ == len_ && (other.data_ == data_ || !::strncasecmp(other.data_, data_, len_));
    }

    inline StringRef substr(size_type start = 0, size_type n = npos) const {
      if (start >= len_)
        return StringRef();
      if (n == npos || start + n >= len_)
        n = len_ - start;
      return StringRef(data_ + start, n);
    }

    inline std::vector<StringRef> tokens(char c) const
    {
      std::vector<StringRef> toks;

      const char * const end = data_ + len_;
      const char * p = data_;

      while (p < end)
      {
        while (*p == c && p < end)
          ++p;

        const char * const start = p;

        while (*p != c && p < end)
          ++p;

        if (p > start)
          toks.push_back(StringRef(start, p));
      }

      return toks;
    }

  private:
    const char * data_;
    size_type    len_;
  };
}

namespace std
{
  inline
  std::ostream & operator<<(std::ostream & os, const mimosa::StringRef & str)
  {
    os.write(str.data(), str.size());
    return os;
  }
}

#endif /* !MIMOSA_STRING_REF_HH */
