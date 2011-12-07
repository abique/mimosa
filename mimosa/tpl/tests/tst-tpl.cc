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
      TEST(Tpl, Value)
      {
        Template::Ptr tpl = Template::parseString("hello {{value1}}");
        Value<std::string> value("world!", "value1");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        EXPECT_EQ("hello world!", stream->str());
      }

      TEST(Tpl, ValueDot)
      {
        Template::Ptr tpl = Template::parseString("hello {{.}}");
        Value<std::string> value("world!");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        EXPECT_EQ("hello world!", stream->str());
      }

      TEST(Tpl, 2ValueDot)
      {
        Template::Ptr tpl = Template::parseString("hello {{.}} {{.}}");
        Value<int> value(2);

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        EXPECT_EQ("hello 2 2", stream->str());
      }
    }
  }
}
