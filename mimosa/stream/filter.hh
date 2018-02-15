#pragma once

# include <string>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    class Filter : public Stream
    {
    public:
      MIMOSA_DEF_PTR(Filter);

      explicit Filter(Stream::Ptr stream);

      virtual int64_t write(const char * data, uint64_t nbytes) = 0;
      virtual int64_t read(char * data, uint64_t nbytes) = 0;

      virtual bool flush();
      virtual void setReadTimeout(Time timeout);
      virtual void setWriteTimeout(Time timeout);

    protected:
      Stream::Ptr stream_;
    };

    template <typename T>
    inline std::string filter(const std::string & data);

    template <typename T>
    inline std::string filter(const char * data, uint64_t nbytes);
  }
}

# include "filter.hxx"

