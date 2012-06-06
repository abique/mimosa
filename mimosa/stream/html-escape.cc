#include <cassert>

#include "html-escape.hh"

namespace mimosa
{
  namespace stream
  {
    HtmlEscape::HtmlEscape(Stream::Ptr stream)
      : Filter(stream)
    {
    }

    int64_t
    HtmlEscape::write(const char * data, uint64_t nbytes)
    {
      const char * const start = data;
      const char * const end   = data + nbytes;
      const char * p           = data;

      while (data < end)
      {
        while (p < end && *p != '<' && *p != '>' &&
               *p != '&' && *p != '"' && *p != '\'')
          ++p;

        if (data < p)
        {
          auto bytes = stream_->write(data, p - data);
          if (bytes < 0)
            return data == start ? bytes : data - start;
          if (bytes < p - data)
            return data - start + bytes;
          data = p;
        }

        if (p < end)
        {
          const char * replace = nullptr;
          int          size    = 0;

          switch (*p)
          {
          case '<':  replace = "&lt;";   size = 4; break;
          case '>':  replace = "&gt;";   size = 4; break;
          case '&':  replace = "&amp;";  size = 5; break;
          case '\'': replace = "&apos;"; size = 6; break;
          case '"':  replace = "&quot;"; size = 6; break;
          default: assert(false); break;
          }

          auto wrote = stream_->loopWrite(replace, size);
          if (wrote < 0)
            return data - start;
          if (wrote < size)
            return -1;
          ++data;
          ++p;
        }
      }
      return nbytes;
    }

    int64_t
    HtmlEscape::read(char * data, uint64_t nbytes)
    {
      assert(false && "TODO");
      errno = ENOSYS;
      return -1;
    }
  }
}
