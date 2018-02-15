#pragma once

# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class HtmlEscape : public Filter
    {
    public:
      MIMOSA_DEF_PTR(HtmlEscape);

      explicit HtmlEscape(Stream::Ptr stream);

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);
    };
  }
}

