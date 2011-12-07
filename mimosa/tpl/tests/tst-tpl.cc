#include <gtest/gtest.h>

#include "../template.hh"
#include "../value.hh"
#include "../../stream/string-stream.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace
    {
      TEST(Tpl, ValueDot)
      {
        Template::Ptr tpl = Template::parseString("hello {{.}}");
        Value<std::string> value("world!");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        EXPECT_EQ("hello world!", stream->str());
      }
    }
  }
}
