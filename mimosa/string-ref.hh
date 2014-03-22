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
    inline void clear() { data_ = nullptr; len_ = 0; }

    inline const char *begin() const { return data_; }
    inline const char *end() const { return data_ + len_; }

    inline char operator[](size_type pos) const { assert(len_ > pos); return data_[pos]; }

    inline bool operator==(const StringRef & other) const {
      return memeq(other);
    }

    inline bool operator!=(const StringRef & other) const {
      return !(*this == other);
    }

    inline operator std::string() const {
      return std::string(data_, len_);
    }

    inline size_type find(char c, size_type pos = 0) const {
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

    inline bool memeq(const StringRef & other) const {
      return other.len_ == len_ && (other.data_ == data_ || !::memcmp(other.data_, data_, len_));
    }

    inline bool streq(const StringRef & other) const {
      return other.len_ == len_ && (other.data_ == data_ || !::strncmp(other.data_, data_, len_));
    }

    inline bool strcaseeq(const StringRef & other) const {
      return other.len_ == len_ && (other.data_ == data_ || !::strncasecmp(other.data_, data_, len_));
    }

    inline bool strncaseeq(const StringRef & other) const {
      return other.len_ <= len_ && (other.data_ == data_ || !::strncasecmp(other.data_, data_, other.len_));
    }

    inline StringRef substr(size_type start = 0, size_type n = npos) const {
      if (start >= len_)
        return StringRef();
      if (n == npos || start + n >= len_)
        n = len_ - start;
      return StringRef(data_ + start, n);
    }

    inline void eatWhitespaces(const StringRef & sp) {
      auto it = begin();
      auto e = end();
      for (; it < e && sp.find(*it) != npos; ++it)
        ;
      *this = substr(it - data_);
    }

    inline StringRef getLine(const StringRef & eol = "\n") const {
      const char  *r = (const char *)memmem(
        (const void *)data_, len_, (const void *)eol.data_, eol.len_);
      if (!r)
        return *this;
      return StringRef(data_, r - data_ + eol.size());
    }

    inline StringRef consumeLine(const StringRef & eol = "\n") {
      StringRef r = getLine(eol);
      *this = substr(r.size());
      return r;
    }

    inline StringRef getToken(const StringRef & sep = " \t\r\n\v") const {
      auto it = begin();
      auto e = end();
      for (; it < e && sep.find(*it) == npos; ++it)
        ;
      StringRef r(data_, it - data_);
      return r;
    }

    inline StringRef consumeToken(const StringRef & sep = " \t\r\n\v") {
      StringRef r = getToken(sep);
      *this = substr(r.size());
      return r;
    }

    template <typename Int>
    inline Int atoi() const {
      if (empty())
        return 0;

      Int value = 0;
      Int sign = 1;
      auto it = begin();
      auto e = end();
      if (*it == '-') {
        ++it;
        sign = -1;
      }

      for (; it < e && '0' <= *it && *it <= '9'; ++it)
        value = value * 10 + (*it - '0');
      return value * sign;
    }

    template <typename Int>
    inline Int atoiHex() const {
      if (empty())
        return 0;

      Int value = 0;
      Int sign = 1;
      auto it = begin();
      auto e = end();
      if (*it == '-') {
        ++it;
        sign = -1;
      }

      for (; it < e; ++it) {
        if ('0' <= *it && *it <= '9')
          value = value * 16 + (*it - '0');
        else if ('A' <= *it && *it <= 'F')
          value = value * 16 + (*it - 'A' + 10);
        else if ('a' <= *it && *it <= 'f')
          value = value * 16 + (*it - 'a' + 10);
        else
          break;
      }
      return value * sign;
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
