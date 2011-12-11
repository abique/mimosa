#ifndef MIMOSA_STREAM_HTML_ESCAPE_HH
# define MIMOSA_STREAM_HTML_ESCAPE_HH

# include "filter.hh"

namespace mimosa
{
  namespace stream
  {
    class HtmlEscape : public Filter
    {
    public:
      MIMOSA_DEF_PTR(HtmlEscape);

      HtmlEscape(Stream::Ptr stream);

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);
    };
  }
}

#endif /* !MIMOSA_STREAM_HTML_ESCAPE_HH */
