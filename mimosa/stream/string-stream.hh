#ifndef MIMOSA_STREAM_STRING_STREAM_HH
# define MIMOSA_STREAM_STRING_STREAM_HH

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

      StringStream(const std::string & str = "");

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      inline const std::string & str() const { return str_; }
      inline void setStr(const std::string & str)
      {
        read_pos_ = 0;
        str_ = str;
      }

    private:
      std::string::size_type read_pos_;
      std::string            str_;
    };
  }
}

#endif /* !MIMOSA_STREAM_STRING_STREAM_HH */
