#pragma once

#include <cstring>
#include <cassert>
#include <string>
#include <ostream>
#include <vector>

#include "preproc.hh"

namespace mimosa
{
  class StringRef
  {
  public:

    typedef size_t size_type;

    static const size_type npos = -1;

    constexpr StringRef() noexcept = default;
    constexpr StringRef(const StringRef &str) noexcept : data_(str.data()), len_(str.size()) {}
    inline StringRef(const char * string) noexcept : data_(string), len_(::strlen(string)) {}
    constexpr inline StringRef(const char * string, size_type len) noexcept : data_(string), len_(len) {}
    constexpr inline StringRef(const char * string, const char * end) noexcept : data_(string), len_(end - string) { assert(end >= string); }
    inline StringRef(const std::string & str) noexcept : data_(str.data()), len_(str.size()) {}

    constexpr size_type size() const noexcept { return len_; }
    constexpr const char *data() const noexcept { return data_; }
    constexpr const char *c_str() const noexcept { return data_; }
    constexpr bool empty() const noexcept { return !data_ || !len_; }
    void clear() noexcept { data_ = nullptr; len_ = 0; }

    constexpr const char *begin() const noexcept { return data_; }
    constexpr const char *end() const noexcept { return data_ + len_; }

    inline StringRef& operator=(const StringRef &str) noexcept
    {
      data_ = str.data_;
      len_ = str.len_;
      return *this;
    }

    constexpr char operator[](size_type pos) const noexcept { assert(len_ > pos); return data_[pos]; }

    bool operator==(const StringRef & other) const noexcept {
      return memeq(other);
    }

    bool operator!=(const StringRef & other) const noexcept {
      return !(*this == other);
    }

    std::string asStdString() const {
        return std::string(data_, len_);
    }

    explicit inline operator std::string() const {
      return std::string(data_, len_);
    }

    inline size_type find(char c, size_type pos = 0) const noexcept {
      if (pos >= len_)
        return npos;
      const char * found = static_cast<const char *>(::memchr(data_ + pos, c, len_ - pos));
      return found ? found - data_ : npos;
    }

    inline size_type find(const char * str, size_type pos, size_type n) const noexcept {
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

    inline size_type find(const char * str, size_type pos = 0) const noexcept {
      return find(str, pos, ::strlen(str));
    }

    inline size_type find(const std::string & str, size_type pos = 0) const noexcept {
      return find(str.c_str(), pos, str.size());
    }

    inline bool memeq(const StringRef & other) const noexcept {
      return other.len_ == len_ && (other.data_ == data_ || !::memcmp(other.data_, data_, len_));
    }

    inline bool streq(const StringRef & other) const noexcept {
      return other.len_ == len_ && (other.data_ == data_ || !::strncmp(other.data_, data_, len_));
    }

    inline bool strcaseeq(const StringRef & other) const noexcept {
      return other.len_ == len_ && (other.data_ == data_ || !::strncasecmp(other.data_, data_, len_));
    }

    inline bool strncaseeq(const StringRef & other) const noexcept {
      return other.len_ <= len_ && (other.data_ == data_ || !::strncasecmp(other.data_, data_, other.len_));
    }

    inline StringRef substr(size_type start = 0, size_type n = npos) const noexcept {
      if (start >= len_)
        return StringRef();
      if (n == npos || start + n >= len_)
        n = len_ - start;
      return StringRef(data_ + start, n);
    }

    inline void eatWhitespaces(const StringRef & sp) noexcept {
      auto it = begin();
      auto e = end();
      for (; it < e && sp.find(*it) != npos; ++it)
        ;
      *this = substr(it - data_);
    }

    inline StringRef getLine(const StringRef & eol = "\n") const noexcept {
#if defined(MIMOSA_UNIX)
      const char  *r = (const char *)memmem(
        (const void *)data_, len_, (const void *)eol.data_, eol.len_);
      if (!r)
        return *this;
      return StringRef(data_, r - data_ + eol.size());
#else
	  # error "missing implementation on this platform"
#endif
    }

    inline StringRef consumeLine(const StringRef & eol = "\n") noexcept {
      StringRef r = getLine(eol);
      *this = substr(r.size());
      return r;
    }

    inline StringRef getToken(const StringRef & sep = " \t\r\n\v") const noexcept {
      auto it = begin();
      auto e = end();
      for (; it < e && sep.find(*it) == npos; ++it)
        ;
      StringRef r(data_, it - data_);
      return r;
    }

    inline StringRef consumeToken(const StringRef & sep = " \t\r\n\v") noexcept {
      StringRef r = getToken(sep);
      *this = substr(r.size());
      return r;
    }

    template <typename Int>
    inline Int atoi() const noexcept {
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
    inline Int atoiHex() const noexcept {
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

    inline std::vector<StringRef> tokens(char c) const noexcept
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
    const char * data_ = nullptr;
    size_type    len_ = 0;
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
