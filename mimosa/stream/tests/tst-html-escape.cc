#include <gtest/gtest.h>
#include "../string-stream.hh"
#include "../html-escape.hh"

namespace mimosa
{
  namespace stream
  {
    namespace
    {

#define MY_TEST(Test, Input, Output)                            \
      TEST(HtmlEscape, Test)                                    \
      {                                                         \
        StringStream::Ptr stream = new StringStream;            \
        HtmlEscape::Ptr filter = new HtmlEscape(stream.get());  \
        filter->loopWrite(Input, sizeof (Input) - 1);           \
        ASSERT_EQ(Output, stream->str());                       \
      }

      MY_TEST(gt, ">", "&gt;")
      MY_TEST(lt, "<", "&lt;")
      MY_TEST(amp, "&", "&amp;")
      MY_TEST(apos, "'", "&apos;")
      MY_TEST(quot, "\"", "&quot;")

      MY_TEST(empty, "", "")
      MY_TEST(clean, "abcdef", "abcdef")
      MY_TEST(mixed1, "ab&", "ab&amp;")
      MY_TEST(mixed2, "&ab", "&amp;ab")
      MY_TEST(mixed3, "ab&ab", "ab&amp;ab")
      MY_TEST(mixed4, "ab&&&cd", "ab&amp;&amp;&amp;cd")

    }
  }
}
