#pragma once

# include <string>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class StringStream : public Stream
    {
    public:
      MIMOSA_DEF_PTR(StringStream);

      explicit StringStream(const std::string & str = "");

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      inline const std::string & str() const { return str_; }
      inline std::string && moveStr() { read_pos_ = 0; return std::move(str_); }
      inline void setStr(const std::string & str)
      {
        read_pos_ = 0;
        str_      = str;
      }
      inline void setStr(std::string && str)
      {
        read_pos_ = 0;
        str_      = str;
      }

      inline void clear() { read_pos_ = 0; str_.clear(); }

    private:
      std::string::size_type read_pos_;
      std::string            str_;
    };
  }
}

